#ifndef IOC_INTERFACE_EXTENSION_H
#define IOC_INTERFACE_EXTENSION_H

#include <tuscany/sca/extension/InterfaceExtension.h>

class IoCInterfaceExtension: public tuscany::sca::InterfaceExtension {
public:
	IoCInterfaceExtension();

	virtual const std::string& getExtensionName() override;

	virtual const std::string& getExtensionTypeQName() override;

	virtual tuscany::sca::model::Interface* getInterface(
		tuscany::sca::model::Composite *composite,
		commonj::sdo::DataObjectPtr scdlInterface) override;
};


#endif /* IOC_INTERFACE_EXTENSION_H */
