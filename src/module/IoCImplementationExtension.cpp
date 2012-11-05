#include "IoCImplementationExtension.h"
#include "model/IoCImplementation.h"

using namespace std;

IoCImplementationExtension::IoCImplementationExtension()
{}

IoCImplementationExtension::~IoCImplementationExtension()
{}


const std::string& IoCImplementationExtension::getExtensionName()
{
	static string name("ioc");
	return name;
}

const std::string& IoCImplementationExtension::getExtensionTypeQName()
{
	static string qname("http://www.osoa.org/xmlns/sca/1.0#IoCImplementation");
	return qname;
}

tuscany::sca::model::ComponentType* IoCImplementationExtension::getImplementation(
		tuscany::sca::model::Composite* composite,
		commonj::sdo::DataObjectPtr scdlImplementation)
{
	const string componentName = scdlImplementation->getContainer()->getCString("name");
	const string library = scdlImplementation->getCString("library");
	const string className = scdlImplementation->getCString("class");
	//string header = scdlImplementation->getCString("header");

	const string scopeName = scdlImplementation->getCString("scope");
	const IoCImplementation::Scope scope = (scopeName == "composite" ? IoCImplementation::COMPOSITE : IoCImplementation::STATELESS);

	return new IoCImplementation(composite, componentName, library, className, scope);
}
