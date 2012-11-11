#include "VariantToParameter.h"

using namespace tuscany::sca;

VariantValue convertToVariant(const Operation::Parameter& p) {
	return std::move(convertToVariant(p.getType(), p.getValue()));
}

VariantValue convertToVariant(const tuscany::sca::Operation::ParameterType type, void* value)
{
	//unsafe conversions :(
	switch (type) {
		case Operation::UNSET:
		case Operation::VOID_TYPE:
			return std::move(VariantValue());
		case Operation::BOOL:
			return std::move(VariantValue(*reinterpret_cast<bool*>(value)));
		case Operation::SHORT:
			return std::move(VariantValue(*reinterpret_cast<short*>(value)));
		case Operation::INT:
			return std::move(VariantValue(*reinterpret_cast<short*>(value)));
		case Operation::LONG:
			return std::move(VariantValue(*reinterpret_cast<long*>(value)));
		case Operation::USHORT:
			return std::move(VariantValue(*reinterpret_cast<unsigned short*>(value)));
		case Operation::UINT:
			return std::move(VariantValue(*reinterpret_cast<unsigned int*>(value)));
		case Operation::ULONG:
			return std::move(VariantValue(*reinterpret_cast<unsigned long*>(value)));
		case Operation::FLOAT:
			return std::move(VariantValue(*reinterpret_cast<float*>(value)));
		case Operation::DOUBLE:
			return std::move(VariantValue(*reinterpret_cast<double*>(value)));
		case Operation::LONGDOUBLE:
			return std::move(VariantValue(*reinterpret_cast<long double*>(value)));
		case Operation::CHARS:
			return std::move(VariantValue(reinterpret_cast<char*>(value)));
		case Operation::CHAR:
			return std::move(VariantValue(*reinterpret_cast<char*>(value)));
		case Operation::STRING: {
			VariantValue v;
			v.construct<std::string>(*reinterpret_cast<std::string*>(value));
			return std::move(v);
		}
		case Operation::DATAOBJECT: {
			// TODO: here we could do something smart like building a scruct using reflection info
			VariantValue v;
			v.construct<commonj::sdo::DataObjectPtr>(*reinterpret_cast<commonj::sdo::DataObjectPtr*>(value));
			return std::move(v);
		}
		default:
			throw std::logic_error("unknown parameter type");
	}
}

namespace {

template<class T>
void setKnownReturnType(Operation& op, const VariantValue& ret) {
	T value = ret.value<T>();
	if(op.getReturnValue() != nullptr) {
		*(T*)op.getReturnValue() = value;
	} else {
		op.setReturnValue(new T(value)); // horror. isso vaza!
	}
}

}

void setReturnValue(Operation& op, const VariantValue& ret) {

	if (ret.isValid()) {

		// oh god
		if (ret.isA<bool>()) {
			setKnownReturnType<bool>(op, ret);
		} else if (ret.isA<short>()) {
			setKnownReturnType<short>(op, ret);
		} else if (ret.isA<int>()) {
			setKnownReturnType<int>(op, ret);
		} else if (ret.isA<long>()) {
			setKnownReturnType<long>(op, ret);
		} else if (ret.isA<unsigned short>()) {
			setKnownReturnType<unsigned short>(op, ret);
		} else if (ret.isA<unsigned int>()) {
			setKnownReturnType<unsigned int>(op, ret);
		} else if (ret.isA<unsigned long>()) {
			setKnownReturnType<unsigned long>(op, ret);
		} else if (ret.isA<float>()) {
			setKnownReturnType<float>(op, ret);
		} else if (ret.isA<double>()) {
			setKnownReturnType<double>(op, ret);
		} else if (ret.isA<long double>()) {
			setKnownReturnType<long double>(op, ret);
		} else if (ret.isA<char*>()) {
			setKnownReturnType<char*>(op, ret);
		} else if (ret.isA<char>()) {
			setKnownReturnType<char>(op, ret);
		} else if (ret.isA<std::string>()) {
			setKnownReturnType<std::string>(op, ret);
		} else if (ret.isA<commonj::sdo::DataObjectPtr>()) {
			setKnownReturnType<commonj::sdo::DataObjectPtr>(op, ret);
		} else {
			throw std::logic_error("return value is not supported by the tuscany runtime");
		}

	}

}

