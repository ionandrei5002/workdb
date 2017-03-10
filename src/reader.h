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
	static std::shared_ptr<PrimitiveReader> Make(
			std::shared_ptr<Column> column);
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
}
;

#endif /* SRC_READER_H_ */
