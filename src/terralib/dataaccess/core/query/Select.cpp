/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/dataaccess/core/query/Select.cpp

  \brief A Select models a query to be used when retrieving data from a data source.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "DataSetName.h"
#include "Distinct.h"
#include "Expression.h"
#include "Field.h"
#include "FromItem.h"
#include "GroupByItem.h"
#include "Having.h"
#include "OrderByItem.h"
#include "PropertyName.h"
#include "Select.h"
#include "Where.h"

te::da::core::Select::Select()
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
}

te::da::core::Select::Select(Fields* fds, From* f, Where* w, OrderBy* o)
  : m_fields(fds),
    m_from(f),
    m_where(w),
    m_groupBy(0),
    m_having(0),
    m_orderBy(o),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
}

te::da::core::Select::Select(const Fields& fds)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields(fds));
}

te::da::core::Select::Select(const Fields& fds, const From& f)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields(fds));
  m_from.reset(new From(f));
}

te::da::core::Select::Select(const Fields& fds, const From& f, const Where& w)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields(fds));
  m_from.reset(new From(f));
  m_where.reset(new Where(w));
}

te::da::core::Select::Select(const Fields& fds, const From& f, const Where& w, const OrderBy& o)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields(fds));
  m_from.reset(new From(f));
  m_where.reset(new Where(w));
  m_orderBy.reset(new OrderBy(o));
}

te::da::core::Select::Select(Fields* fds, From* f, OrderBy* o)
  : m_fields(fds),
    m_from(f),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(o),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
}

te::da::core::Select::Select(const Fields& fds, const From& f, const OrderBy& o)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields(fds));
  m_from.reset(new From(f));
  m_orderBy.reset(new OrderBy(o));
}


te::da::core::Select::Select(Fields* fds, From* f, Where* w, GroupBy* gb, OrderBy* o)
  : m_fields(fds),
    m_from(f),
    m_where(w),
    m_groupBy(gb),
    m_having(0),
    m_orderBy(o),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
}

te::da::core::Select::Select(const Fields& fds, const From& f, const Where& w, const GroupBy& gb)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields(fds));
  m_from.reset(new From(f));
  m_where.reset(new Where(w));
  m_groupBy.reset(new GroupBy(gb));
}

te::da::core::Select::Select(const Fields& fds, const From& f, const Where& w, const GroupBy& gb, const OrderBy& o)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields(fds));
  m_from.reset(new From(f));
  m_where.reset(new Where(w));
  m_groupBy.reset(new GroupBy(gb));
  m_orderBy.reset(new OrderBy(o));
}

te::da::core::Select::Select(Fields* fds, From* f, GroupBy* gb, OrderBy* o)
  : m_fields(fds),
    m_from(f),
    m_where(0),
    m_groupBy(gb),
    m_having(0),
    m_orderBy(o),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
}

te::da::core::Select::Select(const Fields& fds, const From& f, const GroupBy& gb)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields(fds));
  m_from.reset(new From(f));
  m_groupBy.reset(new GroupBy(gb));
}

te::da::core::Select::Select(const Fields& fds, const From& f, const GroupBy& gb, const OrderBy& o)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields(fds));
  m_from.reset(new From(f));
  m_groupBy.reset(new GroupBy(gb));
  m_orderBy.reset(new OrderBy(o));
}

te::da::core::Select::Select(Fields* fds, From* f, Where* w, GroupBy* gb, Having* h, OrderBy* o)
  : m_fields(fds),
    m_from(f),
    m_where(w),
    m_groupBy(gb),
    m_having(h),
    m_orderBy(o),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
}

te::da::core::Select::Select(const Fields& fds, const From& f, const Where& w, const GroupBy& gb, const Having& h)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields(fds));
  m_from.reset(new From(f));
  m_where.reset(new Where(w));
  m_groupBy.reset(new GroupBy(gb));
  m_having.reset(new Having(h));
}

te::da::core::Select::Select(const Fields& fds, const From& f, const Where& w, const GroupBy& gb, const Having& h, const OrderBy& o)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields(fds));
  m_from.reset(new From(f));
  m_where.reset(new Where(w));
  m_groupBy.reset(new GroupBy(gb));
  m_having.reset(new Having(h));
  m_orderBy.reset(new OrderBy(o));
}

te::da::core::Select::Select(Field* f)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields);
  m_fields->push_back(f);
}

te::da::core::Select::Select(const Field& f)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields);
  m_fields->push_back(new Field(f));
}

te::da::core::Select::Select(const std::string& propertyName)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields);
  m_fields->push_back(new Field(propertyName));
}

