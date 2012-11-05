#include "ComponentContext.h"
#include <stdexcept>
#include "IoCServiceProxy.h"
#include "model/IoCInterface.h"

#include <tuscany/sca/model/ReferenceBinding.h>
#include <tuscany/sca/model/Composite.h>
#include <tuscany/sca/core/SCARuntime.h>

#include <SelfPortrait/str_conversion.h>
#include <SelfPortrait/reflection.h>

using namespace std;
using namespace tuscany::sca;
using namespace tuscany::sca::model;

namespace puc {

	namespace sca {

		ComponentContext ComponentContext::getCurrent()
		{
			Component* component = SCARuntime::getCurrentRuntime()->getCurrentComponent();
			if (component == nullptr) {
				throw std::logic_error("No current component");
			}
			return {component};
		}

		ComponentContext::ComponentContext(Component* comp)
			: m_component(comp)
		{}

		ComponentContext::ComponentContext(const ComponentContext& that)
			: ComponentContext(that.m_component) {}

		ComponentContext& ComponentContext::operator=(ComponentContext that)
		{
			swap(that);
			return *this;
		}

		void ComponentContext::swap(ComponentContext& that)
		{
			std::swap(this->m_component, that.m_component);
		}

		std::list<VariantValue> ComponentContext::getServices(const std::string& referenceName) const
		{
			Reference* reference = m_component->findReference(referenceName);
			if (reference == nullptr)
			{
				throw std::logic_error(fmt_str("Component %1 has no reference called %2", m_component->getName(), referenceName));
			}

			auto genericProxy = reference->getBinding()->getServiceProxy();
			if (genericProxy == nullptr)
			{
				throw std::logic_error(fmt_str("Reference %1 of component %2 is not wired", referenceName, m_component->getName()));
			}

			auto serviceProxy = dynamic_cast<IoCServiceProxy*>(genericProxy);
			if (serviceProxy == nullptr)
			{
				throw std::logic_error(fmt_str("The service proxy implementation of reference %1 of component %2 is not supported", referenceName, m_component->getName()));
			}

			return std::move(serviceProxy->proxies());
		}

		VariantValue ComponentContext::getService(const std::string& referenceName) const
		{
			Reference* reference = m_component->findReference(referenceName);
			if (reference == nullptr) {
				throw std::logic_error(fmt_str("Component %1 has no reference called %2", m_component->getName(), referenceName));
			}

			switch (reference->getType()->getMultiplicity())
			{
				case ReferenceType::ZERO_MANY:
				case ReferenceType::ONE_MANY: {
					throw std::logic_error(fmt_str("getService() called for reference %1 of component %2 which has a multiplicity > 1", referenceName, m_component->getName()));
				}
				default: break;
			}

			std::list<VariantValue> services = getServices(referenceName);
			if (services.empty()) {
				throw std::logic_error(fmt_str("reference %1 of component %2 is wired but has no proxies. this is an internal error", referenceName, m_component->getName()));
			}
			return std::move(services.front());
		}

		commonj::sdo::DataObjectPtr ComponentContext::getProperties() const
		{
			return m_component->getProperties();
		}

		commonj::sdo::DataFactoryPtr ComponentContext::getDataFactory() const
		{
			return m_component->getComposite()->getDataFactory();
		}
	}

}
