/*
 * printer.h
 *
 *  Created on: Sep 27, 2016
 *      Author: andrei
 */

#ifndef SRC_PRINTER_H_
#define SRC_PRINTER_H_

#include "base.h"

#include "types.h"
#include "primitive.h"
#include "schema.h"

class Printer: public Base
{
protected:
	std::shared_ptr<Node> node;
public:

	Printer(std::shared_ptr<Node> node) :
			node(node)
	{
	}
	virtual ~Printer()
	{
	}
	static std::shared_ptr<Printer> Make(std::shared_ptr<Node> node);
	virtual void Print(std::ostream& out, std::shared_ptr<Primitive> value) = 0;
	virtual void getTyped(Primitive& value, void* val) = 0;
	virtual std::string getName() = 0;
};

template<typename T>
class TypedPrinter: public Printer
{
private:
	typedef typename T::c_type value_type;
public:

	TypedPrinter(std::shared_ptr<Node> node) :
			Printer(node)
	{
	}

	void Print(std::ostream& out, std::shared_ptr<Primitive> value) override
	{
		if (value->values != nullptr)
			out << *((value_type*) value->values) << "\t";
		else
			out << "null" << "\t";

	}

	void getTyped(Primitive& value, void* val) override
	{
		memcpy(val, (void*) value.values, value.bytes);
	}

	std::string getName() override
	{
		return node->getName();
	}
};

template<>
class TypedPrinter<ByteArrayType> : public Printer
{
private:
	typedef ByteArrayType::c_type value_type;
public:

	TypedPrinter(std::shared_ptr<Node> node) :
			Printer(node)
	{
	}

	void Print(std::ostream& out, std::shared_ptr<Primitive> value) override
	{
		if (value->values != nullptr)
			out << value_type(value->values, value->bytes) << "\t";
		else
			out << "null" << "\t";
	}

	void getTyped(Primitive& value, void* val) override
	{
		val = (void*) new value_type(value.values, value.bytes);
	}

	std::string getName() override
	{
		return node->getName();
	}
};

#endif /* SRC_PRINTER_H_ */
