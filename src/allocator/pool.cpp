/*
 * pool.cpp
 *
 *  Created on: May 16, 2016
 *      Author: andrei
 */

#include "pool.h"

pool::pool(std::size_t pool_size)
{
	m_pool_size = pool_size;
	m_block_allocated = m_pool_size / m_block_size;
	m_bytemap.reserve(m_block_allocated);

	m_pool_allocated = ::new char[pool_size];

	std::cout << __FILE__ << " " << __LINE__ << " pool addr "
			<< (void*) m_pool_allocated << std::endl;

	std::cout << "blocks allocated " << m_block_allocated << std::endl;

	for (unsigned i = 0; i < m_block_allocated; i++)
	{
		m_bytemap.push_back(0);
	}
}

pool::~pool()
{
	delete[] m_pool_allocated;

	std::cout << "pool free" << std::endl;
}

char* pool::allocate(std::size_t size)
{
	char* pointer = find_free(size);

	return (pointer);
}

void pool::deallocate(char* pointer, std::size_t size)
{
	std::size_t position = (pointer) - m_pool_allocated;
	position = (position / m_block_size);

	std::size_t total_size = 0;

	total_size = (size + (m_block_size - size % m_block_size));

	std::size_t blocks = total_size / m_block_size;

	for (unsigned i = position; i < (position + blocks); i++)
	{
		m_bytemap[i] = 0;
	}

}

char* pool::find_free(std::size_t size) throw (std::string)
{
	std::size_t total_size = 0;
	total_size = (size + (m_block_size - size % m_block_size));


	std::size_t blocks = total_size / m_block_size;

	char* last_allocation = m_pool_allocated
			+ (m_last_allocation * m_block_size);

	bool available = false;
	if (last_allocation < (m_pool_allocated + m_block_allocated * m_block_size))
	{
		available = true;
	}

	if (available == false)
	{
		std::cout << "memory request: " << size << std::endl;
		std::cout << "total memory needed: " << total_size << std::endl;
		std::cout << "blocks to allocate: " << blocks << std::endl;

		throw("out_of_memory");
	}
	else
	{
		for (unsigned j = m_last_allocation; j < blocks; j++)
		{
			m_bytemap[j] = 1;
		}

		m_last_allocation = (m_last_allocation + blocks);
	}

	return last_allocation;
}

void pool::printFreeBlocks()
{
	std::cout << "blocks allocated = " << m_block_allocated << std::endl;

	int free_blocks_size = 0;
	int free_pos = 0;

	int used_blocks_size = 0;
	int used_pos = 0;

	for (unsigned i = 0; i < m_block_allocated; i++)
	{
		if (m_bytemap[i] == 0)
		{
			if (free_pos == 0)
			{
				free_pos = i;
			}

			free_blocks_size++;

			if (used_blocks_size > 0)
			{
				std::cout << "used i = " << used_pos << " & blocks = "
						<< used_blocks_size << std::endl;
			}

			used_blocks_size = 0;
			used_pos = 0;
		}
		else
		{
			if (used_pos == 0)
			{
				used_pos = i;
			}

			used_blocks_size++;

			if (free_blocks_size > 0)
			{
				std::cout << "free i = " << free_pos << " & blocks = "
						<< free_blocks_size << std::endl;
			}

			free_pos = 0;
			free_blocks_size = 0;
		}
	}

	if (used_blocks_size > 0)
	{
		std::cout << "used i = " << used_pos << " & blocks = "
				<< used_blocks_size << std::endl;
	}
	if (free_blocks_size > 0)
	{
		std::cout << "free i = " << free_pos << " & blocks = "
				<< free_blocks_size << std::endl;
	}
}

void pool::printByteMap(int start)
{
	for (unsigned i = start; i < m_block_allocated; i++)
	{
		if (i % 64 == 0)
		{
			std::cout << std::endl;
		}
		std::cout << (int) m_bytemap[i] << " ";
	}

	std::cout << std::endl;
}

void pool::dump()
{
	std::string filePath = "/home/andrei/Desktop/dump.dat";
	std::fstream file(filePath, std::ios_base::out | std::ios_base::binary);

	file.write(m_pool_allocated, m_pool_size);

	file.close();
}
