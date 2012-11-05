#ifndef IOC_SERVICE_PROXY_H
#define IOC_SERVICE_PROXY_H

// esse cara vai usar proxy dinamico

#include <tuscany/sca/core/ServiceProxy.h>
#include <tuscany/sca/model/ReferenceType.h>
#include <SelfPortrait/reflection.h>
#include <tuscany/sca/util/Library.h>
#include <tuscany/sca/model/Component.h>
#include <tuscany/sca/core/ServiceWrapper.h>
#include <string>
#include <vector>
#include <list>

class IoCServiceProxy: public tuscany::sca::ServiceProxy {
public:

	IoCServiceProxy(tuscany::sca::model::Reference* sourceReference, tuscany::sca::model::ServiceBinding* serviceBinding);

	std::list<VariantValue> proxies() const;

	void addProxy(tuscany::sca::model::Reference* sourceReference, tuscany::sca::model::ServiceBinding* serviceBinding);

	static Proxy createProxy(Class clazz, tuscany::sca::ServiceWrapper* serviceWrapper);

private:

	// Para fazer ligacao direta o proxy data tera que mudar, mas pelo menos a API do IoCServiceProxy esta generica
	struct ProxyData {
		Proxy proxy;
		Class clazz;
		ProxyData(Proxy&& p, Class c) : proxy(std::move(p)), clazz(c) {}
	};

	static void addProxy(tuscany::sca::model::Reference* sourceReference, tuscany::sca::model::ServiceBinding* serviceBinding, std::vector<ProxyData>& proxies, std::vector<tuscany::sca::util::Library>& libs);

	std::vector<tuscany::sca::util::Library> m_libraries;

	std::vector<ProxyData> m_proxies;
};


#endif /* IOC_SERVICE_PROXY_H */
