#ifndef COMPOSITE_CONTEXT_H
#define COMPOSITE_CONTEXT_H

#include <tuscany/sca/model/Component.h>
#include <tuscany/sca/model/Composite.h>
#include <tuscany/sca/util/Library.h>
#include <SelfPortrait/reflection.h>
#include <list>

// The osoa::sca::CompositeContext is hardcoded internally to use tuscany::sca::cpp::CompositeContextImpl
namespace puc {

	namespace sca {

		class CompositeContext {
		public:
			static CompositeContext getCurrent();

			virtual VariantValue locateService(const std::string& serviceName);

			virtual commonj::sdo::DataFactoryPtr getDataFactory();

			CompositeContext(const CompositeContext&);

			CompositeContext& operator=(CompositeContext);

			void swap(CompositeContext&);

			CompositeContext(CompositeContext* implementation);

		private:

			struct ProxyData {
				Proxy proxy;
				Class clazz;
				tuscany::sca::util::Library lib;
				ProxyData(Proxy&& p, Class c, tuscany::sca::util::Library& l) : proxy(std::move(p)), clazz(c), lib(l) {}
			};

			CompositeContext(tuscany::sca::model::Component* component);

			tuscany::sca::model::Composite* m_composite;

			tuscany::sca::model::Component* m_component;

			std::list<ProxyData> m_proxies;
		};

	}

}

namespace std {
	inline void swap(puc::sca::CompositeContext& c1, puc::sca::CompositeContext& c2) {
		c1.swap(c2);
	}
}

#endif /* COMPOSITE_CONTEXT_H */