te::da::core::Select::Select(const std::string& propertyName, const std::string& alias)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(0),
    m_offset(0)
{
  m_fields.reset(new Fields);
  m_fields->push_back(new Field(propertyName, alias));
}

te::da::core::Select::Select(const Select& rhs)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(rhs.m_limit),
    m_offset(rhs.m_offset)
{
  m_fields.reset(rhs.m_fields.get() ? new Fields(*rhs.m_fields) : 0);
  m_from.reset(rhs.m_from.get() ? new From(*rhs.m_from) : 0);
  m_where.reset(rhs.m_where.get() ? new Where(*rhs.m_where) : 0);
  m_groupBy.reset(rhs.m_groupBy.get() ? new GroupBy(*rhs.m_groupBy) : 0);
  m_having.reset(rhs.m_having.get() ? new Having(*rhs.m_having) : 0);
  m_orderBy.reset(rhs.m_orderBy.get() ? new OrderBy(*rhs.m_orderBy) : 0);
  m_distinct.reset(rhs.m_distinct.get() ? new Distinct(*rhs.m_distinct) : 0);
}

te::da::core::Select::Select(const Select* rhs)
  : m_fields(0),
    m_from(0),
    m_where(0),
    m_groupBy(0),
    m_having(0),
    m_orderBy(0),
    m_distinct(0),
    m_limit(rhs->m_limit),
    m_offset(rhs->m_offset)
{
  m_fields.reset(rhs->m_fields.get() ? new Fields(*rhs->m_fields) : 0);
  m_from.reset(rhs->m_from.get() ? new From(*rhs->m_from) : 0);
  m_where.reset(rhs->m_where.get() ? new Where(*rhs->m_where) : 0);
  m_groupBy.reset(rhs->m_groupBy.get() ? new GroupBy(*rhs->m_groupBy) : 0);
  m_having.reset(rhs->m_having.get() ? new Having(*rhs->m_having) : 0);
  m_orderBy.reset(rhs->m_orderBy.get() ? new OrderBy(*rhs->m_orderBy) : 0);
  m_distinct.reset(rhs->m_distinct.get() ? new Distinct(*rhs->m_distinct) : 0);
}

te::da::core::Select::~Select()
{
}

te::da::core::Select& te::da::core::Select::operator=(const Select& rhs)
{
  if(this != &rhs)
  {
    m_fields.reset(rhs.m_fields.get() ? new Fields(*rhs.m_fields) : 0);
    m_from.reset(rhs.m_from.get() ? new From(*rhs.m_from) : 0);
    m_where.reset(rhs.m_where.get() ? new Where(*rhs.m_where) : 0);
    m_groupBy.reset(rhs.m_groupBy.get() ? new GroupBy(*rhs.m_groupBy) : 0);
    m_having.reset(rhs.m_having.get() ? new Having(*rhs.m_having) : 0);
    m_orderBy.reset(rhs.m_orderBy.get() ? new OrderBy(*rhs.m_orderBy) : 0);
    m_distinct.reset(rhs.m_distinct.get() ? new Distinct(*rhs.m_distinct) : 0);
    m_limit = rhs.m_limit;
    m_offset = rhs.m_offset;
  }

  return *this;
}

te::da::core::Query* te::da::core::Select::clone() const
{
  return new Select(*this);
}

te::da::core::Select& te::da::core::Select::fields(const Fields& f)
{
  m_fields.reset(new Fields(f));
  return *this;
}

te::da::core::Select& te::da::core::Select::fields(Fields* f)
{
  m_fields.reset(f);
  return *this;
}

te::da::core::Fields& te::da::core::Select::fields()
{
  return *m_fields;
}

const te::da::core::Fields& te::da::core::Select::fields() const
{
  return *m_fields;
}

te::da::core::Select& te::da::core::Select::operator()(const Field& f)
{
  assert(m_fields.get() != 0);

  m_fields->push_back(new Field(f));

  return *this;
}

te::da::core::Select& te::da::core::Select::operator()(Field* f)
{
  assert(m_fields.get() != 0);

  m_fields->push_back(f);

  return *this;
}

te::da::core::Select& te::da::core::Select::operator()(const std::string& propertyName)
{
  assert(m_fields.get() != 0);

  m_fields->push_back(new Field(propertyName));

  return *this;
}

te::da::core::Select& te::da::core::Select::operator()(const std::string& propertyName, const std::string& alias)
{
  assert(m_fields.get() != 0);

  m_fields->push_back(new Field(propertyName, alias));

  return *this;
}

