/*
 * reader.cpp
 *
 *  Created on: Sep 27, 2016
 *      Author: andrei
 */

#include "reader.h"

std::shared_ptr<PrimitiveReader> PrimitiveReader::Make(
		std::shared_ptr<Column> column)
{
	switch (column->node->getType())
	{
	case Type::UINT8:
		return std::make_shared<TypedPrimitiveReader<UInt8Type>>(column);
	case Type::INT8:
		return std::make_shared<TypedPrimitiveReader<Int8Type>>(column);
	case Type::UINT16:
		return std::make_shared<TypedPrimitiveReader<UInt16Type>>(column);
	case Type::INT16:
		return std::make_shared<TypedPrimitiveReader<Int16Type>>(column);
	case Type::UINT32:
		return std::make_shared<TypedPrimitiveReader<UInt32Type>>(column);
	case Type::INT32:
		return std::make_shared<TypedPrimitiveReader<Int32Type>>(column);
	case Type::UINT64:
		return std::make_shared<TypedPrimitiveReader<UInt64Type>>(column);
	case Type::INT64:
		return std::make_shared<TypedPrimitiveReader<Int64Type>>(column);
	case Type::FLOAT:
		return std::make_shared<TypedPrimitiveReader<FloatType>>(column);
	case Type::DOUBLE:
		return std::make_shared<TypedPrimitiveReader<DoubleType>>(column);
	case Type::BYTE_ARRAY:
		return std::make_shared<TypedPrimitiveReader<ByteArrayType>>(column);
	default:
		return nullptr;
	}
}
