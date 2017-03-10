/*
 * printer.cpp
 *
 *  Created on: Sep 27, 2016
 *      Author: andrei
 */

#include "printer.h"

std::shared_ptr<Printer> Printer::Make(std::shared_ptr<Node> node)
{
	switch (node->getType())
	{
	case Type::INT64:
		return std::make_shared<TypedPrinter<Int64Type>>(node);
	case Type::UINT64:
		return std::make_shared<TypedPrinter<UInt64Type>>(node);
	case Type::INT32:
		return std::make_shared<TypedPrinter<Int32Type>>(node);
	case Type::UINT32:
		return std::make_shared<TypedPrinter<UInt32Type>>(node);
	case Type::INT16:
		return std::make_shared<TypedPrinter<Int16Type>>(node);
	case Type::UINT16:
		return std::make_shared<TypedPrinter<UInt16Type>>(node);
	case Type::UINT8:
		return std::make_shared<TypedPrinter<Int8Type>>(node);
	case Type::INT8:
		return std::make_shared<TypedPrinter<UInt8Type>>(node);
	case Type::FLOAT:
		return std::make_shared<TypedPrinter<FloatType>>(node);
	case Type::DOUBLE:
		return std::make_shared<TypedPrinter<DoubleType>>(node);
	case Type::BYTE_ARRAY:
		return std::make_shared<TypedPrinter<ByteArrayType>>(node);
	default:
		return nullptr;
	}
}
