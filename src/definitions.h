/*
 * definitions.h
 *
 *  Created on: Dec 21, 2016
 *      Author: andrei
 */

#ifndef SRC_DEFINITIONS_H_
#define SRC_DEFINITIONS_H_

#include "allocator/free_list_allocator.h"
#include <vector>
#include <map>

namespace db
{
template<typename T>
using vector = typename std::vector<T, free_list_allocator<T>>;

template<typename T, typename U>
using map = typename std::map<T, U, std::less<T>, free_list_allocator<std::pair<T,U>>>;
}

#endif /* SRC_DEFINITIONS_H_ */
