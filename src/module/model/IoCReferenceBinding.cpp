#include "IoCReferenceBinding.h"
#include "IoCServiceProxy.h"
#include <tuscany/sca/model/ServiceType.h>
#include <tuscany/sca/model/ComponentType.h>

#include "model/IoCInterface.h"

using namespace tuscany::sca::model;
using namespace tuscany::sca;

IoCReferenceBinding::IoCReferenceBinding(tuscany::sca::model::Reference* reference)
	: ReferenceBinding(reference, "")
	, m_serviceProxy(nullptr)
{

}


void IoCReferenceBinding::configure(tuscany::sca::model::ServiceBinding* serviceBinding)
{
	m_serviceProxy = new IoCServiceProxy(getReference(), serviceBinding);
}
