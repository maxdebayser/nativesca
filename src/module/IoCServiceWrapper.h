#ifndef IOC_SERVICE_WRAPPER_H
#define IOC_SERVICE_WRAPPER_H

// Esser cara vai usar reflexao para instanciar e chamar objetos

#include <tuscany/sca/core/ServiceWrapper.h>
#include <tuscany/sca/model/Interface.h>
#include <tuscany/sca/util/Library.h>
#include <SelfPortrait/reflection.h>

#include "model/IoCImplementation.h"
#include <unordered_map>
#include <string>
#include <list>

class IoCServiceWrapper: public tuscany::sca::ServiceWrapper {
public:
	IoCServiceWrapper(tuscany::sca::model::Service* service);

	virtual ~IoCServiceWrapper();

	virtual void invoke(tuscany::sca::Operation& operation) override;

private:

	void setupClassInfo();

	void configureComponent(VariantValue& object, std::list<VariantValue>& keepAlive);

	tuscany::sca::model::Component* m_component;

	tuscany::sca::model::Interface* m_interface;

	IoCImplementation* m_implementation;

	Class m_class;

	VariantValue m_instance;

	// TODO: unload?
	tuscany::sca::util::Library m_wrapperLibrary;
	tuscany::sca::util::Library m_MDLibrary;

	typedef std::unordered_map<std::string, std::list<Method> > MethodMap;
	typedef std::unordered_map<std::string, Attribute> AttributeMap;

	MethodMap m_setters;
	AttributeMap m_attrs;
	std::list<VariantValue> m_keepAlive;
};


#endif /* IOC_SERVICE_WRAPPER_H */

