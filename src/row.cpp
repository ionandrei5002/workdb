/*
 * row.cpp
 *
 *  Created on: Sep 27, 2016
 *      Author: andrei
 */

#include "row.h"

std::shared_ptr<PrimitiveValue> Row::Make(std::shared_ptr<Node> node)
{
	switch (node->getType())
	{
	case Type::INT64:
		return std::make_shared<TypedPrimitiveValue<Int64Type>>();
	case Type::UINT64:
		return std::make_shared<TypedPrimitiveValue<UInt64Type>>();
	case Type::INT32:
		return std::make_shared<TypedPrimitiveValue<Int32Type>>();
	case Type::UINT32:
		return std::make_shared<TypedPrimitiveValue<UInt32Type>>();
	case Type::INT16:
		return std::make_shared<TypedPrimitiveValue<Int16Type>>();
	case Type::UINT16:
		return std::make_shared<TypedPrimitiveValue<UInt16Type>>();
	case Type::UINT8:
		return std::make_shared<TypedPrimitiveValue<UInt8Type>>();
	case Type::INT8:
		return std::make_shared<TypedPrimitiveValue<Int8Type>>();
	case Type::FLOAT:
		return std::make_shared<TypedPrimitiveValue<FloatType>>();
	case Type::DOUBLE:
		return std::make_shared<TypedPrimitiveValue<DoubleType>>();
	case Type::BYTE_ARRAY:
		return std::make_shared<TypedPrimitiveValue<ByteArrayType>>();
	default:
		return nullptr;
	}
}