te::da::core::Select& te::da::core::Select::from(const FromItem& item)
{
  if(m_from.get() == 0)
    m_from.reset(new From);

  m_from->push_back(item.clone());

  return *this;
}

te::da::core::Select& te::da::core::Select::from(FromItem* item)
{
  if(m_from.get() == 0)
    m_from.reset(new From);

  m_from->push_back(item);

  return *this;
}

te::da::core::Select& te::da::core::Select::from(const FromItem& i1, const FromItem& i2)
{
  if(m_from.get() == 0)
    m_from.reset(new From);

  m_from->push_back(i1.clone());
  m_from->push_back(i2.clone());

  return *this;
}

te::da::core::Select& te::da::core::Select::from(FromItem* i1, FromItem* i2)
{
  if(m_from.get() == 0)
    m_from.reset(new From);

  m_from->push_back(i1);
  m_from->push_back(i2);

  return *this;
}

te::da::core::Select& te::da::core::Select::from(const FromItem& i1, const FromItem& i2, const FromItem& i3)
{
  if(m_from.get() == 0)
    m_from.reset(new From);

  m_from->push_back(i1.clone());
  m_from->push_back(i2.clone());
  m_from->push_back(i3.clone());

  return *this;
}

te::da::core::Select& te::da::core::Select::from(FromItem* i1, FromItem* i2, FromItem* i3)
{
  if(m_from.get() == 0)
    m_from.reset(new From);

  m_from->push_back(i1);
  m_from->push_back(i2);
  m_from->push_back(i3);

  return *this;
}

te::da::core::Select& te::da::core::Select::from(const std::string& datasetName)
{
  if(m_from.get() == 0)
    m_from.reset(new From);

  m_from->push_back(new DataSetName(datasetName));

  return *this;
}

te::da::core::Select& te::da::core::Select::from(From* f)
{
  m_from.reset(f);
  return *this;
}

te::da::core::Select& te::da::core::Select::from(const From& rhs)
{
  m_from.reset(new From(rhs));
  return *this;
}
        
te::da::core::From& te::da::core::Select::from()
{
  return *m_from;
}

const te::da::core::From& te::da::core::Select::from() const
{
  return *m_from;
}

te::da::core::Select& te::da::core::Select::Join(FromItem* /*d2*/, JoinType /*t*/, JoinCondition* /*c*/)
{
  return *this;
}

te::da::core::Select& te::da::core::Select::Join(const FromItem& /*d2*/, JoinType /*t*/, const JoinCondition& /*c*/)
{
  return *this;
}

te::da::core::Select& te::da::core::Select::InnerJoin(FromItem* /*d2*/, JoinCondition* /*c*/)
{
  return *this;
}

te::da::core::Select& te::da::core::Select::InnerJoin(const FromItem& /*d2*/, const JoinCondition& /*c*/)
{
  return *this;
}

te::da::core::Select& te::da::core::Select::LeftJoin(FromItem* /*d2*/, JoinCondition* /*c*/)
{
  return *this;
}

te::da::core::Select& te::da::core::Select::LeftJoin(const FromItem& /*d2*/, const JoinCondition& /*c*/)
{
  return *this;
}

te::da::core::Select& te::da::core::Select::RightJoin(FromItem* /*d2*/, JoinCondition* /*c*/)
{
  return *this;
}

te::da::core::Select& te::da::core::Select::RightJoin(const FromItem& /*d2*/, const JoinCondition& /*c*/)
{
  return *this;
}

te::da::core::Select& te::da::core::Select::FullOuterJoin(FromItem* /*d2*/, JoinCondition* /*c*/)
{
  return *this;
}

te::da::core::Select& te::da::core::Select::FullOuterJoin(const FromItem& /*d2*/, const JoinCondition& /*c*/)
{
  return *this;
}

te::da::core::Select& te::da::core::Select::CrossJoin(FromItem* /*d2*/)
{
  return *this;
}

te::da::core::Select& te::da::core::Select::CrossJoin(const FromItem& /*d2*/)
{
  return *this;
}

te::da::core::Select& te::da::core::Select::NaturalJoin(FromItem* /*d2*/, JoinType /*t*/)
{
  return *this;
}

te::da::core::Select& te::da::core::Select::NaturalJoin(const FromItem& /*d2*/, JoinType /*t*/)
{
  return *this;
}

te::da::core::Select& te::da::core::Select::where(Expression* e)
{
  if(m_where.get() == 0)
    m_where.reset(new Where(e));
  else
    m_where->setExp(e);

  return *this;
}

