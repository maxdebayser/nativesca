#include "IoCImplementation.h"

#include "model/IoCReferenceBinding.h"
#include "model/IoCServiceBinding.h"

using namespace tuscany::sca::model;
using namespace std;

IoCImplementation::IoCImplementation(Composite* composite, const std::string& componentName, const string& library, const string& className, Scope scope)
	: ComponentType(composite, componentName)
	, m_library(library)
	, m_className(className)
	, m_scope(scope)
{
}

IoCImplementation::~IoCImplementation()
{
}

void IoCImplementation::initializeComponent(Component* component)
{
	ComponentType::initializeComponent(component);

	// Create CPP IoC bindings for all the services
	const Component::SERVICE_MAP& services = component->getServices();
	Component::SERVICE_MAP::const_iterator iter = services.begin();
	for (unsigned int i=0; i< services.size(); i++)
	{
		Service *service = iter->second;
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
