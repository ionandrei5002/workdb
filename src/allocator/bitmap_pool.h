/*
 * bitmap_pool.h
 *
 *  Created on: Dec 20, 2016
 *      Author: andrei
 */

#ifndef SRC_ALLOCATOR_BITMAP_POOL_H_
#define SRC_ALLOCATOR_BITMAP_POOL_H_

#include "pool_interface.h"

#include <iostream>
#include <vector>

class BitMapPool: public PoolInterface
{
private:
	std::vector<uint8_t>* bitmap = nullptr;
	char* watermark = nullptr;
	char* cursor = nullptr;
	char* head = nullptr;
	char* tail = nullptr;
	uint32_t m_header_size = 8;
	uint32_t m_block_size = 8;
public:
	BitMapPool(uint32_t size)
	{
		head = new char[size];

		std::cout << "pool head at " << (void*) head << std::endl;

		cursor = head;
		watermark = head;
		tail = head + size;

		bitmap = new std::vector<uint8_t>();
		bitmap->resize(size / m_block_size);

		for (uint32_t i = 0; i < (size / m_block_size); i++)
		{
			bitmap->operator [](i) = 0;
		}

		std::cout << bitmap->size() << std::endl;

		std::cout << "pool tail at " << (void*) tail << std::endl;
	}
	~BitMapPool()
	{
		uint64_t bytes_left = tail - watermark;
		std::cout << (bytes_left / 1024 / 1024) << "Mb" << std::endl;

		cursor = nullptr;
		tail = nullptr;

		delete[] head;
		head = nullptr;

		delete bitmap;
		bitmap = nullptr;

		std::cout << "pool deleted" << std::endl;
	}

	void Debug()
	{
		uint32_t i = 0;
		uint32_t end = bitmap->size();
		for (; i < end; ++i)
		{
			uint8_t status = bitmap->operator [](i);
			if (status == 0)
			{
				uint32_t block = i;
				for (; i < end; ++i)
				{
					if (bitmap->operator [](i) != status)
						break;
				}
				std::cout << "free area : " << block << " - " << i << std::endl;
			}
			else
			{
				uint32_t block = i;
				for (; i < end; ++i)
				{
					if (bitmap->operator [](i) != status)
						break;
				}
				std::cout << "used area : " << block << " - " << i << std::endl;
			}
		}
	}

	inline char* allocate(std::size_t size) override
	{
		char* pointer = nullptr;

		std::size_t total_size = getAlignedSize(size);

		int pos = getFreeBlock(total_size);

		if (pos == -1)
		{
			pointer = (char*) new char[size];
		}
		else
		{
			pointer = head + (pos * m_block_size);
			*(uint64_t *) pointer = size;
			pointer = pointer + m_header_size;

			if (cursor > watermark)
			{
				watermark = cursor;
			}

//			this->Debug();
		}
		return pointer;
	}

	inline void deallocate(char* pointer) override
	{
		if (pointer >= head && pointer < tail)
		{
			uint32_t pos = this->getPosBitMap(pointer);
			uint64_t size = *(uint64_t*) (pointer - m_header_size);

			std::size_t total_size = getAlignedSize(size);

			uint32_t i = pos - 1;
			uint32_t length = (total_size + m_header_size) / m_block_size;
			for (; i < length; ++i)
			{
				bitmap->operator [](i) = 0;
			}
//			this->Debug();
			char* new_cursor = head + ((pos - 1) * m_block_size);
			if (cursor > new_cursor)
			{
				cursor = new_cursor;
			}
		}
		else
		{
			delete[] pointer;
			pointer = nullptr;
		}
	}
private:
	inline uint32_t getAlignedSize(uint32_t size)
	{
		std::size_t total_size = 0;
		std::size_t align = (size & (m_block_size - 1));

		if (align == 0)
		{
			total_size = size;
		}
		else
		{
			total_size = (size + (m_block_size - align));
		}

		return total_size;
	}
	inline uint32_t getFreeBlock(uint32_t total_size)
	{
		uint32_t blocksToFind = (total_size + m_header_size) / m_block_size;
		uint32_t pos = this->getPosBitMap(cursor);

		bool found = false;

		uint32_t i = pos;
		uint32_t length = bitmap->size();
		for (; i < length; ++i)
		{
			uint8_t status = bitmap->operator [](i);
			if (status == 0)
			{
				uint32_t j = i;
				uint32_t max_search = i + blocksToFind;

				if (max_search > length)
				{
					found = false;
					break;
				}

				for (; j < max_search; ++j)
				{
					if (bitmap->operator [](j) == 1)
					{
						found = false;
						i = j + 1;
						break;
					}
					found = true;
				}

				if (found == true)
				{
					pos = i;
					break;
				}
			}
		}

		if (found == false)
		{
			return -1;
		}
		else
		{
			uint32_t i = pos;
			uint32_t max_search = i + blocksToFind;
			for (; i < max_search; ++i)
			{
				bitmap->operator [](i) = 1;
			}

			cursor = head + (max_search * 8);

			return pos;
		}
	}
	inline uint32_t getPosBitMap(char* pointer)
	{
		uint32_t pos = 0;

		pos = ((char*) pointer - (char*) head) / m_block_size;

		return pos;
	}
};

extern BitMapPool bitmap_pool;

#endif /* SRC_ALLOCATOR_BITMAP_POOL_H_ */