te::da::core::Select& te::da::core::Select::where(const Expression& e)
{
  if(m_where.get() == 0)
    m_where.reset(new Where(e));
  else
    m_where->setExp(e.clone());

  return *this;
}

te::da::core::Select& te::da::core::Select::where(Where* w)
{
  m_where.reset(w);
  return *this;
}

te::da::core::Select& te::da::core::Select::where(const Where& rhs)
{
  m_where.reset(new Where(rhs));
  return *this;
}

te::da::core::Where& te::da::core::Select::where()
{
  return *m_where;
}

const te::da::core::Where& te::da::core::Select::where() const
{
  return *m_where;
}

te::da::core::Select& te::da::core::Select::groupBy(const GroupByItem& item)
{
  if(m_groupBy.get() == 0)
    m_groupBy.reset(new GroupBy);

  m_groupBy->push_back(new GroupByItem(item));

  return *this;
}

te::da::core::Select& te::da::core::Select::groupBy(GroupByItem* item)
{
  if(m_groupBy.get() == 0)
    m_groupBy.reset(new GroupBy);

  m_groupBy->push_back(item);

  return *this;
}

te::da::core::Select& te::da::core::Select::groupBy(const Expression& e)
{
  if(m_groupBy.get() == 0)
    m_groupBy.reset(new GroupBy);

  m_groupBy->push_back(new GroupByItem(e));

  return *this;
}

te::da::core::Select& te::da::core::Select::groupBy(const std::string& propertyName)
{
  if(m_groupBy.get() == 0)
    m_groupBy.reset(new GroupBy);

  m_groupBy->push_back(new GroupByItem(new PropertyName(propertyName)));

  return *this;
}

te::da::core::Select& te::da::core::Select::groupBy(GroupBy* gb)
{
  m_groupBy.reset(gb);
  return *this;
}

te::da::core::Select& te::da::core::Select::groupBy(const GroupBy& rhs)
{
  m_groupBy.reset(new GroupBy(rhs));
  return *this;
}

te::da::core::GroupBy& te::da::core::Select::groupBy()
{
  return *m_groupBy;
}

const te::da::core::GroupBy& te::da::core::Select::groupBy() const
{
  return *m_groupBy;
}

te::da::core::Select& te::da::core::Select::having(Expression* e)
{
  if(m_having.get() == 0)
    m_having.reset(new Having(e));
  else
    m_having->setExp(e);

  return *this;
}

te::da::core::Select& te::da::core::Select::having(const Expression& e)
{
  if(m_having.get() == 0)
    m_having.reset(new Having(e));
  else
    m_having->setExp(e.clone());

  return *this;
}

te::da::core::Select& te::da::core::Select::having(Having* h)
{
  m_having.reset(h);
  return *this;
}

te::da::core::Select& te::da::core::Select::having(const Having& rhs)
{
  m_having.reset(new Having(rhs));
  return *this;
}

te::da::core::Having& te::da::core::Select::having()
{
  return *m_having;
}

const te::da::core::Having& te::da::core::Select::having() const
{
  return *m_having;
}

te::da::core::Select& te::da::core::Select::orderBy(const OrderByItem& item)
{
  if(m_orderBy.get() == 0)
    m_orderBy.reset(new OrderBy);

  m_orderBy->push_back(new OrderByItem(item));

  return *this;
}

te::da::core::Select& te::da::core::Select::orderBy(OrderByItem* item)
{
  if(m_orderBy.get() == 0)
    m_orderBy.reset(new OrderBy);

  m_orderBy->push_back(item);

  return *this;
}

te::da::core::Select& te::da::core::Select::orderBy(const Expression& e, SortOrder o)
{
  if(m_orderBy.get() == 0)
    m_orderBy.reset(new OrderBy);

  m_orderBy->push_back(new OrderByItem(e, o));

  return *this;
}

te::da::core::Select& te::da::core::Select::orderBy(const std::string& propertyName, SortOrder o )
{
  if(m_orderBy.get() == 0)
    m_orderBy.reset(new OrderBy);

  m_orderBy->push_back(new OrderByItem(new PropertyName(propertyName), o));

  return *this;
}

te::da::core::Select& te::da::core::Select::orderBy(OrderBy* o)
{
  m_orderBy.reset(o);
  return *this;
}

te::da::core::Select& te::da::core::Select::orderBy(const OrderBy& rhs)
{
  m_orderBy.reset(new OrderBy(rhs));
  return *this;
}

te::da::core::OrderBy& te::da::core::Select::orderBy()
{
  return *m_orderBy;
}

const te::da::core::OrderBy& te::da::core::Select::orderBy() const
{
  return *m_orderBy;
}

