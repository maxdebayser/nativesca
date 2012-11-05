#include "CompositeContext.h"

#include <tuscany/sca/core/SCARuntime.h>
#include <tuscany/sca/model/Composite.h>
#include <tuscany/sca/model/Component.h>
#include <tuscany/sca/model/Service.h>
#include <tuscany/sca/model/ServiceType.h>
#include <stdexcept>
#include <SelfPortrait/reflection.h>
#include "IoCServiceProxy.h"
#include "model/IoCImplementation.h"
#include "model/IoCInterface.h"

using namespace std;
using namespace tuscany::sca;
using namespace tuscany::sca::model;
using namespace tuscany::sca::util;

namespace puc {

	namespace sca {

		CompositeContext CompositeContext::getCurrent()
		{
			return {SCARuntime::getCurrentRuntime()->getDefaultComponent()};
		}

		CompositeContext::CompositeContext(Component* component)
			: m_component(component), m_composite(nullptr)
		{
			m_composite = dynamic_cast<Composite*>(component->getType());
			if (m_composite == nullptr) {
				throw std::logic_error(fmt_str("Component %1 is not a composite", m_component->getName()));
			}
		}

		CompositeContext::CompositeContext(const CompositeContext& that)
			: m_component(that.m_component), m_composite(that.m_composite) {}

		CompositeContext& CompositeContext::operator=(CompositeContext that) {
			swap(that);
			return *this;
		}

		void CompositeContext::swap(CompositeContext& that) {
			std::swap(this->m_component, that.m_component);
			std::swap(this->m_composite, that.m_composite);
		}

		VariantValue CompositeContext::locateService(const std::string& serviceName)
		{
			Service* service = m_composite->findComponentService(serviceName);
			if (service == nullptr) {
				throw std::logic_error(fmt_str("Service %1 not found in component %2", serviceName, m_component->getName()));
			}

			ServiceBinding* binding = service->getBinding();
			if (binding == nullptr) {
				throw std::logic_error(fmt_str("Service %1 has no binding", service->getType()->getName()));
			}
			ServiceWrapper* wrapper = binding->getServiceWrapper();
			if (wrapper == nullptr) {
				throw std::logic_error(fmt_str("Service %1 has no wrapper", service->getType()->getName()));
			}

			Component* serviceComponent = service->getComponent();
			ComponentType* componentType = serviceComponent->getType();

			// Verifiy the component type
			if (componentType == nullptr) {
				throw std::logic_error(fmt_str("Component %1 has no implementation defined", serviceComponent->getName()));
			}
			auto impl = dynamic_cast<IoCImplementation*>(componentType);
			if (impl == nullptr) {
				throw std::logic_error(fmt_str("Component implementation is not of the expected type"));
			}

			string library = impl->getLibrary();
			string fullLibraryName = serviceComponent->getComposite()->getRoot() + "/" + library;
			Library lib(fullLibraryName);

			ServiceType* stype = service->getType();
			auto iface = dynamic_cast<IoCInterface*>(stype->getInterface());
			if (iface == nullptr) {
				throw std::logic_error(fmt_str("The interface implementation type of service %1 is not supported", serviceName));
			}

			const string& className = iface->getClass();
			Class clazz = Class::lookup(className);

			if (!clazz.isValid()) {
				throw std::logic_error(fmt_str("Cannot find a class called %1", className));
			}

			Proxy p = IoCServiceProxy::createProxy(clazz, wrapper);
			VariantValue ref = p.reference(clazz);

			// yes, we leak proxies for now
			m_proxies.emplace_back(std::move(p), clazz, lib);
			return std::move(ref);
		}

		commonj::sdo::DataFactoryPtr CompositeContext::getDataFactory()
		{
			return m_composite->getDataFactory();
		}
	}
}
