#ifndef VARIANT_TO_PARAMETER_H
#define VARIANT_TO_PARAMETER_H

#include <SelfPortrait/reflection.h>
#include <tuscany/sca/core/Operation.h>


VariantValue convertToVariant(const tuscany::sca::Operation::Parameter& p);

VariantValue convertToVariant(const tuscany::sca::Operation::ParameterType type, void* value);

void setReturnValue(tuscany::sca::Operation& op, const VariantValue& ret);

void addParameter(size_t index, tuscany::sca::Operation& op, const VariantValue& parm);

#endif /* VARIANT_TO_PARAMETER_H */
