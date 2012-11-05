#include "IoCServiceBinding.h"


using namespace tuscany::sca;
using namespace tuscany::sca::model;

IoCServiceBinding::IoCServiceBinding(Service* service)
	: ServiceBinding(service, "")
	, m_wrapper(service)
{

}

ServiceWrapper* IoCServiceBinding::getServiceWrapper()
{
	return &m_wrapper;
}
