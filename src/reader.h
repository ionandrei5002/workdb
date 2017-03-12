/*
 * reader.h
 *
 *  Created on: Sep 27, 2016
 *      Author: andrei
 */

#ifndef SRC_READER_H_
#define SRC_READER_H_

#include "base.h"
#include "types.h"
#include "column.h"
#include "table.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <netinet/in.h>
#include "allocator/free_list_allocator.h"

class PrimitiveReader: public Base
{
public:
    std::shared_ptr<Column> column;

    PrimitiveReader(std::shared_ptr<Column> column) :
        column(column)
    {
    }
    virtual ~PrimitiveReader()
    {
    }
    static std::shared_ptr<PrimitiveReader> Make(std::shared_ptr<Column> column);
    virtual uint32_t read(uint8_t* buff) = 0;
};

template<typename T>
class TypedPrimitiveReader: public PrimitiveReader
{
public:
    TypedPrimitiveReader(std::shared_ptr<Column> column) :
        PrimitiveReader(column)
    {
    }

    inline uint32_t read(uint8_t * buff)
    {
        uint32_t size = column->Append((uint8_t*) buff);

        return size;
    }
}
;

template<>
class TypedPrimitiveReader<ByteArrayType> : public PrimitiveReader
{
public:
    TypedPrimitiveReader(std::shared_ptr<Column> column) :
        PrimitiveReader(column)
    {
    }

    uint32_t read(uint8_t* buff)
    {
        uint32_t size = column->Append((uint8_t*) buff);

        return size;
    }
};

class RowReader: public Base
{
private:
    std::vector<std::shared_ptr<PrimitiveReader>> reader;
public:

    RowReader(Table& table)
    {
        uint32_t num_columns = table.num_columns();
        uint32_t i = 0;
        for (; i < num_columns; i++)
        {
            reader.push_back(PrimitiveReader::Make(table.getColumn(i)));
        }
    }

    void read(std::ifstream& in)
    {
        in.seekg(0, std::ios::end);
        uint32_t file_size = in.tellg();
        in.seekg(0, std::ios::beg);

        std::cout << "file size = " << file_size << std::endl;

        uint8_t* buffer = free_list_alloca.allocate(file_size);

        in.read((char*) buffer, file_size);

        uint32_t bytes_read = 0;
        while (bytes_read < file_size)
        {
            auto it = reader.begin();
            auto end = reader.end();
            for (; it != end; ++it)
            {
                std::shared_ptr<PrimitiveReader> read = (*it);
                bytes_read += read->read(&buffer[bytes_read]);
            }
        }

        free_list_alloca.deallocate((void*) buffer, file_size);
    }
};

class StringReader: public Base {
public:
    std::shared_ptr<Column> column;
    StringReader(std::shared_ptr<Column> column) :
        column(column) {
    }
    virtual ~StringReader() {
    }
    static std::shared_ptr<StringReader> Make(std::shared_ptr<Column> column);
    virtual void read(std::string* in) = 0;
};

template<typename T>
class TypedStringReader: public StringReader {
public:
    TypedStringReader(std::shared_ptr<Column> column) :
        StringReader(column) {
    }

    void read(std::string* in) override {
        uint32_t size = type_traits<T::type_num>::value_byte_size;
        char buffer[size];

        switch (T::type_num) {
        case Type::UINT8:
        case Type::INT8: {
            int8_t p = (int8_t) std::stoi(*in);
            memcpy((void*) &buffer[0], reinterpret_cast<void*>(&p), size);
        }
        break;
        case Type::UINT16:
        case Type::INT16: {
            short p = (short) std::stoi(*in);
            memcpy((void*) &buffer[0], reinterpret_cast<void*>(&p), size);
        }
        break;
        case Type::UINT32:
        case Type::INT32: {
            int p = (int) std::stoi(*in);
            memcpy((void*) &buffer[0], reinterpret_cast<void*>(&p), size);
        }
        break;
        case Type::UINT64:
        case Type::INT64: {
            long p = (long) std::stol(*in);
            memcpy((void*) &buffer[0], reinterpret_cast<void*>(&p), size);
        }
        break;
        case Type::FLOAT: {
            float p = (float) std::stof(*in);
            memcpy((void*) &buffer[0], reinterpret_cast<void*>(&p), size);
        }
        break;
        case Type::DOUBLE: {
            double p = (double) std::stod(*in);
            memcpy((void*) &buffer[0], reinterpret_cast<void*>(&p), size);
        }
        break;
        }

        column->Append((uint8_t*) &buffer[0]);
    }
};

template<>
class TypedStringReader<ByteArrayType> : public StringReader {
public:
    TypedStringReader(std::shared_ptr<Column> column) :
        StringReader(column) {
    }

    void read(std::string* in) override {
        uint32_t size = in->size();
        char buffer[size + sizeof(size)];
        memcpy((void*) &buffer[0], (void*) &size, sizeof(size));
        memcpy((void*) &buffer[sizeof(size)], (void*) in->data(), size);

        column->Append((uint8_t*) &buffer[0]);
    }
};

class CsvReader: public Base {
private:
    Table& table;
    std::vector<std::shared_ptr<StringReader>> reader;
public:
    CsvReader(Table& table) :
        table(table) {
        uint32_t num_columns = table.num_columns();
        for (uint32_t i = 0; i < num_columns; i++) {
            reader.push_back(StringReader::Make(table.getColumn(i)));
        }
    }
    void read(std::ifstream& in) {
        while (true) {
            std::string line;
            std::getline(in, line);

            if (in.eof()) {
                break;
            }

            std::stringstream lineStream(line);
            std::string token;

            for (auto it = reader.begin(); it != reader.end(); ++it) {
                std::shared_ptr<StringReader> read = (*it);
                std::getline(lineStream, token, ';');
                read->read(&token);
            }
        }
    }
};

#endif /* SRC_READER_H_ */
