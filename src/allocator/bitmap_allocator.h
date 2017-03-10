/*
 * bitmap_allocator.h
 *
 *  Created on: Dec 21, 2016
 *      Author: andrei
 */

#ifndef SRC_ALLOCATOR_BITMAP_ALLOCATOR_H_
#define SRC_ALLOCATOR_BITMAP_ALLOCATOR_H_

#include<iostream>
#include <bits/c++config.h>
#include <new>
#include <bits/functexcept.h>
#include <bits/move.h>
#include <limits>
#if __cplusplus >= 201103L
#include <type_traits>
#endif

#include "bitmap_pool.h"

template<class T>
class bitmap_allocator
{
public:
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;

#if __cplusplus >= 201103L
	typedef std::true_type propagate_on_container_move_assignment;
#endif

	bitmap_allocator()
	{
	}
	bitmap_allocator(const bitmap_allocator&)
	{
	}

	pointer allocate(size_type n, const void * = 0)
	{
//		void* t = operator new(n * sizeof(T));
//		void* t = malloc(n * sizeof(T));

		void* t = bitmap_pool.allocate(n * sizeof(T));
		return (pointer) t;
	}

	void deallocate(void* p, size_type size)
	{
		if (p)
		{
//			operator delete(p);
//			free(p);

			bitmap_pool.deallocate((char*) p);
		}
	}

	pointer address(reference x) const
	{
		return &x;
	}
	const_pointer address(const_reference x) const
	{
		return &x;
	}
	bitmap_allocator<T>& operator=(const bitmap_allocator&)
	{
		return *this;
	}

#if __cplusplus >= 201103L
	template<typename _Up, typename ... _Args>
	void construct(_Up* __p, _Args&&... __args)
	{
		::new ((void *) __p) _Up(std::forward<_Args>(__args)...);
	}

	template<typename _Up>
	void destroy(_Up* __p)
	{
		__p->~_Up();
	}
#else
	inline void construct(pointer p, const T& val)
	{
		new ((T*) p) T(val);
	}
	inline void destroy(pointer p)
	{
		p->~T();
	}
#endif

	size_type max_size() const
	{
		return std::size_t(-1);
	}

	template<class U>
	struct rebind
	{
		typedef bitmap_allocator<U> other;
	};

	template<class U>
	bitmap_allocator(const bitmap_allocator<U>&)
	{
	}

	template<class U>
	bitmap_allocator& operator=(const bitmap_allocator<U>&)
	{
		return *this;
	}
};

// return that all specializations of this allocator are interchangeable
template<class T1, class T2>
bool operator==(const bitmap_allocator<T1>&,
		const bitmap_allocator<T2>&) throw ()
{
	return true;
}
template<class T1, class T2>
bool operator!=(const bitmap_allocator<T1>&,
		const bitmap_allocator<T2>&) throw ()
{
	return false;
}

extern bitmap_allocator<uint8_t> bitmap_alloca;

#endif /* SRC_ALLOCATOR_BITMAP_ALLOCATOR_H_ */
