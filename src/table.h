/*
 * table.h
 *
 *  Created on: Sep 27, 2016
 *      Author: andrei
 */

#ifndef SRC_TABLE_H_
#define SRC_TABLE_H_

#include "definitions.h"

#include "base.h"

#include "types.h"
#include "schema.h"
#include "column.h"
#include "primitive.h"
#include "printer.h"
#include "row.h"

#include <climits>
#include <string>
#include <iostream>

class Table: public Base
{
private:
	Schema& schema;
	db::map<uint32_t, std::shared_ptr<Column>> columns;
	db::map<uint32_t, std::shared_ptr<Printer>> printers;
	db::map<uint32_t, std::shared_ptr<PrimitiveValue>> primitives;    
	void Init()
	{
		db::vector<std::shared_ptr<Node>> nodes = schema.getNodes();
		auto it = nodes.begin();
		auto end = nodes.end();
		for (; it != end; ++it)
		{
			std::shared_ptr<Node> node = *it;
			columns.emplace(
					std::make_pair<uint32_t, std::shared_ptr<Column>>(
							columns.size(), Column::Make(node)));
			printers.emplace(
					std::make_pair<uint32_t, std::shared_ptr<Printer>>(
							printers.size(), Printer::Make(node)));
			primitives.emplace(
					std::make_pair<uint32_t, std::shared_ptr<PrimitiveValue>>(
							primitives.size(), Row::Make(node)));
		}
	}

public:
    uint32_t walCounter = 0;
	Table(Schema& schema) :
			schema(schema)
	{
		Init();
	}
	~Table()
	{
	}
	void Debug();
	std::shared_ptr<Column> getColumn(uint32_t column);
	std::shared_ptr<Printer> getPrinter(uint32_t column);
	bool validate();
	uint32_t num_columns();
	uint32_t num_rows();
	db::map<uint32_t, std::shared_ptr<PrimitiveValue>>* getPrimitives();
	db::map<uint32_t, std::shared_ptr<Printer>>* getPrinters();
	db::map<uint32_t, std::shared_ptr<Column>>* getColumns();
	void buildFrom(db::vector<std::shared_ptr<Row>> rows);

	Schema& getSchema()
	{
		return schema;
	}

	void Release() {
		columns.clear();
		printers.clear();
		primitives.clear();
	}
};

#endif /* SRC_TABLE_H_ */
