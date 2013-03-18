#ifndef IOC_REFERENCE_BINDING_H
#define IOC_REFERENCE_BINDING_H

#include <tuscany/sca/export.h>
#include <tuscany/sca/model/ReferenceBinding.h>
#include <tuscany/sca/model/ServiceBinding.h>
#include <tuscany/sca/core/ServiceProxy.h>

// Um binding na lingua do SCA representa o mecanismo de ligacao
// entre componentes

class IoCReferenceBinding : public tuscany::sca::model::ReferenceBinding {
public:


	IoCReferenceBinding(tuscany::sca::model::Reference* reference);

	virtual std::string getType() override { return "http://www.osoa.org/xmlns/sca/1.0#IoCImplementationBinding"; };

	virtual tuscany::sca::ServiceProxy* getServiceProxy() override { return m_serviceProxy; }

	virtual void configure(tuscany::sca::model::ServiceBinding* serviceBinding) override;

private:

	// TODO: preciso colocar um smart pointer aqui
	tuscany::sca::ServiceProxy* m_serviceProxy;
};

#endif /* IOC_REFERENCE_BINDING_H */
