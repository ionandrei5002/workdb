/*
 * Copyright 2017 <copyright holder> <email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <memory>
#include "base.h"
#include "primitive.h"

class Buffer : public Base
{
protected:
    uint8_t* buffer;
    uint32_t capacity;
    uint32_t size;
    uint32_t bytesLeft;
    void resize();
public:
    Buffer();
    ~Buffer();
    Buffer(const Buffer& other) = delete;
    Buffer(Buffer&& other) = delete;
    Buffer& operator=(const Buffer& other) = delete;
    Buffer& operator=(Buffer&& other) = delete;
    uint32_t getCapacity();
    uint32_t getSize();
    virtual void AppendPrimitive(const Primitive& data) = 0;
    virtual void getPrimitive(std::shared_ptr<PrimitiveValue> cell, uint32_t position) = 0;
};

template<typename T>
class TypedBuffer: public Buffer {
public:
    typedef typename T::c_type value_type;
    void AppendPrimitive(const Primitive& data) override
    {
        uint32_t bytes = data.bytes;
        uint8_t* values = data.values;

        while (this->bytesLeft < bytes) {
            this->resize();
        }

        uint8_t* cursor =  (uint8_t*)&this->buffer[this->size];
        memcpy(cursor, values, bytes);
        this->size += bytes;
        this->bytesLeft = this->capacity - this->size;
    }
    void getPrimitive(std::shared_ptr<PrimitiveValue> cell, uint32_t position) override
    {
        uint32_t bytes = cell->typeSize();

        uint8_t* values = &this->buffer[bytes * position];
        cell->Init(new Primitive(values, bytes));
    }
};

template<>
class TypedBuffer<ByteArrayType>: public Buffer {
public:
    typedef typename ByteArrayType::c_type value_type;
    void AppendPrimitive(const Primitive& data) override
    {
        uint32_t bytes = data.bytes;
        uint8_t* values = data.values;

        while (this->bytesLeft < bytes) {
            this->resize();
        }

        uint8_t* cursor =  this->buffer + this->size;        
        memcpy(cursor, values, bytes);
        this->size += bytes;
        this->bytesLeft = this->capacity - this->size;
    }
    void getPrimitive(std::shared_ptr<PrimitiveValue> cell, uint32_t position) override
    {
        uint32_t offset = 0;

        /*for(uint32_t i = 0; i < position; i++) {
            uint32_t size = *(uint32_t*)&buffer[offset];
            offset += (size + sizeof(size));
        }*/

        uint32_t type_size = *(uint32_t*)&buffer[offset];
        offset += sizeof(type_size);

        uint8_t* values = &this->buffer[offset];
        cell->Init(new Primitive(values, type_size));
    }
};

#endif // BUFFER_H
