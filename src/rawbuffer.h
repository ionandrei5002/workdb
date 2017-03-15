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

#ifndef RAWBUFFER_H
#define RAWBUFFER_H

#include <memory.h>
#include "base.h"
#include "allocator/free_list_allocator.h"

class RawBuffer
{
private:
    uint8_t* buffer;
    uint32_t capacity;
    uint32_t size;
    uint32_t bytesLeft;
public:
    RawBuffer(uint32_t bufSize);
    ~RawBuffer();
    RawBuffer(const RawBuffer& other) = delete;
    RawBuffer(RawBuffer&& other) = delete;
    RawBuffer& operator=(const RawBuffer& other) = delete;
    RawBuffer& operator=(RawBuffer&& other) = delete;
    uint32_t getCapacity();
    uint32_t getSize();
    void resetSize();
    uint8_t* getBuffer();
    bool Append(uint8_t* data, uint32_t len);
};

#endif // RAWBUFFER_H
