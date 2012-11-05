#ifndef COMPONENT_CONTEXT_H
#define COMPONENT_CONTEXT_H

#include <tuscany/sca/model/Component.h>
#include <tuscany/sca/model/Service.h>
#include <tuscany/sca/core/ServiceWrapper.h>
#include <tuscany/sca/model/Reference.h>
#include <SelfPortrait/variant.h>


// The osoa::sca::ComponentContext is hardcoded internally to use tuscany::sca::cpp::ComponentContextImpl
namespace puc {

	namespace sca {

		class ComponentContext {
		public:
			static ComponentContext getCurrent();

			ComponentContext(const ComponentContext& that);

			ComponentContext& operator=(ComponentContext that);

			void swap(ComponentContext& that);

			virtual VariantValue getService(const std::string& referenceName) const;

			virtual std::list<VariantValue> getServices(const std::string& referenceName) const;

			virtual commonj::sdo::DataObjectPtr getProperties() const;

			virtual commonj::sdo::DataFactoryPtr getDataFactory() const;

			virtual const tuscany::sca::model::Component* getComponent() const { return m_component; }

		private:
			ComponentContext(tuscany::sca::model::Component* component);

			tuscany::sca::model::Component* m_component;

			void* getServiceProxy(
					tuscany::sca::model::Reference* serviceReference,
					tuscany::sca::model::Service* target);

			tuscany::sca::ServiceWrapper* getServiceWrapper(tuscany::sca::model::Service* target);
		};

	}

}

namespace std {

	inline void swap(puc::sca::ComponentContext& c1, puc::sca::ComponentContext& c2) {
		c1.swap(c2);
	}

}

#endif /* COMPONENT_CONTEXT_H */
