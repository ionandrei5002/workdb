/*
 * main.cpp
 *
 *  Created on: Dec 19, 2016
 *      Author: andrei
 */

#include <chrono>

#include "definitions.h"

#include "allocator/free_list_pool.h"
#include "schema.h"
#include "table.h"
#include "reader.h"
#include "dataset.h"

FreeListPool freelist_pool = FreeListPool(800 * 1024 * 1024);

int main(int argc, char **argv)
{
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

	start = std::chrono::high_resolution_clock::now();

	db::vector<std::shared_ptr<Node>> nodes;
	nodes.push_back(std::make_shared<TypedNode<Int64Type>>("udid_id"));
	nodes.push_back(std::make_shared<TypedNode<Int32Type>>("ver_id"));
	nodes.push_back(std::make_shared<TypedNode<Int16Type>>("country_id"));
	nodes.push_back(std::make_shared<TypedNode<Int16Type>>("nb_launches"));
	nodes.push_back(std::make_shared<TypedNode<Int16Type>>("sessions"));
	nodes.push_back(std::make_shared<TypedNode<Int32Type>>("time_spent"));
	nodes.push_back(std::make_shared<TypedNode<Int32Type>>("level"));

	Schema schema(nodes);
//	schema.Debug();

	Table table(schema);
//	table.Debug();

	RowReader reader(table);

	std::ifstream is;
	is.open("/home/andrei/Desktop/oldies/desktop/aria_bins/dau/2016-04-01/52108.aria",
			std::ifstream::binary);
	reader.read(is);
	std::cout << "valid: " << table.validate() << std::endl;
	std::cout << "rows: " << table.num_rows() << std::endl;

    
	uint16_t launches = 10;
	uint16_t country = 44;
	int32_t level = 10;

	db::vector<std::shared_ptr<Row>> rows;
	Dataset set = Dataset::Init(table);

	db::vector<uint32_t> columns
	{ 0, 1, 2, 3, 6 };

	set.Fields(columns).Filter(
			Predicate::Make(table, 3, (uint8_t*) &launches,
					Condition::EQUAL_MORE, false)).Filter(
			Predicate::Make(table, 2, (uint8_t*) &country, Condition::EQUAL,
					false)).Filter(
			Predicate::Make(table, 6, (uint8_t*) &level, Condition::EQUAL,
					false)).getRows(rows);

	std::cout << " ======================== " << std::endl;
	std::cout << "rows = " << rows.size() << std::endl;

	auto it = rows.begin();
	auto endIt = rows.end();
	for (; it != endIt; ++it)
	{
		std::shared_ptr<Row> row = (*it);
		//row->toString();

		//std::cout << std::endl;
	}

	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_time = end - start;

	std::cout << "duration = " << elapsed_time.count() << "s" << std::endl;
    
	return 0;
}