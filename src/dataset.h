/*
 * dataset.h
 *
 *  Created on: Sep 28, 2016
 *      Author: andrei
 */

#ifndef SRC_DATASET_H_
#define SRC_DATASET_H_

#include "definitions.h"

#include "base.h"
#include "types.h"
#include "primitive.h"
#include "table.h"

#include <vector>

struct Condition: public Base
{
    enum condition
    {
        EQUAL = 0, LESS = 1, MORE = 2, EQUAL_LESS = 3, EQUAL_MORE = 4
    };
};

struct Predicate: public Base
{
public:
    uint32_t column;
    std::shared_ptr<PrimitiveValue> value;
    bool toXCompare;
    Predicate(uint32_t column, std::shared_ptr<PrimitiveValue> value,
              bool compare) :
        column(column), value(value), toXCompare(compare)
    {
    }
    virtual ~Predicate()
    {
    }

    virtual bool compare(const std::shared_ptr<PrimitiveValue>& other) = 0;

    static std::shared_ptr<Predicate> Make(Table& table, uint32_t column,
                                           uint8_t* value, Condition::condition condition, bool compare);
};

template<Condition::condition Condition>
struct ConditionalPredicat: public Predicate
{
    bool compare(const std::shared_ptr<PrimitiveValue>& other) override
    {
        return false;
    }
};

template<>
struct ConditionalPredicat<Condition::EQUAL> : public Predicate
{
    ConditionalPredicat(uint32_t column, std::shared_ptr<PrimitiveValue> value,
                        bool compare) :
        Predicate(column, value, compare)
    {
    }
    bool compare(const std::shared_ptr<PrimitiveValue>& other) override
    {

    }
};

template<>
struct ConditionalPredicat<Condition::LESS> : public Predicate
{
    ConditionalPredicat(uint32_t column, std::shared_ptr<PrimitiveValue> value,
                        bool compare) :
        Predicate(column, value, compare)
    {
    }
    bool compare(const std::shared_ptr<PrimitiveValue>& other) override
    {

    }
};

template<>
struct ConditionalPredicat<Condition::MORE> : public Predicate
{
    ConditionalPredicat(uint32_t column, std::shared_ptr<PrimitiveValue> value,
                        bool compare) :
        Predicate(column, value, compare)
    {
    }
    bool compare(const std::shared_ptr<PrimitiveValue>& other) override
    {

    }
};

template<>
struct ConditionalPredicat<Condition::EQUAL_LESS> : public Predicate
{
    ConditionalPredicat(uint32_t column, std::shared_ptr<PrimitiveValue> value,
                        bool compare) :
        Predicate(column, value, compare)
    {
    }
    bool compare(const std::shared_ptr<PrimitiveValue>& other) override
    {

    }
};

template<>
struct ConditionalPredicat<Condition::EQUAL_MORE> : public Predicate
{
    ConditionalPredicat(uint32_t column, std::shared_ptr<PrimitiveValue> value,
                        bool compare) :
        Predicate(column, value, compare)
    {
    }
    bool compare(const std::shared_ptr<PrimitiveValue>& other) override
    {

    }
};

typedef ConditionalPredicat<Condition::EQUAL> EqualPredicate;
typedef ConditionalPredicat<Condition::LESS> LessPredicate;
typedef ConditionalPredicat<Condition::MORE> MorePredicate;
typedef ConditionalPredicat<Condition::EQUAL_LESS> EqualLessPredicate;
typedef ConditionalPredicat<Condition::EQUAL_MORE> EqualMorePredicate;

