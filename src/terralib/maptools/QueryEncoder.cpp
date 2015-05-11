/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/maptools/QueryEncoder.cpp
  
  \brief A visitor that converts a OGC Filter Expression to TerraLib Expression.
 */

// TerraLib
#include "../dataaccess/query_h.h" 
#include "../fe.h"
#include "../gml/Envelope.h"
#include "QueryEncoder.h"

std::map<std::string, std::string> te::map::QueryEncoder::sm_fnameMap;

te::map::QueryEncoder::QueryEncoder()
  : m_expression(0)
{
}

te::map::QueryEncoder::~QueryEncoder()
{
}

te::da::Expression* te::map::QueryEncoder::getExpression(const te::fe::Filter* f)
{
  te::fe::AbstractOp* ops = f->getOp();
  if(ops)
  {
    ops->accept(*this);
    return m_expression;
  }
  else
  {
    te::da::In* in = new te::da::In(""); // TODO: What is the PropertyName?
    // ids
    size_t nids = f->getNumberOfOids();
    for(size_t i = 0; i < nids; ++i)
    {
      te::fe::ObjectId* objId = f->getOid(i);
      in->add(new te::da::LiteralString(objId->getId()));
    }
    return in;
  }

  return 0;
}

void te::map::QueryEncoder::visit(const te::fe::AbstractOp& /*visited*/)
{
  // no need
}

void te::map::QueryEncoder::visit(const te::fe::SpatialOp& /*visited*/)
{
  // no need
}

void te::map::QueryEncoder::visit(const te::fe::ComparisonOp& /*visited*/)
{ 
  // no need 
}

void te::map::QueryEncoder::visit(const te::fe::LogicOp& /*visited*/)
{
  // no need
}

void te::map::QueryEncoder::visit(const te::fe::BBOXOp& visited)
{
  te::da::Expression* l = 0;
  te::gml::Envelope* e = visited.getEnvelope();
  if(e)
    l = new te::da::LiteralEnvelope(e->getCoordinates(), e->getSRID());

  te::fe::PropertyName* p = visited.getProperty();
  assert(p);
  p->accept(*this);

  te::da::ST_Intersects* f = new te::da::ST_Intersects(l, m_expression);

  m_expression = f;
}

void te::map::QueryEncoder::visit(const te::fe::BinaryComparisonOp& visited)
{
  visited.getFirst()->accept(*this);
  te::da::Expression* e1 = m_expression;

  visited.getSecond()->accept(*this);
  te::da::Expression* e2 = m_expression;

  te::da::Function* f = new te::da::Function(sm_fnameMap[visited.getName()]);
  f->add(e1);
  f->add(e2);

  m_expression = f;
}

void te::map::QueryEncoder::visit(const te::fe::BinaryLogicOp& visited)
{ 
  te::da::Function* f = new te::da::Function(sm_fnameMap[visited.getName()]);
  size_t n = visited.size();
  for(size_t i = 0; i < n; ++i)
  {
    visited.getOp(i)->accept(*this);
    te::da::Expression* e = m_expression;
    f->add(e);
  }
  m_expression = f;
}

void te::map::QueryEncoder::visit(const te::fe::BinarySpatialOp& visited)
{
  te::da::Expression* l = 0;
  te::gml::Envelope* e = visited.getEnvelope();
  if(e)
  {
    l = new te::da::LiteralEnvelope(e->getCoordinates(), e->getSRID());
  }
  else
  {
    te::gm::Geometry* g = visited.getGeometry();
    if(g)
      l = new te::da::LiteralGeom(g);
  }

  te::fe::PropertyName* p = visited.getProperty();
  assert(p);
  p->accept(*this);

  te::da::BinaryFunction* f = new te::da::BinaryFunction(sm_fnameMap[visited.getName()], m_expression, l);

  m_expression = f;
}

void te::map::QueryEncoder::visit(const te::fe::DistanceBuffer& visited)
{
  te::fe::PropertyName* p = visited.getProperty();
  assert(p);
  p->accept(*this);

  te::gm::Geometry* g = visited.getGeometry();
  te::common::Distance* d = visited.getDistance();

  te::da::ST_DistanceBuffer* dbuffer = new te::da::ST_DistanceBuffer(sm_fnameMap[visited.getName()], m_expression, g, d);

  m_expression = dbuffer;
}

void te::map::QueryEncoder::visit(const te::fe::PropertyIsBetween& visited)
{
  visited.getExpression()->accept(*this);
  te::da::Expression* v = m_expression;
  
  visited.getLowerBoundary()->accept(*this);
  te::da::Expression* e1 = m_expression;

  visited.getUpperBoundary()->accept(*this);
  te::da::Expression* e2 = m_expression;

  te::da::GreaterThan* gt = new te::da::GreaterThan(v, e1);
  te::da::LessThan* lt = new te::da::LessThan(v, e2);

  te::da::And* and_op = new te::da::And(gt, lt);

  m_expression = and_op;
}

