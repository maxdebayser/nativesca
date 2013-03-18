#include "IoCImplementation.h"

#include "model/IoCReferenceBinding.h"
#include "model/IoCServiceBinding.h"
#include <tuscany/sca/model/ServiceType.h>

using namespace tuscany::sca::model;
using namespace std;

IoCImplementation::IoCImplementation(Composite* composite, commonj::sdo::DataObjectPtr implDataObject)
	: ComponentType(composite, implDataObject->getContainer()->getCString("name"))
	, m_library(implDataObject->getCString("library"))
	, m_MDLibrary(implDataObject->getCString("metadata"))
	, m_className(implDataObject->getCString("class"))
	, m_scope((std::string(implDataObject->getCString("scope")) == "composite" ? IoCImplementation::COMPOSITE : IoCImplementation::STATELESS))
	, m_implDataObject(implDataObject)
{
	// TODO: Tenho que mexer aqui e no IoCServiceWrapper para impedir que um componente
	// com scope composite que implementa dois servicos crie duas instancias em vez de uma
	// É fácil, basta colocar o Variant de instancia unica aqui
}

IoCImplementation::~IoCImplementation()
{
}

void IoCImplementation::initializeComponent(Component* component)
{
	ComponentType::initializeComponent(component);

	/*
	DataObjectList& serviceTypes = componentTypeDO->getList("service");
	for (unsigned int i=0; i<serviceTypes.size(); i++)
	{
		Interface* iface = getInterface(composite, serviceTypes[i]);
		ServiceType* serviceType =  new ServiceType(
			componentType, serviceTypes[i]->getCString("name"), iface, NULL);
		componentType->addServiceType(serviceType);
	}*/

	// Create CPP IoC bindings for all the services
	const Component::SERVICE_MAP& services = component->getServices();
	Component::SERVICE_MAP::const_iterator iter = services.begin();
	std::cout << "this component = " << component->getName() << std::endl;
	for (unsigned int i=0; i< services.size(); i++)
	{
		Service *service = iter->second;
		std::cout << "service " << service->getType()->getName() << std::endl;
		std::cout << "service component = " << service->getComponent()->getName() << std::endl;

		auto binding = new IoCServiceBinding(service);
		service->setBinding(binding);
		iter++;
	}

	// Create CPP bindings for all the references
	const Component::REFERENCE_MAP& references = component->getReferences();
	Component::REFERENCE_MAP::const_iterator refiter = references.begin();
	for (unsigned int ri=0; ri< references.size(); ri++)
	{
		Reference *reference = refiter->second;
		auto binding = new IoCReferenceBinding(reference);
		reference->setBinding(binding);
		refiter++;
	}
}
