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
  \file terralib/dataaccess/dataset/ObjectIdSet.cpp

  \brief This class represents a set of unique ids created in the same context. i.e. from the same data set.
*/

// TerraLib
#include "../../common/STLUtils.h"
#include "../../common/Translator.h"
#include "../../datatype/Enums.h"
#include "../../geometry/Envelope.h"
#include "../query/And.h"
#include "../query/Expression.h"
#include "../query/Field.h"
#include "../query/In.h"
#include "../query/Literal.h"
#include "../query/LiteralEnvelope.h"
#include "../query/LiteralString.h"
#include "../query/ST_Intersects.h"
#include "../Exception.h"
#include "ObjectId.h"
#include "ObjectIdSet.h"

// STL
#include <cassert>

te::da::ObjectIdSet::ObjectIdSet() : m_expression(0), m_expByClauseIn(false)
{
}

te::da::ObjectIdSet::ObjectIdSet(const ObjectIdSet& rhs, bool copyOids)
  : m_pnames(rhs.m_pnames),
    m_ppos(rhs.m_ppos),
    m_ptypes(rhs.m_ptypes),
    m_expression(0),
    m_expByClauseIn(rhs.m_expByClauseIn)
{
  if(copyOids)
  {
    if(rhs.getExpression())
      m_expression = rhs.getExpression()->clone();

    std::set<ObjectId*, te::common::LessCmp<ObjectId*> >::const_iterator it;
    for(it = rhs.m_oids.begin(); it != rhs.m_oids.end(); ++it)
      m_oids.insert((*it)->clone());
  }
}

te::da::ObjectIdSet::~ObjectIdSet()
{
  te::common::FreeContents(m_oids);

  delete m_expression;
}

void te::da::ObjectIdSet::addProperty(const std::string& name, std::size_t pos, int type)
{
  assert(!name.empty());
  m_pnames.push_back(name);
  m_ppos.push_back(pos);
  m_ptypes.push_back(type);
}

void te::da::ObjectIdSet::add(te::da::ObjectId* oid)
{
  assert(oid);
  m_oids.insert(oid);
}

void te::da::ObjectIdSet::setExpression(te::da::Expression* expression, bool isClauseIn)
{
  delete m_expression;

  m_expression = expression;

  m_expByClauseIn = isClauseIn;
}

void te::da::ObjectIdSet::setExpressionByIntersection(std::string geomAttrName, te::gm::Envelope env, int srid)
{
  te::da::Field* field = new te::da::Field(geomAttrName);
  te::da::LiteralEnvelope* lenv = new te::da::LiteralEnvelope(env, srid);
  te::da::ST_Intersects* intersects = new te::da::ST_Intersects(field->getExpression(), lenv);

  setExpression(intersects, false);
}

void te::da::ObjectIdSet::setExpressionByInClause(const std::string source)
{
  assert(m_pnames.size() == m_ptypes.size());

  Expression* ins = 0;
  Expression* tmp = 0;

  // for each property used to be part of the object identification builds a IN clause
  for(std::size_t i = 0; i < m_pnames.size(); ++i)
  {
    In* in = 0;
    if(source.empty())
      in = new In(m_pnames[i]);
    else
      in = new In(source + "." + m_pnames[i]);

    // for each object in the set include its property value in the IN clause
    std::set<ObjectId*, te::common::LessCmp<ObjectId*> >::const_iterator it;
    for(it = m_oids.begin(); it != m_oids.end(); ++it)
    {
      const boost::ptr_vector<te::dt::AbstractData>& data = (*it)->getValue();

      if(i >= data.size())
        continue;

      if(m_ptypes[i] == te::dt::STRING_TYPE)
        in->add(new LiteralString(data[i].toString()));
      else
        in->add(new Literal(data[i]));
    }

    if(in->getNumArgs() > 0)
    {
      if(i > 0)
      {
        tmp = *ins || *in;
        delete ins;
        delete in;
        ins = tmp;
      }
      else
        ins = in;
    }
  }

  setExpression(ins, true);
}

