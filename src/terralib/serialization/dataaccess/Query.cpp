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
  \file terralib/serialization/dataaccess/Query.cpp
 
  \brief Support for Query serialization.
*/

// TerraLib
#include "../../dataaccess/query/Distinct.h"
#include "../../dataaccess/query/Expression.h"
#include "../../dataaccess/query/Field.h"
#include "../../dataaccess/query/FromItem.h"
#include "../../dataaccess/query/Function.h"
#include "../../dataaccess/query/GroupByItem.h"
#include "../../dataaccess/query/Having.h"
#include "../../dataaccess/query/Literal.h"
#include "../../dataaccess/query/OrderByItem.h"
#include "../../dataaccess/query/PropertyName.h"
#include "../../dataaccess/query/Select.h"
#include "../../dataaccess/query/Where.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../Exception.h"
#include "Query.h"

// STL
#include <cassert>
#include <memory>

te::da::Distinct* te::serialize::ReadDistinct(te::xml::Reader& reader)
{
  throw te::serialize::Exception("Not implemented yet!");
}

te::da::Expression* te::serialize::ReadExpression(te::xml::Reader& reader)
{
  throw te::serialize::Exception("Not implemented yet!");
}

te::da::Field* te::serialize::ReadField(te::xml::Reader& reader)
{
  throw te::serialize::Exception("Not implemented yet!");
}

te::da::Fields* te::serialize::ReadFields(te::xml::Reader& reader)
{
  throw te::serialize::Exception("Not implemented yet!");
}

te::da::From* te::serialize::ReadFrom(te::xml::Reader& reader)
{
  throw te::serialize::Exception("Not implemented yet!");
}

te::da::Function* te::serialize::ReadFunction(te::xml::Reader& reader)
{
  throw te::serialize::Exception("Not implemented yet!");
}

te::da::GroupBy* te::serialize::ReadGroupBy(te::xml::Reader& reader)
{
  throw te::serialize::Exception("Not implemented yet!");
}

te::da::Having* te::serialize::ReadHaving(te::xml::Reader& reader)
{
  throw te::serialize::Exception("Not implemented yet!");
}

te::da::Literal* te::serialize::ReadLiteral(te::xml::Reader& reader)
{
  throw te::serialize::Exception("Not implemented yet!");
}

te::da::OrderBy* te::serialize::ReadOrderBy(te::xml::Reader& reader)
{
  throw te::serialize::Exception("Not implemented yet!");
}

te::da::PropertyName* te::serialize::ReadPropertyName(te::xml::Reader& reader)
{
  throw te::serialize::Exception("Not implemented yet!");
}

te::da::Select* te::serialize::ReadSelect(te::xml::Reader& reader)
{
  throw te::serialize::Exception("Not implemented yet!");
}

te::da::Where* te::serialize::ReadWhere(te::xml::Reader& reader)
{
  throw te::serialize::Exception("Not implemented yet!");
}

void te::serialize::Save(const te::da::Distinct* distinct, te::xml::Writer& writer)
{
  assert(distinct);
  writer.writeStartElement("te_da:Distinct");

  for(std::size_t i = 0; i < distinct->size(); ++i)
  {
    const te::da::Expression& exp = distinct->at(i);

    Save(&exp, writer);
  }

  writer.writeEndElement("te_da:Distinct");
}

void te::serialize::Save(const te::da::Expression* expression, te::xml::Writer& writer)
{
  assert(expression);
  std::auto_ptr<te::da::Expression> exp(expression->clone());

  // Check Expression Type
  std::auto_ptr<te::da::PropertyName> pName(dynamic_cast<te::da::PropertyName*>(exp.get()));
  std::auto_ptr<te::da::Function> func(dynamic_cast<te::da::Function*>(exp.get()));
  std::auto_ptr<te::da::Literal> lit(dynamic_cast<te::da::Literal*>(exp.get()));

  if(pName.get())
    Save(pName.get(), writer);
  else if(func.get())
    Save(func.get(), writer);
  else if(lit.get())
    Save(lit.get(), writer);
  else
    throw te::serialize::Exception("Error: Expression Type Indefined!");
}

void te::serialize::Save(const te::da::Field* field, te::xml::Writer& writer)
{
  assert(field);
  writer.writeStartElement("te_da:Field");

  Save(field->getExpression(), writer);

  writer.writeStartElement("te_da:Alias");

  writer.writeValue(*field->getAlias());

  writer.writeEndElement("te_da:Alias");

  writer.writeEndElement("te_da:Field");

}

void te::serialize::Save(const te::da::Fields* fields, te::xml::Writer& writer)
{
  assert(fields);
  writer.writeStartElement("te_da:Fields");

  for(std::size_t i = 0; i < fields->size(); ++i)
  {
    const te::da::Field& item = fields->at(i);

    Save(&item, writer);
  }

  writer.writeEndElement("te_da:Fields");
}

void te::serialize::Save(const te::da::From* from, te::xml::Writer& writer)
{
  writer.writeStartElement("te_da:From");

  for(std::size_t i = 0; i < from->size(); ++i)
  {
    const te::da::FromItem& item= from->at(i);

    Save(&item, writer);
  }

  writer.writeEndElement("te_da:From");
}

