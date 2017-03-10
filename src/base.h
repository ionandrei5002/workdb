/*
 * base.h
 *
 *  Created on: Oct 3, 2016
 *      Author: andrei
 */

#ifndef SRC_BASE_H_
#define SRC_BASE_H_

#include "allocator/free_list_allocator.h"

class Base
{
public:
	void* operator new(std::size_t size)
	{
		return free_list_alloca.allocate(size);
	}
	void* operator new(std::size_t size, void* pointer)
	{
		return pointer;
	}
	void operator delete(void* pointer, std::size_t size)
	{
		free_list_alloca.deallocate(pointer, size);
	}
};

#endif /* SRC_BASE_H_ */
