#include "IoCServiceWrapper.h"

#include "VariantToParameter.h"
#include <SelfPortrait/reflection.h>
#include <SelfPortrait/str_conversion.h>
#include <tuscany/sca/core/SCARuntime.h>
#include <tuscany/sca/model/ServiceType.h>

#include <commonj/sdo/SDO.h>

#include <stdexcept>
#include <string>
using namespace std;

#include "ScopeGuard.h"

using namespace tuscany::sca;
using namespace tuscany::sca::util;

namespace {

	VariantValue createDefault(const Class& c) {
		auto constructor = c.findConstructor([](const Constructor& co) { return co.isDefaultConstructor(); });

		if (!c.isValid()) {
			throw std::logic_error(fmt_str("Class %1 has no default constructor", c.fullyQualifiedName()));
		}
		return std::move(constructor.call());
	}
}

IoCServiceWrapper::IoCServiceWrapper(tuscany::sca::model::Service* service)
	: ServiceWrapper(service)
{
	m_component = service->getComponent();
	m_interface = service->getType()->getInterface();
	m_implementation = dynamic_cast<IoCImplementation*>(m_component->getType());
	if (m_implementation == nullptr) {
		throw std::logic_error(fmt_str("Component %1 has an incorrect implementation type or has no implementation defined", m_component->getName()));
	}

	string libraryName = m_implementation->getLibrary();

	// ------------
	// Load the dll
	// ------------
	string fullLibraryName = m_component->getComposite()->getRoot() + "/" + libraryName;
	m_wrapperLibrary = Library(fullLibraryName);

	// Locate the class

	m_class = Class::lookup(m_implementation->getClass());

	if (!m_class.isValid()) {
		throw std::logic_error(fmt_str("Cannot find class %1 of component %2", m_implementation->getClass(), m_component->getName()));
	}

	if (m_implementation->getScope() == IoCImplementation::COMPOSITE) {
		m_instance = createDefault(m_class);
	}
}


IoCServiceWrapper::~IoCServiceWrapper()
{}

void IoCServiceWrapper::invoke(Operation& operation)
{
	SCARuntime* runtime = SCARuntime::getCurrentRuntime();
	runtime->setCurrentComponent(m_component);
	LOKI_ON_BLOCK_EXIT_OBJ(*runtime, &SCARuntime::unsetCurrentComponent);


	VariantValue object;

	if (m_implementation->getScope() == IoCImplementation::COMPOSITE) {
		object = m_instance.createReference();
	} else {
		object = createDefault(m_class);
	}

	if (!object.isValid()) {
		throw std::logic_error("invalid object");
	}

	const std::string& operationName = operation.getName();
	const int numParams = operation.getNParms();

	/* TODO: Simplistic treatment
	 *
	 * We should match the parameter and return types. Also in the
	 * ocurrence of more than one method with the same name and
	 * signature we have to check for overriden methods
	 */

	Class::MethodList methods = m_class.findAllMethods([&](const Method& m) { return m.name() == operationName && m.numberOfArguments() == numParams && m.getClass() == m_class; });

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
