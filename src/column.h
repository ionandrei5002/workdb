/*
 * column.h
 *
 *  Created on: Sep 27, 2016
 *      Author: andrei
 */

#ifndef SRC_COLUMN_H_
#define SRC_COLUMN_H_

#include "definitions.h"

#include "types.h"
#include "primitive.h"
#include "schema.h"

#include <vector>
#include <unordered_map>
#include <netinet/in.h>
#include "allocator/free_list_allocator.h"
#include "buffer.h"

class Column: public Base
{
public:
    Buffer buffer;
    uint32_t entries = 0;
    std::shared_ptr<Node> node;
    std::shared_ptr<PrimitiveValue> primitive;

    Column(std::shared_ptr<Node> node) :
        node(node)
    {
        Debug();
    }
    virtual ~Column()
    {
    }
    void Debug()
    {
        std::shared_ptr<Node> it = node->typeGet();

        std::cout << it->getName() << " " << it->toString() << std::endl;
    }
    virtual std::string toString() = 0;
    virtual std::string getTypeName() = 0;

    static std::shared_ptr<Column> Make(std::shared_ptr<Node> node);
    static std::shared_ptr<PrimitiveValue> MakePrimitive(std::shared_ptr<Node> node);
    virtual std::shared_ptr<PrimitiveValue> getThisPrimitive() = 0;
    virtual uint32_t Append(const uint8_t* data) = 0;
};

template<typename T>
class TypedColumn: public Column
{
public:
    TypedColumn(std::shared_ptr<Node> node) :
        Column(node)
    {
        std::shared_ptr<Node> it = node->typeGet();

        primitive = Column::MakePrimitive(it);
    }

    std::string toString() override
    {
        return node->getName() + " " + node->toString();
    }

    std::string getTypeName() override
    {
        return type_traits<T::type_num>::name;
    }
    std::shared_ptr<PrimitiveValue> getThisPrimitive() override {
        //return std::make_shared<TypedPrimitiveValue<T>>();
        return std::allocate_shared<TypedPrimitiveValue<T>, free_list_allocator<uint8_t>>(free_list_alloca, TypedPrimitiveValue<T>());
    }
    inline uint32_t Append(const uint8_t* data) override
    {
        uint32_t bytes = type_traits<T::type_num>::value_byte_size;

        Primitive values = primitive->read((uint8_t*) data, bytes);
        buffer.AppendPrimitive(values);

        entries++;

        return bytes;
    }
};

template<>
class TypedColumn<ByteArrayType> : public Column
{
public:
    TypedColumn(std::shared_ptr<Node> node) :
        Column(node)
    {
        std::shared_ptr<Node> it = node->typeGet();

        primitive = Column::MakePrimitive(it);
    }

    std::string toString() override
    {
        return node->getName() + " " + node->toString();
    }

    std::string getTypeName() override
    {
        return type_traits<ByteArrayType::type_num>::name;
    }
    std::shared_ptr<PrimitiveValue> getThisPrimitive() override {
        //return std::make_shared<TypedPrimitiveValue<ByteArrayType>>();
        return std::allocate_shared<TypedPrimitiveValue<ByteArrayType>, free_list_allocator<uint8_t>>(free_list_alloca, TypedPrimitiveValue<ByteArrayType>());
    }
    inline uint32_t Append(const uint8_t* data) override
    {
        uint32_t bytes = *(uint32_t*) data;
        bytes = ntohl(bytes);

        Primitive values = primitive->read((uint8_t*) data, bytes);
        buffer.AppendPrimitive(values);

        entries++;

        return (bytes + sizeof(uint32_t));
    }
};

#endif /* SRC_COLUMN_H_ */
