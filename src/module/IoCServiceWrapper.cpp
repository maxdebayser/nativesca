#include "IoCServiceWrapper.h"

#include "VariantToParameter.h"
#include <SelfPortrait/reflection.h>
#include <SelfPortrait/str_conversion.h>
#include <tuscany/sca/core/SCARuntime.h>
#include <tuscany/sca/model/ServiceType.h>
#include "ComponentContext.h"

#include <commonj/sdo/SDO.h>

#include <stdexcept>
#include <string>
#include <regex>
#include <ctype.h>
#include <algorithm>
using namespace std;

#include "ScopeGuard.h"

using namespace tuscany::sca;
using namespace tuscany::sca::util;
using namespace puc::sca;
using namespace commonj::sdo;

namespace {

	VariantValue createDefault(const Class& c) {
		auto constructor = c.findConstructor([](const Constructor& co) { return co.isDefaultConstructor(); });

		if (!c.isValid()) {
			throw std::logic_error(fmt_str("Class %1 has no default constructor", c.fullyQualifiedName()));
		}
		return std::move(constructor.call());
	}
	std::list<Method> filterOverriden(const std::list<Method>& input) {
		std::list<Method> ret;

		for (auto& m: input) {
			// if no method overrides m, we include it
			if (find_if(begin(input), end(input), [&](const Method& t) { return overrides(t, m); }) == end(input)) {
				ret.push_back(m);
			}
		}
		return ret;
	}
}

IoCServiceWrapper::IoCServiceWrapper(tuscany::sca::model::Service* service)
	: ServiceWrapper(service)
	, m_component(service->getComponent())
	, m_interface(service->getType()->getInterface())
	, m_implementation(dynamic_cast<IoCImplementation*>(m_component->getType()))
{
	if (m_implementation == nullptr) {
		throw std::logic_error(fmt_str("Component %1 has an incorrect implementation type or has no implementation defined", m_component->getName()));
	}

	string libraryName = m_implementation->getLibrary();
	string metadata = m_implementation->getMDLibrary();


	// ------------
	// Load the dll
	// ------------
	string fullLibraryName = m_component->getComposite()->getRoot() + "/" + libraryName;
	m_wrapperLibrary = Library(fullLibraryName);

	if (!metadata.empty()) {
		m_MDLibrary = Library(m_component->getComposite()->getRoot() + "/" + metadata);
	}

	// Find the class and set up setter info
	setupClassInfo();
}

IoCServiceWrapper::~IoCServiceWrapper()
{}

void IoCServiceWrapper::setupClassInfo()
{
	m_class = Class::lookup(m_implementation->getClass());

	if (!m_class.isValid()) {
		throw std::logic_error(fmt_str("Cannot find class %1 of component %2", m_implementation->getClass(), m_component->getName()));
	}

	for (auto& attr: m_class.attributes()) {
		m_attrs[attr.name()] = attr;
	}

	MethodMap preprocessed;

	regex rx("set.+"); // This is what gcc supports right now (no character classes yet :( )

	Class::MethodList oneArg = m_class.findAllMethods([&](const Method& m){
		return !m.isStatic() &&
			   !m.isConst() &&
			   m.numberOfArguments() == 1 &&
			   regex_match(m.name(), rx);                  ;
	});

	for (auto& m: oneArg) {
		preprocessed[m.name()].push_back(m);
	}

	for (auto p: preprocessed) {

		string suffix = p.first.substr(3);

		if (!isupper(suffix.at(0))) {
			continue;
		}
		suffix.at(0) = tolower(suffix.at(0));

		m_setters[suffix] = filterOverriden(p.second);
	}
}

void IoCServiceWrapper::configureComponent(VariantValue& object, std::list<VariantValue>& keepAlive)
{
	if (!object.isValid()) {
		throw std::logic_error("invalid object");
	}

	ComponentContext context = ComponentContext::getCurrent();
	DataObjectPtr properties = context.getProperties();

	auto propList = properties->getInstanceProperties();
	for (int i = 0; i < propList.size(); ++i) {
		const Property& prop = propList[i];
		string property = prop.getName();

		VariantValue propVal = convertToVariant(prop, properties);
		keepAlive.emplace_back(std::move(propVal.createReference()));
		auto attrIt = m_attrs.find(property);
		if (attrIt != m_attrs.end()) {
			attrIt->second.set(object, propVal);
			continue;
		}

		auto methIt = m_setters.find(property);
		if (methIt != m_setters.end()) {
			const std::list<Method>& setters = methIt->second;

			if (setters.size() > 1) {
				throw std::logic_error(fmt_str("found more than one setter method %1: overloads are not yet supported", setters.front().name()));
			}

			std::vector<VariantValue> vargs { std::move(propVal) };
			setters.front().callArgArray(object, vargs);

			continue;
		}
		throw std::logic_error(fmt_str("Couldn't find attribute or setter for property: %1", property));

	}

	std::list<string> referenceNames;
	for( auto ref: m_component->getReferences() ) {
		referenceNames.push_back(ref.first);
	}

	for (auto name: referenceNames) {
		VariantValue service = context.getService(name);
		keepAlive.emplace_back(std::move(service.createReference()));

		auto attrIt = m_attrs.find(name);
		if (attrIt != m_attrs.end()) {
			attrIt->second.set(object, keepAlive.back());
			continue;
		}
		auto methIt = m_setters.find(name);
		if (methIt != m_setters.end()) {
			const std::list<Method>& setters = methIt->second;

			if (setters.size() > 1) {
				throw std::logic_error(fmt_str("found more than one setter method %1: overloads are not yet supported", setters.front().name()));
			}

			std::vector<VariantValue> vargs;
			vargs.emplace_back(std::move(service.createReference()));
			setters.front().callArgArray(object, vargs);

			continue;
		}
		throw std::logic_error(fmt_str("Couldn't find attribute or setter for reference: %1", name));
	}
}


void IoCServiceWrapper::invoke(Operation& operation)
{
	SCARuntime* runtime = SCARuntime::getCurrentRuntime();
	runtime->setCurrentComponent(m_component);
	LOKI_ON_BLOCK_EXIT_OBJ(*runtime, &SCARuntime::unsetCurrentComponent);

	VariantValue object;

	std::list<VariantValue> keepAlive;

	if (m_implementation->getScope() == IoCImplementation::COMPOSITE) {
		if (!m_instance.isValid()) {
			m_instance = createDefault(m_class);
			configureComponent(m_instance, m_keepAlive);
		}
		object = m_instance.createReference();
	} else {
		object = createDefault(m_class);
		configureComponent(object, keepAlive);
	}

	const std::string& operationName = operation.getName();
	const int numParams = operation.getNParms();

	/* TODO: Simplistic treatment
	 *
	 * We should match the parameter and return types. Also in the
	 * ocurrence of more than one method with the same name and
	 * signature we have to check for overriden methods
	 */

	Class::MethodList methods = filterOverriden(m_class.findAllMethods([&](const Method& m) { return m.name() == operationName && m.numberOfArguments() == numParams; }));

	if (methods.empty()) {
		throw std::logic_error(fmt_str("Class %1 has no method called %2 with %3 parameters", m_class.fullyQualifiedName(), operationName, numParams));
	}

	if (methods.size() > 1) {
		throw std::logic_error("Overloads are not yet supported");
	}

	Method method = methods.front();

	std::vector<VariantValue> vargs;

	for (size_t i = 0; i < numParams; ++i) {
		vargs.emplace_back(convertToVariant(operation.getParameter(i)));
	}

	VariantValue ret = method.callArgArray(object, vargs);

	setReturnValue(operation, ret);
}
