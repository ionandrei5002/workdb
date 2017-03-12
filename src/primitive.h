/*
 * primitive.h
 *
 *  Created on: Sep 27, 2016
 *      Author: andrei
 */

#ifndef SRC_PRIMITIVE_H_
#define SRC_PRIMITIVE_H_

#include "base.h"

#include "types.h"
#include "schema.h"

#include <memory>
#include "allocator/free_list_allocator.h"

struct Primitive: public Base
{
public:
    uint8_t* values;
    uint32_t bytes;

    Primitive() :
        values(nullptr), bytes(0)
    {
    }

    Primitive(uint8_t* values, uint32_t bytes) :
        values(values), bytes(bytes)
    {
    }

    Primitive(const Primitive& other)
    {
        this->bytes = other.bytes;
        this->values = other.values;
    }

    Primitive(Primitive&& other)
    {
        this->bytes = other.bytes;
        this->values = other.values;
        other.values = nullptr;
    }

    Primitive& operator=(const Primitive& other)
    {
        this->bytes = other.bytes;
        this->values = other.values;

        return *this;
    }

    Primitive& operator=(Primitive&& other)
    {
        this->bytes = other.bytes;
        this->values = other.values;
        other.values = nullptr;

        return *this;
    }

    bool operator==(const Primitive& other)
    {
        return (this->bytes == other.bytes
                && 0
                == memcmp((void*) this->values, (void*) other.values,
                          bytes));
    }

    bool operator!=(const Primitive& other)
    {
        return (this->bytes != other.bytes
                || 0
                != memcmp((void*) this->values, (void*) other.values,
                          bytes));
    }

    int32_t hash()
    {
        int32_t hash = 0;

        uint32_t i = 0;
        for (; i < this->bytes; i++)
        {
            hash += values[i];
        }

        return hash;
    }

    ~Primitive()
    {
        this->values = nullptr;
    }
};

class PrimitiveValue: public Base
{
public:
    Primitive* chunk;

    PrimitiveValue() :
        chunk(nullptr)
    {
    }

    PrimitiveValue(Primitive* chunk) :
        chunk(chunk)
    {
    }

    virtual ~PrimitiveValue()
    {
    }

    void Init(Primitive* chunk)
    {
        this->chunk = chunk;
    }

    Primitive& get()
    {
        return *this->chunk;
    }

    bool operator==(const PrimitiveValue& value) const
    {
        return (this->chunk->operator ==(*value.chunk));
    }

    inline static void reverse(void *start, int size)
    {
        char *istart = (char*) start, *iend = istart + size;
        std::reverse(istart, iend);
    }

    virtual int32_t typeSize() = 0;
    virtual Primitive read(uint8_t* data, uint32_t bytes) const = 0;
};

template<typename T>
class TypedPrimitiveValue: public PrimitiveValue
{
public:
    typedef typename T::c_type value_type;

    TypedPrimitiveValue() :
        PrimitiveValue()
    {
    }

    TypedPrimitiveValue(Primitive* chunk) :
        PrimitiveValue(chunk)
    {
    }

    int32_t typeSize() override
    {
        return sizeof(value_type);
    }

    Primitive read(uint8_t* data, uint32_t bytes) const override
    {
        //PrimitiveValue::reverse((void*) data, bytes);
        Primitive values(data, bytes);

        return values;
    }
};

template<>
class TypedPrimitiveValue<ByteArrayType> : public PrimitiveValue
{
public:
    typedef ByteArrayType::c_type value_type;

    TypedPrimitiveValue() :
        PrimitiveValue()
    {
    }

    TypedPrimitiveValue(Primitive* chunk) :
        PrimitiveValue(chunk)
    {
    }

    int32_t typeSize() override
    {
        return sizeof(value_type);
    }

    Primitive read(uint8_t* data, uint32_t bytes) const override
    {
        Primitive values(data, bytes);

        return values;
    }
};

namespace std
{

template<typename T> struct hash<TypedPrimitiveValue<T>>
{
    typedef TypedPrimitiveValue<T> argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& value) const
    {
        return value.chunk->hash();
    }
};
}

#endif /* SRC_PRIMITIVE_H_ */
