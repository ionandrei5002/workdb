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

#include "rawbuffer.h"

RawBuffer::RawBuffer(uint32_t bufSize)
{
    this->capacity = bufSize;
    this->size = 0;
    this->buffer = free_list_alloca.allocate(bufSize);
}
RawBuffer::~RawBuffer()
{
    free_list_alloca.deallocate(this->buffer, this->capacity);
    this->buffer = nullptr;
    this->size = 0;
    this->capacity = 0;
}
uint32_t RawBuffer::getCapacity()
{
    return this->capacity;
}
uint32_t RawBuffer::getSize()
{
    return this->size;
}
void RawBuffer::resetSize()
{
    this->size = 0;
}
uint8_t * RawBuffer::getBuffer()
{
    return this->buffer;
}
bool RawBuffer::Append(uint8_t* data, uint32_t len)
{
    bool full = false;

    if (len > (capacity - size)) {
        full = true;
    } else {
        uint8_t* cursor = this->buffer + this->size;
        memcpy(cursor, data, len);
        this->size += len;
        full = false;
    }

    return full;
}
