#include "IoCServiceProxy.h"

#include <SelfPortrait/reflection.h>
#include <SelfPortrait/str_conversion.h>

#include <stdexcept>
#include "model/IoCReferenceBinding.h"
#include "model/IoCImplementation.h"
#include "model/IoCInterface.h"
#include "VariantToParameter.h"
#include <tuscany/sca/model/ServiceType.h>


using namespace std;
using namespace tuscany::sca::model;
using namespace tuscany::sca;
using namespace tuscany::sca::util;

IoCServiceProxy::IoCServiceProxy(tuscany::sca::model::Reference* sourceReference, tuscany::sca::model::ServiceBinding* serviceBinding)
	: ServiceProxy(sourceReference)
	, m_libraries()
	, m_proxies()
{
	addProxy(sourceReference, serviceBinding);
}

void IoCServiceProxy::addProxy(tuscany::sca::model::Reference* sourceReference, tuscany::sca::model::ServiceBinding* serviceBinding)
{
	addProxy(sourceReference, serviceBinding, m_proxies, m_libraries);
}


void IoCServiceProxy::addProxy(tuscany::sca::model::Reference* sourceReference, tuscany::sca::model::ServiceBinding* serviceBinding, std::vector<ProxyData>& proxies, std::vector<tuscany::sca::util::Library>& libs)
{
	// aqui temos a oportunidade de fazer a ligacao direta se do outro lado for um IoCServiceWrapper

	Component* sourceComponent = sourceReference->getComponent();
	ComponentType* componentType = sourceComponent->getType();

	// Verifiy the component type
	if (componentType == nullptr) {
		throw std::logic_error(fmt_str("Component %1 has no implementation defined", sourceComponent->getName()));
	}

	auto impl = dynamic_cast<IoCImplementation*>(componentType);
	if (impl == nullptr) {
		throw std::logic_error(fmt_str("Component implementation is not of the expected type"));
	}

	// Verify the reference interface. This is resolved from the xml definitions,
	// The actual type might be different
	ReferenceType* rtype = sourceReference->getType();
	auto rIFace = dynamic_cast<IoCInterface*>(rtype->getInterface());
	if (rIFace == nullptr) {
		throw std::logic_error(fmt_str("Reference interface is not of the expected type"));
	}

	// Libraries, at least opened with dlopen, are ref-counted. So for every
	// open we try to load the library
	string library = impl->getLibrary();
	string fullLibraryName = sourceComponent->getComposite()->getRoot() + "/" + library;
	libs.emplace_back(fullLibraryName);

	string ifaceClass = rIFace->getClass();

	Class clazz = Class::lookup(ifaceClass);

	if (!clazz.isValid()) {
		throw std::logic_error(fmt_str("Cannot find a class called %1", ifaceClass));
	}

	proxies.emplace_back(std::move(createProxy(clazz, serviceBinding->getServiceWrapper())), clazz);
}


Proxy IoCServiceProxy::createProxy(Class clazz, tuscany::sca::ServiceWrapper* serviceWrapper)
{
	if (!clazz.isInterface()) {
		throw std::logic_error(fmt_str("Class %1 is not an interface", clazz.fullyQualifiedName()));
	}

	Proxy proxy{clazz};

	// Add a generic implementation for each method

	for (const Method m: clazz.methods()) {

		proxy.addImplementation(m, [=](const std::vector<VariantValue>& vargs) -> VariantValue {

								Operation op(m.name().c_str());

					// pack params
					for (size_t i = 0; i < vargs.size(); ++i) {
				addParameter(i, op, vargs[i]);
			}

			serviceWrapper->invoke(op);
			// pack retval
			return std::move(convertToVariant(op.getReturnType(), op.getReturnValue()));
		});
	}
	return std::move(proxy);
}


std::list<VariantValue> IoCServiceProxy::proxies() const
{
	std::list<VariantValue>	impls;
	for (const ProxyData& data: m_proxies) {
		impls.emplace_back(data.proxy.reference(data.clazz));
	}
	return impls;
}
