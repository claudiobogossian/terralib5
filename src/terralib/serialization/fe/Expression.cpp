/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/serialization/fe/Expression.cpp
   
  \brief Auxiliary classes and functions to serialize filter expressions from a XML document.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../fe/BinaryOperator.h"
#include "../../fe/Expression.h"
#include "../../fe/Globals.h"
#include "../../fe/Literal.h"
#include "../../fe/PropertyName.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../Exception.h"
#include "Expression.h"

// STL
#include <cassert>

// Boost
#include <boost/format.hpp>

/* @name Expression Reader Methods */
//@{

te::fe::Expression* BinaryOperatorReader(te::xml::Reader& reader);
te::fe::Expression* LiteralReader(te::xml::Reader& reader);
te::fe::Expression* PropertyNameReader(te::xml::Reader& reader);

//@}

void te::serialize::Expression::reg(const std::string& expName, const ExpressionReadFnctType& fnct)
{
  m_fncts[expName] = fnct;
}

te::fe::Expression* te::serialize::Expression::read(te::xml::Reader& reader) const
{
  std::string name = reader.getElementLocalName();

  ExpressionFnctIdxType::const_iterator it = m_fncts.find(name);

  if(it == m_fncts.end())
    throw Exception((boost::format(TR_SERIALIZATION("Could not find a reader for the following expression type: %1%.")) % name).str());

  assert(it->second);

  return it->second(reader);
}

void te::serialize::Expression::write(const te::fe::Expression* exp, te::xml::Writer& writer)
{
  assert(exp);
  assert(&writer);

  if(m_writer != &writer)
    m_writer = &writer;

  exp->accept(*this);
}

void te::serialize::Expression::visit(const te::fe::BinaryOperator& visited)
{
  m_writer->writeStartElement(visited.getName());

  te::fe::Expression* exp1 = visited.getFirst();
  assert(exp1);
  exp1->accept(*this);

  te::fe::Expression* exp2 = visited.getSecond();
  assert(exp2);
  exp2->accept(*this);

  m_writer->writeEndElement(visited.getName());
}

void te::serialize::Expression::visit(const te::fe::Function& visited)
{
  // (???)
}

void te::serialize::Expression::visit(const te::fe::Literal& visited)
{
  //m_writer->writeElement("Literal", visited.getValue());
  m_writer->writeValue(visited.getValue());
}

void te::serialize::Expression::visit(const te::fe::PropertyName& visited)
{
  m_writer->writeElement("PropertyName", visited.getName());
}

te::serialize::Expression::~Expression()
{
}

te::serialize::Expression::Expression()
  : m_writer(0)
{
  // Binary Operators
  m_fncts[te::fe::Globals::sm_add] = ExpressionReadFnctType(&BinaryOperatorReader);
  m_fncts[te::fe::Globals::sm_sub] = ExpressionReadFnctType(&BinaryOperatorReader);
  m_fncts[te::fe::Globals::sm_mul] = ExpressionReadFnctType(&BinaryOperatorReader);
  m_fncts[te::fe::Globals::sm_div] = ExpressionReadFnctType(&BinaryOperatorReader);

  // Functions (???)

  // Literal
  m_fncts["Literal"] = ExpressionReadFnctType(&LiteralReader);

  // PropertyName
  m_fncts["PropertyName"] = ExpressionReadFnctType(&PropertyNameReader);
}

te::fe::Expression* BinaryOperatorReader(te::xml::Reader& reader)
{
  return 0;
}

te::fe::Expression* LiteralReader(te::xml::Reader& reader)
{
  return 0;
}

te::fe::Expression* PropertyNameReader(te::xml::Reader& reader)
{
  return 0;
}