te::da::core::Select& te::da::core::Select::distinct(Expression* e)
{
  if(m_distinct.get() == 0)
    m_distinct.reset(new Distinct);

  m_distinct->push_back(e);

  return *this;
}

te::da::core::Select& te::da::core::Select::distinct(const Expression& e)
{
  if(m_distinct.get() == 0)
    m_distinct.reset(new Distinct);

  m_distinct->push_back(e.clone());

  return *this;
}

te::da::core::Select& te::da::core::Select::distinct(const std::string& propertyName)
{
  if(m_distinct.get() == 0)
    m_distinct.reset(new Distinct);

  m_distinct->push_back(new PropertyName(propertyName));

  return *this;
}

te::da::core::Select& te::da::core::Select::distinct(Distinct* d)
{
  m_distinct.reset(d);
  return *this;
}

te::da::core::Select& te::da::core::Select::distinct(const Distinct& rhs)
{
  m_distinct.reset(new Distinct(rhs));
  return *this;
}

te::da::core::Distinct& te::da::core::Select::distinct()
{
  return *m_distinct;
}

const te::da::core::Distinct& te::da::core::Select::distinct() const
{
  return *m_distinct;
}

te::da::core::Select& te::da::core::Select::limit(std::size_t l)
{
  m_limit = l;
  return *this;
}

te::da::core::Select& te::da::core::Select::offset(std::size_t i)
{
  m_offset = i;
  return *this;
}

void te::da::core::Select::setFields(Fields* f)
{
  m_fields.reset(f);
}

const te::da::core::Fields* te::da::core::Select::getFields() const
{
  return m_fields.get();
}

void te::da::core::Select::setFrom(From* f)
{
  m_from.reset(f);
}

const te::da::core::From* te::da::core::Select::getFrom() const
{
  return m_from.get();
}

void te::da::core::Select::setWhere(Where* w)
{
  m_where.reset(w);
}

te::da::core::Where* te::da::core::Select::getWhere() const
{
  return m_where.get();
}

void te::da::core::Select::setGroupBy(GroupBy* g)
{
  m_groupBy.reset(g);
}

const te::da::core::GroupBy* te::da::core::Select::getGroupBy() const
{
  return m_groupBy.get();
}

void te::da::core::Select::setHaving(Having* h)
{
  m_having.reset(h);
}

const te::da::core::Having* te::da::core::Select::getHaving() const
{
  return m_having.get();
}

void te::da::core::Select::setOrderBy(OrderBy* o)
{
  m_orderBy.reset(o);
}

const te::da::core::OrderBy* te::da::core::Select::getOrderBy() const
{
  return m_orderBy.get();
}

void te::da::core::Select::setDistinct(Distinct* d)
{
  m_distinct.reset(d);
}

const te::da::core::Distinct* te::da::core::Select::getDistinct() const
{
  return m_distinct.get();
}

void te::da::core::Select::setLimit(std::size_t m)
{
  m_limit = m;
}

std::size_t te::da::core::Select::getLimit() const
{
  return m_limit;
}

void te::da::core::Select::setOffset(std::size_t o)
{
  m_offset = o;
}

std::size_t te::da::core::Select::getOffset() const
{
  return m_offset;
}

te::da::core::Select& te::da::core::Select::operator+(const te::da::core::From& f)
{
  from(f);
  return *this;
}

te::da::core::Select& te::da::core::Select::operator+(te::da::core::From* f)
{
  from(f);
  return *this;
}

te::da::core::Select& te::da::core::Select::operator+(const te::da::core::Where& w) 
{
  where(w);
  return *this;
}

te::da::core::Select& te::da::core::Select::operator+(te::da::core::Where* w) 
{
  where(w);
  return *this;
}

te::da::core::Select& te::da::core::Select::operator+(const te::da::core::GroupBy& g)
{
  groupBy(g);
  return *this;
}

te::da::core::Select& te::da::core::Select::operator+(te::da::core::GroupBy* g)
{
  groupBy(g);
  return *this;
}

te::da::core::Select& te::da::core::Select::operator+(const te::da::core::Having& h)
{
  having(h);
  return *this;
}

te::da::core::Select& te::da::core::Select::operator+(te::da::core::Having* h)
{
  having(h);
  return *this;
}

te::da::core::Select& te::da::core::Select::operator+(const te::da::core::OrderBy& o)
{
  orderBy(o);
  return *this;
}

te::da::core::Select& te::da::core::Select::operator+(te::da::core::OrderBy* o)
{
  orderBy(o);
  return *this;
}