std::shared_ptr<Predicate> Predicate::Make(Table& table, uint32_t column,
        uint8_t* value, Condition::condition condition, bool compare)
{
    Type::type value_type;
    value_type = table.getColumn(column)->node->getType();

    uint32_t value_byte_size = 0;

    switch (value_type)
    {
    case (Type::NONE):
        value_byte_size = type_traits<Type::NONE>::value_byte_size;
        break;
    case (Type::BOOLEAN):
        value_byte_size = type_traits<Type::BOOLEAN>::value_byte_size;
        break;
    case (Type::UINT8):
        value_byte_size = type_traits<Type::UINT8>::value_byte_size;
        break;
    case (Type::INT8):
        value_byte_size = type_traits<Type::INT8>::value_byte_size;
        break;
    case (Type::UINT16):
        value_byte_size = type_traits<Type::UINT16>::value_byte_size;
        break;
    case (Type::INT16):
        value_byte_size = type_traits<Type::INT16>::value_byte_size;
        break;
    case (Type::UINT32):
        value_byte_size = type_traits<Type::UINT32>::value_byte_size;
        break;
    case (Type::INT32):
        value_byte_size = type_traits<Type::INT32>::value_byte_size;
        break;
    case (Type::UINT64):
        value_byte_size = type_traits<Type::UINT64>::value_byte_size;
        break;
    case (Type::INT64):
        value_byte_size = type_traits<Type::INT64>::value_byte_size;
        break;
    case (Type::FLOAT):
        value_byte_size = type_traits<Type::FLOAT>::value_byte_size;
        break;
    case (Type::DOUBLE):
        value_byte_size = type_traits<Type::DOUBLE>::value_byte_size;
        break;
    case (Type::BYTE_ARRAY):
        value_byte_size = type_traits<Type::BYTE_ARRAY>::value_byte_size;
        break;
    default:
        value_byte_size = 0;
    }

    Primitive* res = new Primitive(value, value_byte_size);
    std::shared_ptr<PrimitiveValue> cell = table.getColumns()->at(column)->getThisPrimitive();
    cell->chunk = res;

    switch (condition)
    {
    case (Condition::EQUAL):
        return std::make_shared<EqualPredicate>(column, cell, compare);
        break;
    case (Condition::LESS):
        return std::make_shared<LessPredicate>(column, cell, compare);
        break;
    case (Condition::MORE):
        return std::make_shared<MorePredicate>(column, cell, compare);
        break;
    case (Condition::EQUAL_LESS):
        return std::make_shared<EqualLessPredicate>(column, cell, compare);
        break;
    case (Condition::EQUAL_MORE):
        return std::make_shared<EqualMorePredicate>(column, cell, compare);
        break;
    }

    return nullptr;
}

class Dataset: public Base
{
private:
    db::vector<std::shared_ptr<Predicate>> predicates;
    Table& table;
    db::vector<uint32_t> m_fields;
    Dataset(Table& table) :
        table(table)
    {
    }
public:
    static Dataset Init(Table& table)
    {
        return Dataset(table);
    }

    Dataset Fields(db::vector<uint32_t> fields)
    {
        m_fields = fields;
        return *this;
    }

    Dataset Filter(std::shared_ptr<Predicate> predicate)
    {
        predicates.push_back(predicate);
        return *this;
    }

    Dataset Filter(db::vector<std::shared_ptr<Predicate>> predicate)
    {
        auto it = predicate.begin();
        auto end = predicate.end();
        for (; it != end; ++it)
        {
            predicates.push_back(*it);
        }

        return *this;
    }

    Schema getSchema()
    {
        db::vector<std::shared_ptr<Node>> nodes;

        uint32_t i = 0;
        uint32_t length = m_fields.size();
        for (; i < length; ++i)
        {
            uint32_t column = m_fields.at(i);
            std::shared_ptr<Node> node = table.getSchema().getNodes().at(
                                             column);
            nodes.push_back(node);
        }

        Schema schema(nodes);
        return schema;
    }

    void getRows(db::vector<std::shared_ptr<Row>>& rows)
    {

        db::map<uint32_t, std::shared_ptr<Column>>* columns =
                table.getColumns();
        db::map<uint32_t, std::shared_ptr<PrimitiveValue>>* primitives =
                    table.getPrimitives();

        uint32_t nb_rows = table.num_rows();

        auto it_print = table.getPrinters()->begin();
        auto end_print = table.getPrinters()->end();

        db::vector<std::shared_ptr<Printer>> printers;

        for (; it_print != end_print; ++it_print)
        {
            printers.push_back((*it_print).second);
        }

        uint32_t i = 0;
        for (; i < nb_rows; ++i)
        {
            db::vector<std::shared_ptr<PrimitiveValue>> cells;

            auto it = columns->begin();
            auto end = columns->end();
            for (; it != end; ++it)
            {
                uint32_t column = (*it).first;

                std::shared_ptr<PrimitiveValue> cell = table.getColumns()->at(column)->getThisPrimitive();
                table.getColumns()->at(column)->buffer->getPrimitive(cell, i);

                cells.emplace_back(cell);
            }

            std::shared_ptr<Row> row = std::allocate_shared<Row,
                                 free_list_allocator<uint8_t>>(free_list_alloca,
                                         Row(cells, printers));
            rows.push_back(std::move(row));
        }
    }
}
;

#endif /* SRC_DATASET_H_ */

