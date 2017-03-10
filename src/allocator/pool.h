/*
 * pool.h
 *
 *  Created on: May 16, 2016
 *      Author: andrei
 */

#ifndef SRC_POOL_H_
#define SRC_POOL_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <memory.h>

class pool
{
private:
	std::size_t m_block_size = 16;
	std::size_t m_pool_size;
	std::vector<char> m_bytemap;
	std::size_t m_block_allocated = 0;
	char* m_pool_allocated = nullptr;
	std::size_t m_last_allocation = 0;
	char* find_free(std::size_t size) throw (std::string);
	void dump();
public:
	pool(std::size_t pool_size);
	virtual ~pool();
	char* allocate(std::size_t size);
	void deallocate(char* pointer, std::size_t size);
	void printByteMap(int start);
	void printFreeBlocks();
};

extern pool my_pool;

#endif /* SRC_POOL_H_ */
