#ifndef IOC_SERVICE_BINDING_H
#define IOC_SERVICE_BINDING_H

#include <tuscany/sca/export.h>
#include <tuscany/sca/model/ServiceBinding.h>
#include <tuscany/sca/core/ServiceWrapper.h>

#include "IoCServiceWrapper.h"

class IoCServiceBinding: public tuscany::sca::model::ServiceBinding {
public:

	IoCServiceBinding(tuscany::sca::model::Service* service);

	virtual tuscany::sca::ServiceWrapper* getServiceWrapper() override;

	virtual std::string getType() { return "http://www.osoa.org/xmlns/sca/1.0#IoCImplementationBinding"; };

private:
	IoCServiceWrapper m_wrapper;

};


#endif /* IOC_SERVICE_BINDING_H */
