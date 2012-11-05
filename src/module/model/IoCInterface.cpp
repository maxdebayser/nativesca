#include "IoCInterface.h"

#include <string>

IoCInterface::IoCInterface(const std::string& className, bool remotable, bool conversational)
	: Interface(remotable, conversational)
	, m_className(className)
{

}

const std::string& IoCInterface::getInterfaceTypeQName()
{
	static std::string typeQName("http://www.osoa.org/xmlns/sca/1.0#IoCInterface");
	return typeQName;
}