void te::serialize::Save(const te::da::FromItem* fromItem, te::xml::Writer& writer)
{
  writer.writeStartElement("te_da:FromItem");

  writer.writeValue(fromItem->getAlias());

  writer.writeEndElement("te_da:FromItem");
}

void te::serialize::Save(const te::da::Function* func, te::xml::Writer& writer)
{
  assert(func);
  writer.writeStartElement("te_da:Expression");
  writer.writeAttribute("Type", "Function");

  

  writer.writeEndElement("te_da:Expression");
}

void te::serialize::Save(const te::da::GroupByItem* groupByItem, te::xml::Writer& writer)
{
  assert(groupByItem);
  writer.writeStartElement("te_da:GroupByItem");

  Save(groupByItem->getExpression(), writer);

  writer.writeEndElement("te_da:GroupByItem");
}

void te::serialize::Save(const te::da::GroupBy* groupBy, te::xml::Writer& writer)
{
  writer.writeStartElement("te_da:GroupBy");

  for(std::size_t i = 0; i < groupBy->size(); ++i)
  {
    const te::da::GroupByItem& item = groupBy->at(i);

    Save(&item, writer);
  }

  writer.writeEndElement("te_da:GroupBy");
}

void te::serialize::Save(const te::da::Having* having, te::xml::Writer& writer)
{
  assert(having);
  writer.writeStartElement("te_da:Having");

  Save(having->getExp(), writer);

  writer.writeEndElement("te_da:Having");
}

void te::serialize::Save(const te::da::Literal* lit, te::xml::Writer& writer)
{
  assert(lit);
  writer.writeStartElement("te_da:Expression");
  writer.writeAttribute("Type", "Literal");

  //writer.writeValue(lit->getValue());

  writer.writeEndElement("te_da:Expression");
}

void te::serialize::Save(const te::da::OrderByItem* orderByItem, te::xml::Writer& writer)
{
  assert(orderByItem);
  writer.writeStartElement("te_da:OrderByItem");

  Save(orderByItem->getExpression(), writer);

  writer.writeStartElement("te_da:SortOrder");
  std::string so = (orderByItem->getSortOrder() == te::da::ASC ? "ASC" : "DESC");
  writer.writeValue(so);
  writer.writeEndElement("te_da:SortOrder");

  writer.writeEndElement("te_da:OrderByItem");
}

void te::serialize::Save(const te::da::OrderBy* orderBy, te::xml::Writer& writer)
{
  writer.writeStartElement("te_da:OrderBy");

  for(std::size_t i = 0; i < orderBy->size(); ++i)
  {
    const te::da::OrderByItem& item = orderBy->at(i);

    Save(&item, writer);
  }

  writer.writeEndElement("te_da:OrderBy");
}

void te::serialize::Save(const te::da::PropertyName* propertyName, te::xml::Writer& writer)
{
  assert(propertyName);
  writer.writeStartElement("te_da:Expression");
  writer.writeAttribute("Type", "PropertyName");

  writer.writeValue(propertyName->getName());

  writer.writeEndElement("te_da:Expression");
}

void te::serialize::Save(const te::da::Select* select, te::xml::Writer& writer)
{
  assert(select);
  writer.writeStartElement("te_da:Select");

  std::auto_ptr<const te::da::Fields> fields(select->getFields());
  if(fields.get())
    Save(fields.get(), writer); // Fields
  
  std::auto_ptr<const te::da::From> from(select->getFrom());
  if(from.get())
    Save(from.get(), writer); // From

  std::auto_ptr<const te::da::Where> wh(select->getWhere());
  if(wh.get())
    Save(wh.get(), writer); // Where

  std::auto_ptr<const te::da::GroupBy> groupBy(select->getGroupBy());
  if(groupBy.get())
    Save(groupBy.get(), writer); // GroupBy

  std::auto_ptr<const te::da::Having> having(select->getHaving());
  if(having.get())
    Save(having.get(), writer); // Having

  std::auto_ptr<const te::da::OrderBy> orderBy(select->getOrderBy());
  if(orderBy.get())
    Save(orderBy.get(), writer); // OrderBy

  std::auto_ptr<const te::da::Distinct> distinct(select->getDistinct());
  if(distinct.get())
    Save(distinct.get(), writer); // Distinct

  if(select->getLimit() != 0) // Limit
  {
    writer.writeStartElement("te_da:Limit");
    writer.writeValue(select->getLimit());
    writer.writeEndElement("te_da:Limit");
  }

  if(select->getOffset() != 0) // Offset
  {
    writer.writeStartElement("te_da:Offset");
    writer.writeValue(select->getOffset());
    writer.writeEndElement("te_da:Offset");
  }

  writer.writeEndElement("te_da:Select");
}

void te::serialize::Save(const te::da::Where* wh, te::xml::Writer& writer)
{
  writer.writeStartElement("te_da:OrdeWhererBy");

  Save(wh->getExp(), writer);

  writer.writeEndElement("te_da:Where");
}
