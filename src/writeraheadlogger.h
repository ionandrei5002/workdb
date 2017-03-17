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

#ifndef WRITERAHEADLOGGER_H
#define WRITERAHEADLOGGER_H

#include <fstream>
#include <snappy-c.h>

#include "base.h"
#include "rawbuffer.h"
#include "allocator/free_list_allocator.h"

class WriterAheadLogger
{
private:
    RawBuffer& buffer;
    std::string path;
public:
    WriterAheadLogger(RawBuffer& buffer, std::string path);
    ~WriterAheadLogger();
    void write(uint32_t walCounter);
    void read(uint32_t walCounter);
};

#endif // WRITERAHEADLOGGER_H
