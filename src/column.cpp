/*
 * column.cpp
 *
 *  Created on: Sep 27, 2016
 *      Author: andrei
 */

#include "column.h"

std::shared_ptr<Column> Column::Make(std::shared_ptr<Node> node)
{
	switch (node->getType())
	{
	case Type::UINT8:
		return std::make_shared<TypedColumn<UInt8Type>>(node);
	case Type::INT8:
		return std::make_shared<TypedColumn<Int8Type>>(node);
	case Type::UINT16:
		return std::make_shared<TypedColumn<UInt16Type>>(node);
	case Type::INT16:
		return std::make_shared<TypedColumn<Int16Type>>(node);
	case Type::UINT32:
		return std::make_shared<TypedColumn<UInt32Type>>(node);
	case Type::INT32:
		return std::make_shared<TypedColumn<Int32Type>>(node);
	case Type::UINT64:
		return std::make_shared<TypedColumn<UInt64Type>>(node);
	case Type::INT64:
		return std::make_shared<TypedColumn<Int64Type>>(node);
	case Type::FLOAT:
		return std::make_shared<TypedColumn<FloatType>>(node);
	case Type::DOUBLE:
		return std::make_shared<TypedColumn<DoubleType>>(node);
	case Type::BYTE_ARRAY:
		return std::make_shared<TypedColumn<ByteArrayType>>(node);
	default:
		return nullptr;
	}
}

std::shared_ptr<PrimitiveValue> Column::MakePrimitive(
		std::shared_ptr<Node> node)
{
	switch (node->getType())
	{
	case Type::UINT8:
		return std::make_shared<TypedPrimitiveValue<UInt8Type>>();
	case Type::INT8:
		return std::make_shared<TypedPrimitiveValue<Int8Type>>();
	case Type::UINT16:
		return std::make_shared<TypedPrimitiveValue<UInt16Type>>();
	case Type::INT16:
		return std::make_shared<TypedPrimitiveValue<Int16Type>>();
	case Type::UINT32:
		return std::make_shared<TypedPrimitiveValue<UInt32Type>>();
	case Type::INT32:
		return std::make_shared<TypedPrimitiveValue<Int32Type>>();
	case Type::UINT64:
		return std::make_shared<TypedPrimitiveValue<UInt64Type>>();
	case Type::INT64:
		return std::make_shared<TypedPrimitiveValue<Int64Type>>();
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
