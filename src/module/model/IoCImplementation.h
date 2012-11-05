#ifndef IOC_IMPLEMENTATION_H
#define IOC_IMPLEMENTATION_H

#include "tuscany/sca/model/Component.h"
#include "tuscany/sca/model/Composite.h"

class IoCImplementation: public tuscany::sca::model::ComponentType {
public:

	enum Scope
	{
		COMPOSITE,
		STATELESS
	};

	IoCImplementation(tuscany::sca::model::Composite* composite, const std::string& componentName,
					  const std::string& library, const std::string& className, Scope scope);

	virtual ~IoCImplementation();

	virtual void initializeComponent(tuscany::sca::model::Component* component);

	const std::string& getLibrary() const { return m_library; }

	const std::string& getClass() const { return m_className; }

	Scope getScope() { return m_scope; }

private:

	const std::string m_library;

	const std::string m_className;

	const Scope m_scope;

};

#endif /* IOC_IMPLEMENTATION_H */
