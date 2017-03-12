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

#include "buffer.h"

Buffer::Buffer()
{
    this->capacity = sizeof(uint64_t);
    this->size = 0;
    this->bytesLeft = this->capacity;
    this->buffer = (uint8_t*)malloc(this->capacity);
}
Buffer::~Buffer()
{
    free((void*)this->buffer);
    this->buffer = nullptr;
    this->capacity = 0;
    this->size = 0;
}
uint32_t Buffer::getCapacity()
{
    return this->capacity;
}
uint32_t Buffer::getSize()
{
    return this->size;
}
void Buffer::resize()
{
    uint32_t new_capacity = this->capacity * 2;
    uint8_t* new_buffer = (uint8_t*)malloc(new_capacity);
    memcpy(new_buffer, this->buffer, this->size);
    this->capacity = new_capacity;
    free(this->buffer);
    this->buffer = new_buffer;
    new_buffer = nullptr;
    this->bytesLeft = this->capacity - this->size;
}

/*
void Buffer::AppendPrimitive(const Primitive& data)
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
void Buffer::getPrimitive(std::shared_ptr<PrimitiveValue> cell, uint32_t position) {
    uint32_t bytes = cell->typeSize();
    uint8_t* values = &this->buffer[bytes * position];
    cell->Init(new Primitive(values, bytes));
}
*/
