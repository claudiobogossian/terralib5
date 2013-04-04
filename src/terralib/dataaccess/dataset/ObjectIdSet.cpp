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
#include "../../common/STLUtils.h"
#include "../../common/Translator.h"
#include "../../datatype/Enums.h"
#include "../query/And.h"
#include "../query/DataSetName.h"
#include "../query/EqualTo.h"
#include "../query/Expression.h"
#include "../query/Field.h"
#include "../query/Fields.h"
#include "../query/From.h"
#include "../query/FromItem.h"
#include "../query/In.h"
#include "../query/Literal.h"
#include "../query/LiteralString.h"
#include "../query/PropertyName.h"
#include "../query/Select.h"
#include "../query/Where.h"
#include "../Exception.h"
#include "DataSetType.h"
#include "ObjectId.h"
#include "ObjectIdSet.h"

// STL
#include <cassert>
#include <memory>

te::da::ObjectIdSet::ObjectIdSet(const te::da::DataSetType* type)
  : m_type(static_cast<DataSetType*>(type->clone()))
{
}

te::da::ObjectIdSet::~ObjectIdSet()
{
  delete m_type;
  te::common::FreeContents(m_oids);
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

  m_oids.insert(oid);
}

te::da::Select* te::da::ObjectIdSet::getQuery() const
{
  assert(m_type);
  
  Expression* ins = 0;
  Expression* tmp = 0;
  
  // for each property used to be part of the object identification builds a IN clause.
  for(std::size_t i = 0; i < m_indexes.size(); ++i)
  {
    const std::string& propertyName = m_type->getProperty(m_indexes[i])->getName();
    
    In* in = new In(propertyName);

    // for each object in the set include its property value in the IN clause
    std::set<ObjectId*, te::common::LessCmp<ObjectId*> >::const_iterator it;
    for(it = m_oids.begin(); it != m_oids.end(); ++it)
    {
      const boost::ptr_vector<te::dt::AbstractData>& data = (*it)->getValue();

      if(m_type->getProperty(m_indexes[i])->getType() == te::dt::STRING_TYPE)
        in->add(new LiteralString(data[i].toString()));
      else
        in->add(new Literal(data[i]));
    }

    if(i > 0)
    {
      tmp = *ins && *in;
      delete ins;
      delete in;
      ins = tmp;
    }
    else 
      ins = in;
  }
  
  // filter is one or more IN clauses
  Where* filter = new Where(ins);
  
  // all fields
  te::da::Fields* all = new te::da::Fields;
  all->push_back(new te::da::Field("*"));
  
  // from the data set?
  FromItem* fromItem = new DataSetName(m_type->getName());
  From* from = new From;
  from->push_back(fromItem);
  
  return new Select(all, from, filter);
}

void te::da::ObjectIdSet::clear()
{
  te::common::FreeContents(m_oids);
  m_oids.clear();
}

std::size_t te::da::ObjectIdSet::size() const
{
  return m_oids.size();
}

void te::da::ObjectIdSet::Union(te::da::ObjectIdSet* rhs)
{
  assert(rhs);

  std::auto_ptr<ObjectIdSet*> toDelete(&rhs);

  std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >& newOids = rhs->m_oids;

  std::set<te::da::ObjectId*,  te::common::LessCmp<te::da::ObjectId*> >::iterator it;
  for(it = newOids.begin(); it != newOids.end(); ++it)
    m_oids.find(*it) == m_oids.end() ? add(*it) : delete *it;

  newOids.clear();
}

void te::da::ObjectIdSet::difference(te::da::ObjectIdSet* rhs)
{
  assert(rhs);

  std::auto_ptr<ObjectIdSet*> toDelete(&rhs);

  if(m_oids.empty())
    return;

  std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >& oidsToRemove = rhs->m_oids;
  
  if(oidsToRemove.empty())
    return;
  
  std::set<te::da::ObjectId*,  te::common::LessCmp<te::da::ObjectId*> >::iterator it;
  for(it = oidsToRemove.begin(); it != oidsToRemove.end(); ++it)
  {
    std::set<te::da::ObjectId*,  te::common::LessCmp<te::da::ObjectId*> >::iterator itSearch = m_oids.find(*it);

    if(itSearch == m_oids.end())
      continue;

    delete *itSearch;
    m_oids.erase(itSearch);
  }
}
