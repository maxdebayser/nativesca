#ifndef IOC_SERVICE_WRAPPER_H
#define IOC_SERVICE_WRAPPER_H

// Esser cara vai usar reflexao para instanciar e chamar objetos

#include <tuscany/sca/core/ServiceWrapper.h>
#include <tuscany/sca/model/Interface.h>
#include <tuscany/sca/util/Library.h>
#include <SelfPortrait/reflection.h>

#include "model/IoCImplementation.h"


class IoCServiceWrapper: public tuscany::sca::ServiceWrapper {
public:
	IoCServiceWrapper(tuscany::sca::model::Service* service);

	virtual ~IoCServiceWrapper();

	virtual void invoke(tuscany::sca::Operation& operation) override;

private:
	tuscany::sca::model::Component* m_component;

	tuscany::sca::model::Interface* m_interface;

	IoCImplementation* m_implementation;

	Class m_class;

	VariantValue m_instance;

	// TODO: unload?
	tuscany::sca::util::Library m_wrapperLibrary;
};


#endif /* IOC_SERVICE_WRAPPER_H */

