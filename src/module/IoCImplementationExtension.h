#ifndef IOC_IMPLEMENTATION_EXTENSION_H
#define IOC_IMPLEMENTATION_EXTENSION_H

#include <tuscany/sca/extension/ImplementationExtension.h>
#include <string.h>

class IoCImplementationExtension : public tuscany::sca::ImplementationExtension {
public:

	IoCImplementationExtension();
	~IoCImplementationExtension();

	virtual const std::string& getExtensionName() override;

	virtual const std::string& getExtensionTypeQName() override;

	virtual tuscany::sca::model::ComponentType* getImplementation(
		tuscany::sca::model::Composite* composite,
		commonj::sdo::DataObjectPtr scdlImplementation) override;

};

#endif /* IOC_IMPLEMENTATION_EXTENSION_H */