te::da::Expression* te::da::ObjectIdSet::getExpressionByInClause(const std::string source) const
{
  assert(m_pnames.size() == m_ptypes.size());

  Expression* ins = 0;
  Expression* tmp = 0;

  // for each property used to be part of the object identification builds a IN clause
  for(std::size_t i = 0; i < m_pnames.size(); ++i)
  {
    In* in = 0;
    if(source.empty())
      in = new In(m_pnames[i]);
    else
      in = new In(source + "." + m_pnames[i]);

    // for each object in the set include its property value in the IN clause
    std::set<ObjectId*, te::common::LessCmp<ObjectId*> >::const_iterator it;
    for(it = m_oids.begin(); it != m_oids.end(); ++it)
    {
      const boost::ptr_vector<te::dt::AbstractData>& data = (*it)->getValue();

      if(i >= data.size())
        continue;

      if(m_ptypes[i] == te::dt::STRING_TYPE)
        in->add(new LiteralString(data[i].toString()));
      else
        in->add(new Literal(data[i]));
    }

    if(in->getNumArgs() > 0)
    {
      if(i > 0)
      {
        tmp = *ins || *in;
        delete ins;
        delete in;
        ins = tmp;
      }
      else
        ins = in;
    }
  }

  return ins;
}

te::da::Expression* te::da::ObjectIdSet::getExpression() const
{
  if(m_expression)
    return m_expression->clone();
  else
    return getExpressionByInClause();
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

const std::vector<std::string>& te::da::ObjectIdSet::getPropertyNames() const
{
  return m_pnames;
}

const std::vector<std::size_t>& te::da::ObjectIdSet::getPropertyPos() const
{
  return m_ppos;
}

const std::vector<int>& te::da::ObjectIdSet::getPropertyTypes() const
{
  return m_ptypes;
}

bool te::da::ObjectIdSet::contains(ObjectId* oid) const
{
  assert(oid);
  return m_oids.find(oid) != m_oids.end();
}

void te::da::ObjectIdSet::remove(ObjectId* oid)
{
  std::set<te::da::ObjectId*,  te::common::LessCmp<te::da::ObjectId*> >::iterator it = m_oids.find(oid);

  if(it != m_oids.end())
    m_oids.erase(it);
}

void te::da::ObjectIdSet::Union(te::da::ObjectIdSet* rhs)
{
  assert(rhs);

  std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >& newOids = rhs->m_oids;

  std::set<te::da::ObjectId*,  te::common::LessCmp<te::da::ObjectId*> >::iterator it;
  for(it = newOids.begin(); it != newOids.end(); ++it)
    m_oids.find(*it) == m_oids.end() ? add(*it) : delete *it;

  newOids.clear();

  setExpressionByInClause();

  delete rhs;
  rhs = 0;
}

void te::da::ObjectIdSet::difference(const te::da::ObjectIdSet* rhs)
{
  assert(rhs);

  if(m_oids.empty())
    return;

  const std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >& oidsToRemove = rhs->m_oids;
  
  if(oidsToRemove.empty())
    return;
  
  std::set<te::da::ObjectId*,  te::common::LessCmp<te::da::ObjectId*> >::const_iterator it;
  for(it = oidsToRemove.begin(); it != oidsToRemove.end(); ++it)
  {
    std::set<te::da::ObjectId*,  te::common::LessCmp<te::da::ObjectId*> >::iterator itSearch = m_oids.find(*it);

    if(itSearch == m_oids.end())
      continue;

    delete *itSearch;
    m_oids.erase(itSearch);
  }

  setExpressionByInClause();
}

void te::da::ObjectIdSet::symDifference(const te::da::ObjectIdSet* rhs)
{
  assert(rhs);

  const std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >& oidsToInsert = rhs->m_oids;

  std::set<te::da::ObjectId*,  te::common::LessCmp<te::da::ObjectId*> >::const_iterator it;
  for(it = oidsToInsert.begin(); it != oidsToInsert.end(); ++it)
  {
    std::set<te::da::ObjectId*,  te::common::LessCmp<te::da::ObjectId*> >::iterator itSearch = m_oids.find(*it);

    if(itSearch != m_oids.end())
    {
      delete *itSearch;
      m_oids.erase(itSearch);
    }
    else
      m_oids.insert((*it)->clone());
  }

  setExpressionByInClause();
}

std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator te::da::ObjectIdSet::begin() const
{
  return m_oids.begin();
}

std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator te::da::ObjectIdSet::end() const
{
  return m_oids.end();
}

te::da::ObjectIdSet* te::da::ObjectIdSet::clone() const
{
  return new ObjectIdSet(*this);
}

bool te::da::ObjectIdSet::isExpressionClauseIn() const
{
  if(!m_expression)
    getExpressionByInClause();

  return m_expByClauseIn;
}
