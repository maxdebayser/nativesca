#ifndef VARIANT_TO_PARAMETER_H
#define VARIANT_TO_PARAMETER_H

#include <SelfPortrait/reflection.h>
#include <tuscany/sca/core/Operation.h>
#include <commonj/sdo/DataObject.h>
#include <commonj/sdo/RefCountingPointer.h>
#include <string>

VariantValue convertToVariant(const tuscany::sca::Operation::Parameter& p);

VariantValue convertToVariant(const tuscany::sca::Operation::ParameterType type, void* value);

void setReturnValue(tuscany::sca::Operation& op, const VariantValue& ret);

void addParameter(size_t index, tuscany::sca::Operation& op, const VariantValue& parm);

VariantValue convertToVariant(const std::string& propName, commonj::sdo::DataObjectPtr& datatObject);

VariantValue convertToVariant(const commonj::sdo::Property& property, commonj::sdo::DataObjectPtr& datatObject);

#endif /* VARIANT_TO_PARAMETER_H */
