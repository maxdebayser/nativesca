#ifndef IOC_INTERFACE_H
#define IOC_INTERFACE_H

#include <tuscany/sca/export.h>
#include <tuscany/sca/model/Interface.h>

class IoCInterface: public tuscany::sca::model::Interface {
public:
	IoCInterface(const std::string& className, bool remotable, bool conversational);

	const std::string& getClass() { return m_className; }

	virtual const std::string& getInterfaceTypeQName() override;

private:
	const std::string m_className;
};

#endif /* IOC_INTERFACE_H */
