/*
 * schema.h
 *
 *  Created on: Sep 27, 2016
 *      Author: andrei
 */

#ifndef SRC_SCHEMA_H_
#define SRC_SCHEMA_H_

#include "definitions.h"

#include "base.h"

#include "types.h"
#include <vector>
#include <string>
#include <memory>

class Node: public Base
{
protected:
	Type::type type;
	std::string name;
public:
	std::shared_ptr<Node> node;

	Node(Type::type type, std::string name) :
			type(type), name(name)
	{
	}
	virtual ~Node()
	{
	}

	Type::type getType()
	{
		return type;
	}

	std::string getName()
	{
		return name;
	}
	virtual bool complexType() = 0;
	virtual std::shared_ptr<Node> typeGet() = 0;
	virtual std::string toString() = 0;
};

template<typename T>
class TypedNode: public Node
{
public:
	explicit TypedNode(std::string name) :
			Node(T::type_num, name)
	{
		this->node = std::make_shared<TypedNode<T>>(*this);
	}

	bool complexType() override
	{
		return false;
	}

	std::shared_ptr<Node> typeGet() override
	{
		std::shared_ptr<Node> flaten = std::make_shared<TypedNode<T>>(*this);

		return flaten;
	}

	std::string toString() override
	{
		return std::string(type_traits<T::type_num>::name);
	}
};

class Schema: public Base
{
private:
	db::vector<std::shared_ptr<Node>> nodes;
public:
	Schema(db::vector<std::shared_ptr<Node>> nodes) :
			nodes(nodes)
	{
	}
	void Debug()
	{
		std::cout << "Schema : " << std::endl;

		auto it = nodes.begin();
		auto end = nodes.end();
		for (; it != end; ++it)
		{
			std::shared_ptr<Node> node = *it;
			std::cout << node->getName() << " " << node->toString()
					<< std::endl;
		}
	}
	std::shared_ptr<Node> getNode(uint32_t node)
	{
		return nodes.at(node);
	}
	db::vector<std::shared_ptr<Node>> getNodes()
	{
		return nodes;
	}
};

#endif /* SRC_SCHEMA_H_ */
