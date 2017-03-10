/*
 * types.h
 *
 *  Created on: Sep 27, 2016
 *      Author: andrei
 */

#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

#include "base.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <string>
#include <iostream>

struct Type
{

	enum type
	{
		NONE = 0,
		UINT8 = 1,
		INT8 = 2,
		UINT16 = 3,
		INT16 = 4,
		UINT32 = 5,
		INT32 = 6,
		UINT64 = 7,
		INT64 = 8,
		BYTE_ARRAY = 9,
		FLOAT = 10,
		DOUBLE = 11,
		BOOLEAN = 12
	};
};

struct ByteArray: public Base
{
	uint32_t len;
	const uint8_t* ptr;

	ByteArray() :
			len(0), ptr(nullptr)
	{
	}

	ByteArray(const uint8_t* ptr, uint32_t len) :
			len(len), ptr(ptr)
	{
	}

	bool operator==(const ByteArray& other) const
	{
		return this->len == other.len
				&& 0 == memcmp(this->ptr, other.ptr, this->len);
	}

	bool operator!=(const ByteArray& other) const
	{
		return this->len != other.len
				|| 0 != memcmp(this->ptr, other.ptr, this->len);
	}

	friend std::ostream &operator<<(std::ostream &output,
			const ByteArray &input)
	{
		output.write((char*) &input.len, sizeof(uint32_t));
		output.write((char*) input.ptr, input.len);
		return output;
	}
};

static inline std::string ByteArrayToString(const ByteArray& a)
{
	return std::string(reinterpret_cast<const char*>(a.ptr), a.len);
}

struct PairArray: public Base
{
	uint32_t len;
	const uint8_t* ptr;

	PairArray() :
			len(0), ptr(nullptr)
	{
	}

	PairArray(const uint8_t* ptr, uint32_t len) :
			len(len), ptr(ptr)
	{
	}

	friend std::ostream &operator<<(std::ostream &output,
			const PairArray &input)
	{
		output.write((char*) &input.len, sizeof(uint32_t));
		output.write((char*) input.ptr, input.len);
		return output;
	}
};

struct ListArray: public Base
{
	uint32_t len;
	const uint8_t* ptr;

	ListArray() :
			len(0), ptr(nullptr)
	{
	}

	ListArray(const uint8_t* ptr, uint32_t len) :
			len(len), ptr(ptr)
	{
	}

	friend std::ostream &operator<<(std::ostream &output,
			const ListArray &input)
	{
		output.write((char*) &input.len, sizeof(uint32_t));
		output.write((char*) input.ptr, input.len);
		return output;
	}
};

struct SetArray: public Base
{
	uint32_t len;
	const uint8_t* ptr;

	SetArray() :
			len(0), ptr(nullptr)
	{
	}

	SetArray(const uint8_t* ptr, uint32_t len) :
			len(len), ptr(ptr)
	{
	}

	friend std::ostream &operator<<(std::ostream &output, const SetArray &input)
	{
		output.write((char*) &input.len, sizeof(uint32_t));
		output.write((char*) input.ptr, input.len);
		return output;
	}
};

struct MapArray: public Base
{
	uint32_t len;
	const uint8_t* ptr;

	MapArray() :
			len(0), ptr(nullptr)
	{
	}

	MapArray(const uint8_t* ptr, uint32_t len) :
			len(len), ptr(ptr)
	{
	}

	friend std::ostream &operator<<(std::ostream &output, const MapArray &input)
	{
		output.write((char*) &input.len, sizeof(uint32_t));
		output.write((char*) input.ptr, input.len);
		return output;
	}
};

struct StructArray: public Base
{
	uint32_t len;
	const uint8_t* ptr;

	StructArray() :
			len(0), ptr(nullptr)
	{
	}

	StructArray(const uint8_t* ptr, uint32_t len) :
			len(len), ptr(ptr)
	{
	}

	friend std::ostream &operator<<(std::ostream &output,
			const StructArray &input)
	{
		output.write((char*) &input.len, sizeof(uint32_t));
		output.write((char*) input.ptr, input.len);
		return output;
	}
};

template<int TYPE>
struct type_traits
{
};

template<>
struct type_traits<Type::NONE>
{
	typedef void value_type;
	static constexpr char* name = (char*) "NONE";
	static constexpr int value_byte_size = 0;
};

template<>
struct type_traits<Type::BOOLEAN>
{
	typedef bool value_type;
	static constexpr char* name = (char*) "BOOLEAN";
	static constexpr int value_byte_size = 1;
};

template<>
struct type_traits<Type::UINT8>
{
	typedef uint8_t value_type;
	static constexpr char* name = (char*) "UINT8";
	static constexpr int value_byte_size = 1;
};

template<>
struct type_traits<Type::INT8>
{
	typedef int8_t value_type;
	static constexpr char* name = (char*) "INT8";
	static constexpr int value_byte_size = 1;
};

template<>
struct type_traits<Type::UINT16>
{
	typedef uint16_t value_type;
	static constexpr char* name = (char*) "UINT16";
	static constexpr int value_byte_size = 2;
};

template<>
struct type_traits<Type::INT16>
{
	typedef int16_t value_type;
	static constexpr char* name = (char*) "INT16";
	static constexpr int value_byte_size = 2;
};

template<>
struct type_traits<Type::UINT32>
{
	typedef uint32_t value_type;
	static constexpr char* name = (char*) "UINT32";
	static constexpr int value_byte_size = 4;
};

template<>
struct type_traits<Type::INT32>
{
	typedef int32_t value_type;
	static constexpr char* name = (char*) "INT32";
	static constexpr int value_byte_size = 4;
};

template<>
struct type_traits<Type::UINT64>
{
	typedef uint64_t value_type;
	static constexpr char* name = (char*) "UINT64";
	static constexpr int value_byte_size = 8;
};

template<>
struct type_traits<Type::INT64>
{
	typedef int64_t value_type;
	static constexpr char* name = (char*) "INT64";
	static constexpr int value_byte_size = 8;
};

template<>
struct type_traits<Type::FLOAT>
{
	typedef float value_type;
	static constexpr char* name = (char*) "FLOAT";
	static constexpr int value_byte_size = 4;
};

template<>
struct type_traits<Type::DOUBLE>
{
	typedef double value_type;
	static constexpr char* name = (char*) "DOUBLE";
	static constexpr int value_byte_size = 8;
};

template<>
struct type_traits<Type::BYTE_ARRAY>
{
	typedef ByteArray value_type;
	static constexpr char* name = (char*) "BYTE_ARRAY";
	static constexpr int value_byte_size = sizeof(ByteArray);
};

template<Type::type TYPE>
struct DataType
{
	static constexpr Type::type type_num = TYPE;
	static constexpr char* name = type_traits<TYPE>::name;
	typedef typename type_traits<TYPE>::value_type c_type;
};

typedef DataType<Type::NONE> NoneType;
typedef DataType<Type::BOOLEAN> BooleanType;
typedef DataType<Type::UINT8> UInt8Type;
typedef DataType<Type::INT8> Int8Type;
typedef DataType<Type::UINT16> UInt16Type;
typedef DataType<Type::INT16> Int16Type;
typedef DataType<Type::UINT32> UInt32Type;
typedef DataType<Type::INT32> Int32Type;
typedef DataType<Type::UINT64> UInt64Type;
typedef DataType<Type::INT64> Int64Type;
typedef DataType<Type::FLOAT> FloatType;
typedef DataType<Type::DOUBLE> DoubleType;
typedef DataType<Type::BYTE_ARRAY> ByteArrayType;

#endif /* SRC_TYPES_H_ */
