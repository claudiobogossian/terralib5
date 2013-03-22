/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/dataset/ObjectIdSet.cpp

  \brief This class represents a set of unique ids created in the same context. i.e. from the same data set.
*/

// TerraLib
#include "../query/And.h"
#include "../query/DataSetName.h"
#include "../query/EqualTo.h"
#include "../query/Field.h"
#include "../query/Fields.h"
#include "../query/From.h"
#include "../query/FromItem.h"
#include "../query/Literal.h"
#include "../query/PropertyName.h"
#include "../query/Select.h"
#include "../query/Where.h"
#include "DataSetType.h"
#include "ObjectId.h"
#include "ObjectIdSet.h"

// STL
#include <cassert>

te::da::ObjectIdSet::ObjectIdSet(const te::da::DataSetType* type)
  : m_type(type),
    m_isFromPk(false),
    m_isFromUk(false)
{
}

te::da::ObjectIdSet::~ObjectIdSet()
{
}

const te::da::DataSetType* te::da::ObjectIdSet::getType() const
{
  return m_type;
}

void te::da::ObjectIdSet::addProperty(const std::string& name)
{
  assert(m_type);

  addProperty(m_type->getPropertyPosition(name));
}

void te::da::ObjectIdSet::addProperty(std::size_t i)
{
  m_indexes.push_back(i);
}

void te::da::ObjectIdSet::setProperties(const std::vector<std::string>& names)
{
  assert(m_type);

  std::vector<std::size_t> indexes;

  for(std::size_t i = 0; i < names.size(); ++i)
    indexes.push_back(m_type->getPropertyPosition(names[i]));

  setProperties(indexes);
}

void te::da::ObjectIdSet::setProperties(const std::vector<std::size_t>& indexes)
{
  m_indexes = indexes;
}

void te::da::ObjectIdSet::add(te::da::ObjectId* oid)
{
  assert(oid);

  m_oids.push_back(oid);
}

te::da::Select* te::da::ObjectIdSet::getQuery() const
{
  return 0; // TODO!
}

te::da::Select* te::da::ObjectIdSet::getQuery(std::size_t i) const
{
  assert(m_type);
  assert(i < m_oids.size());
  assert(!m_oids.is_null(i));

  // Gets the requested object id
  const ObjectId& oid = m_oids[i];

  // The list of restrictions to query for the requested object id. i.e. a list of EqualTo
  std::vector<Expression*> expressions;

  /* For each property used to generate the object id
     let's build the restriction expressions */

  // The object id values
  const boost::ptr_vector<te::dt::AbstractData>& data = oid.getValue();
  assert(!data.empty());
  assert(data.size() == m_indexes.size());

  // For each value
  for(std::size_t i = 0; i < data.size(); ++i)
  {
    // What property was used?
    te::dt::Property* p = m_type->getProperty(m_indexes[i]);

    assert(p);
    assert(!p->getName().empty());
    assert(!data.is_null(i));

    // The restriction is an EqualTo expression
    PropertyName* propertyName = new PropertyName(p->getName());
    Literal* value = new Literal(data[i]);

    expressions.push_back(new EqualTo(propertyName, value));
  }

  // Building the Where clause
  Expression* finalRestriction = 0;
  if(!expressions.empty() && expressions.size() == 1)
    finalRestriction = expressions[0];
  else
  {
    // Build the "and" expressions
    finalRestriction = new And(expressions[0], expressions[1]);
    for(std::size_t i = 2; i < expressions.size(); ++i)
    {
      And* and = new And(finalRestriction, expressions[i]);
      finalRestriction = and;
    }
  }
  Where* filter = new Where(finalRestriction);

  // What is the data set?
  assert(!m_type->getName().empty());
  FromItem* fromItem = new DataSetName(m_type->getName());
  From* from = new From;
  from->push_back(fromItem);

  // All fields
  te::da::Fields* all = new te::da::Fields;
  all->push_back(new te::da::Field("*"));

  return new Select(all, from, filter);
}

void te::da::ObjectIdSet::clear()
{
  m_oids.clear();
}

bool te::da::ObjectIdSet::isFromPrimaryKey() const
{
  return m_isFromPk;
}

void te::da::ObjectIdSet::setIsFromPrimaryKey(bool value)
{
  m_isFromPk = value;
}

bool te::da::ObjectIdSet::isFromUniqueKeys() const
{
  return m_isFromUk;
}

void te::da::ObjectIdSet::setIsFromUniqueKeys(bool value)
{
  m_isFromUk = value;
}
