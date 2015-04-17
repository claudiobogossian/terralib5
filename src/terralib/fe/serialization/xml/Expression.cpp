/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/fe/serialization/xml/Expression.cpp
   
  \brief Auxiliary classes and functions to serialize filter expressions from a XML document.
*/

// TerraLib
#include "../../../common/Translator.h"
#include "../../../fe/BinaryOperator.h"
#include "../../../fe/Expression.h"
#include "../../../fe/Globals.h"
#include "../../../fe/Literal.h"
#include "../../../fe/PropertyName.h"
#include "../../../xml/AbstractWriter.h"
#include "../../../xml/Reader.h"
#include "../../Exception.h"
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

void te::fe::serialize::Expression::reg(const std::string& expName, const ExpressionReadFnctType& fnct)
{
  m_fncts[expName] = fnct;
}

te::fe::Expression* te::fe::serialize::Expression::read(te::xml::Reader& reader) const
{
  std::string name;
  reader.getNodeType() == te::xml::VALUE ? name = "Literal" : name = reader.getElementLocalName();

  ExpressionFnctIdxType::const_iterator it = m_fncts.find(name);

  if(it == m_fncts.end())
    throw Exception((boost::format(TE_TR("Could not find a reader for the following expression type: %1%.")) % name).str());

  assert(it->second);

  return it->second(reader);
}

void te::fe::serialize::Expression::write(const te::fe::Expression* exp, te::xml::AbstractWriter& writer)
{
  assert(exp);
  assert(&writer);

  if(m_writer != &writer)
    m_writer = &writer;

  exp->accept(*this);
}

void te::fe::serialize::Expression::visit(const te::fe::BinaryOperator& visited)
{
  std::string name = "ogc:";
  name += visited.getName();

  m_writer->writeStartElement(name);

  te::fe::Expression* exp1 = visited.getFirst();
  assert(exp1);
  exp1->accept(*this);

  te::fe::Expression* exp2 = visited.getSecond();
  assert(exp2);
  exp2->accept(*this);

  m_writer->writeEndElement(name);
}

void te::fe::serialize::Expression::visit(const te::fe::Function& visited)
{
  // (???)
}

void te::fe::serialize::Expression::visit(const te::fe::Literal& visited)
{
  m_writer->writeElement("ogc:Literal", visited.getValue());
  //m_writer->writeValue(visited.getValue());
}

void te::fe::serialize::Expression::visit(const te::fe::PropertyName& visited)
{
  m_writer->writeElement("ogc:PropertyName", visited.getName());
}

te::fe::serialize::Expression::~Expression()
{
}

te::fe::serialize::Expression::Expression()
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
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == te::fe::Globals::sm_add ||
         reader.getElementLocalName() == te::fe::Globals::sm_sub ||
         reader.getElementLocalName() == te::fe::Globals::sm_mul ||
         reader.getElementLocalName() == te::fe::Globals::sm_div);

  te::fe::BinaryOperator* op = 0;
  if(reader.getElementLocalName() == te::fe::Globals::sm_add)
    op = new te::fe::BinaryOperator(te::fe::Globals::sm_add);
  else if(reader.getElementLocalName() == te::fe::Globals::sm_sub)
    op = new te::fe::BinaryOperator(te::fe::Globals::sm_sub);
  else if(reader.getElementLocalName() == te::fe::Globals::sm_mul)
    op = new te::fe::BinaryOperator(te::fe::Globals::sm_mul);
  else op = new te::fe::BinaryOperator(te::fe::Globals::sm_div);

  std::auto_ptr<te::fe::BinaryOperator> exp(op);

  reader.next();

  exp->setFirst(te::fe::serialize::Expression::getInstance().read(reader));

  exp->setSecond(te::fe::serialize::Expression::getInstance().read(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return exp.release();
}

te::fe::Expression* LiteralReader(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);

  std::auto_ptr<te::fe::Literal> exp(new te::fe::Literal(""));

  if(reader.getElementLocalName() == "Literal")
  {
    reader.next();
    if(reader.getNodeType() == te::xml::END_ELEMENT)
    {
      reader.next();
    }
    else
    {
      assert(reader.getNodeType() == te::xml::VALUE);
      std::string value = reader.getElementValue();
      exp->setValue(value);
      reader.next();

      assert(reader.getNodeType() == te::xml::END_ELEMENT);
      reader.next();
    }

    return exp.release();
  }

  // Else, no <Literal> tags
  std::string value = reader.getElementValue();
  exp->setValue(value);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return exp.release();
}

te::fe::Expression* PropertyNameReader(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "PropertyName");

  reader.next();
  
  assert(reader.getNodeType() == te::xml::VALUE);
  std::string value = reader.getElementValue();
  std::auto_ptr<te::fe::PropertyName> exp(new te::fe::PropertyName(value));

  reader.next();

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return exp.release();
}