namespace {

template<class T>
void addKnownType(size_t index, Operation& op, const VariantValue& ret) {
	T* value = ret.convertTo<T*>();
	op.addParameter(fmt_str("arg%1", index), value);
}

}

void addParameter(size_t index, Operation& op, const VariantValue& parm) {

	if (parm.isValid()) {

		// oh god
		if (parm.isA<bool>()) {
			addKnownType<bool>(index, op, parm);
		} else if (parm.isA<short>()) {
			addKnownType<short>(index, op, parm);
		} else if (parm.isA<int>()) {
			addKnownType<int>(index, op, parm);
		} else if (parm.isA<long>()) {
			addKnownType<long>(index, op, parm);
		} else if (parm.isA<unsigned short>()) {
			addKnownType<unsigned short>(index, op, parm);
		} else if (parm.isA<unsigned int>()) {
			addKnownType<unsigned int>(index, op, parm);
		} else if (parm.isA<unsigned long>()) {
			addKnownType<unsigned long>(index, op, parm);
		} else if (parm.isA<float>()) {
			addKnownType<float>(index, op, parm);
		} else if (parm.isA<double>()) {
			addKnownType<double>(index, op, parm);
		} else if (parm.isA<long double>()) {
			addKnownType<long double>(index, op, parm);
		} else if (parm.isA<char*>()) {
			addKnownType<char*>(index, op, parm);
		} else if (parm.isA<char>()) {
			addKnownType<char>(index, op, parm);
		} else if (parm.isA<std::string>()) {
			addKnownType<std::string>(index, op, parm);
		} else if (parm.isA<commonj::sdo::DataObjectPtr>()) {
			addKnownType<commonj::sdo::DataObjectPtr>(index, op, parm);
		} else {
			throw std::logic_error("parameter type is not supported by the tuscany runtime");
		}

	} else {
		throw std::logic_error("parameter variant is empty");
	}

}

using namespace commonj::sdo;

namespace {

	struct null_type{};

	template<class VType>
	static VariantValue extract(const commonj::sdo::Property& property, commonj::sdo::DataObjectPtr& dataObject, VType (DataObject::*getter)(const Property&)) {
		return (dataObject->*getter)(property);
	}
}


VariantValue convertToVariant(const std::string& propName, commonj::sdo::DataObjectPtr& dataObject)
{
	if (!dataObject->hasProperty(propName.c_str())) {
		return VariantValue();
	}
	return convertToVariant(dataObject->getProperty(propName.c_str()), dataObject);
}


VariantValue convertToVariant(const commonj::sdo::Property& property, commonj::sdo::DataObjectPtr& dataObject)
{
	switch(property.getTypeEnum()) {
		case Type::BigDecimalType:
			return extract(property, dataObject, &DataObject::getDouble); // <- is this correct?
		case Type::BigIntegerType:
			return extract(property, dataObject, &DataObject::getInteger); // <- is this correct?
		case Type::BooleanType:
			return extract(property, dataObject, &DataObject::getBoolean);
		case Type::ByteType:
			return extract(property, dataObject, &DataObject::getByte);
		case Type::BytesType:
			throw std::logic_error("conversion of SDO property to byte is not implemented");
		case Type::CharacterType:
			return extract(property, dataObject, &DataObject::getCharacter);
		case Type::DateType:
			return extract(property, dataObject, &DataObject::getDate);
		case Type::DoubleType:
			return extract(property, dataObject, &DataObject::getDouble);
		case Type::FloatType:
			return extract(property, dataObject, &DataObject::getFloat);
		case Type::IntegerType:
			return extract(property, dataObject, &DataObject::getInteger);
		case Type::LongType:
			return extract(property, dataObject, &DataObject::getLong);
		case Type::ShortType:
			return extract(property, dataObject, &DataObject::getShort);
		case Type::StringType:
			return extract(property, dataObject, &DataObject::getCString);
		case Type::DataObjectType:
			return extract(property, dataObject, &DataObject::getDataObject);
		case Type::ChangeSummaryType:
			return extract(property, dataObject, &DataObject::getChangeSummary);
		case Type::TextType:
			return extract(property, dataObject, &DataObject::getCString); // <- is this correct?
		default:
			return VariantValue();
	}
}
