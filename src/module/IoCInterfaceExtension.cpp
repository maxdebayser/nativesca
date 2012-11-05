#include "IoCInterfaceExtension.h"
#include "model/IoCInterface.h"

using namespace tuscany::sca;
using namespace tuscany::sca::model;

IoCInterfaceExtension::IoCInterfaceExtension()
	: InterfaceExtension()
{}

const std::string& IoCInterfaceExtension::getExtensionName()
{
	static std::string name("ioc");
	return name;
}

const std::string& IoCInterfaceExtension::getExtensionTypeQName()
{
	static std::string name("http://www.osoa.org/xmlns/sca/1.0#IoCInterface");
	return name;
}


Interface* IoCInterfaceExtension::getInterface(
	tuscany::sca::model::Composite *composite,
	commonj::sdo::DataObjectPtr scdlInterface)
{
	std::string className = scdlInterface->getCString("class");
	bool remotable = scdlInterface->getBoolean("remotable");

	return new IoCInterface(className, remotable, false);
}