void te::map::QueryEncoder::visit(const te::fe::PropertyIsLike& visited)
{
  te::fe::Literal* l = visited.getLiteral();
  assert(l);
  std::string pattern = l->getValue();
  std::string wildCard = visited.getWildCard();
  std::string singleChar = visited.getSingleChar();
  std::string escapeChar = visited.getEscapeChar();

  te::fe::PropertyName* p = visited.getPropertyName();
  assert(p);
  p->accept(*this);
  
  te::da::Like* like = new te::da::Like(m_expression, pattern, wildCard, singleChar, escapeChar);

  m_expression = like;
}

void te::map::QueryEncoder::visit(const te::fe::PropertyIsNull& visited)
{
  te::fe::PropertyName* p = visited.getPropertyName();
  assert(p);
  p->accept(*this);

  te::da::IsNull* f = new te::da::IsNull(m_expression);

  m_expression = f;
}

void te::map::QueryEncoder::visit(const te::fe::UnaryLogicOp& visited)
{
  visited.getOp()->accept(*this);

  te::da::UnaryOp* op = new te::da::UnaryOp(sm_fnameMap[visited.getName()], m_expression);

  m_expression = op;
}

void te::map::QueryEncoder::visit(const te::fe::Expression& /*visited*/)
{
  // no need
}

void te::map::QueryEncoder::visit(const te::fe::BinaryOperator& visited)
{
  visited.getFirst()->accept(*this);
  te::da::Expression* e1 = m_expression;

  visited.getSecond()->accept(*this);
  te::da::Expression* e2 = m_expression;

  te::da::BinaryFunction* f = new te::da::BinaryFunction(sm_fnameMap[visited.getName()], e1, e2);

  m_expression = f;
}

void te::map::QueryEncoder::visit(const te::fe::Function& /*visited*/)
{
  // no need
}

void te::map::QueryEncoder::visit(const te::fe::Literal& visited)
{
  te::da::Literal* l = new te::da::LiteralString(visited.getValue());
  m_expression = l;
}

void te::map::QueryEncoder::visit(const te::fe::PropertyName& visited)
{
  te::da::PropertyName* p = new te::da::PropertyName(visited.getName());
  m_expression = p;
}

void te::map::QueryEncoder::initialize()
{
  // BinaryComparisonOp
  sm_fnameMap[te::fe::Globals::sm_propertyIsEqualTo] = te::da::FunctionNames::sm_EqualTo;
  sm_fnameMap[te::fe::Globals::sm_propertyIsGreaterThan] = te::da::FunctionNames::sm_GreaterThan;
  sm_fnameMap[te::fe::Globals::sm_propertyIsGreaterThanOrEqualTo] = te::da::FunctionNames::sm_GreaterThanOrEqualTo;
  sm_fnameMap[te::fe::Globals::sm_propertyIsLessThan] = te::da::FunctionNames::sm_LessThan;
  sm_fnameMap[te::fe::Globals::sm_propertyIsLessThanOrEqualTo] = te::da::FunctionNames::sm_LessThanOrEqualTo;

  // BinaryLogicOp
  sm_fnameMap[te::fe::Globals::sm_and] = te::da::FunctionNames::sm_And;
  sm_fnameMap[te::fe::Globals::sm_or] = te::da::FunctionNames::sm_Or;

  // BinarySpatialOp
  sm_fnameMap[te::fe::Globals::sm_crosses] = te::da::FunctionNames::sm_ST_Crosses;
  sm_fnameMap[te::fe::Globals::sm_disjoint] = te::da::FunctionNames::sm_ST_Disjoint;
  sm_fnameMap[te::fe::Globals::sm_equals] = te::da::FunctionNames::sm_ST_Equals;
  sm_fnameMap[te::fe::Globals::sm_intersects] = te::da::FunctionNames::sm_ST_Intersects;
  sm_fnameMap[te::fe::Globals::sm_overlaps] = te::da::FunctionNames::sm_ST_Overlaps;
  sm_fnameMap[te::fe::Globals::sm_touches] = te::da::FunctionNames::sm_ST_Touches;
  sm_fnameMap[te::fe::Globals::sm_within] = te::da::FunctionNames::sm_ST_Within;

  // DistanceBuffer
  sm_fnameMap[te::fe::Globals::sm_dWithin] = te::da::FunctionNames::sm_ST_DWithin;
  sm_fnameMap[te::fe::Globals::sm_beyond] = te::da::FunctionNames::sm_ST_Beyond;

  // UnaryLogicOp
  sm_fnameMap[te::fe::Globals::sm_not] = te::da::FunctionNames::sm_Not;

  // BinaryOperator
  sm_fnameMap[te::fe::Globals::sm_add] = te::da::FunctionNames::sm_Add;
  sm_fnameMap[te::fe::Globals::sm_sub] = te::da::FunctionNames::sm_Sub;
  sm_fnameMap[te::fe::Globals::sm_mul] = te::da::FunctionNames::sm_Mul;
  sm_fnameMap[te::fe::Globals::sm_div] = te::da::FunctionNames::sm_Div;
}
