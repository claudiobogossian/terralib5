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
  \file terralib/serialization/dataaccess/Query.h
 
  \brief Support for Query serialization.
*/

#ifndef __TERRALIB_SERIALIZATION_DATAACCESS_INTERNAL_QUERY_H
#define __TERRALIB_SERIALIZATION_DATAACCESS_INTERNAL_QUERY_H

// TerraLib
#include "../../dataaccess/query/Distinct.h"
#include "../../dataaccess/query/Fields.h"
#include "../../dataaccess/query/From.h"
#include "../../dataaccess/query/GroupBy.h"
#include "../../dataaccess/query/OrderBy.h"
#include "../Config.h"

namespace te
{
  namespace da
  {
    class Select;
    class Field;
    class Function;
    class GroupByItem;
    class Having;
    class Literal;
    class OrderByItem;
    class PropertyName;
    class Where;
  }

  namespace xml
  {
    class Reader;
    class Writer;
  }

  namespace serialize
  {
    TESERIALIZATIONEXPORT te::da::Distinct* ReadDistinct(te::xml::Reader& reader);

    TESERIALIZATIONEXPORT te::da::Expression* ReadExpression(te::xml::Reader& reader);

    TESERIALIZATIONEXPORT te::da::Field* ReadQueryField(te::xml::Reader& reader);

    TESERIALIZATIONEXPORT te::da::Fields* ReadFields(te::xml::Reader& reader);

    TESERIALIZATIONEXPORT te::da::From* ReadFrom(te::xml::Reader& reader);

    TESERIALIZATIONEXPORT te::da::Function* ReadFunction(te::xml::Reader& reader);

    TESERIALIZATIONEXPORT te::da::GroupBy* ReadGroupBy(te::xml::Reader& reader);

    TESERIALIZATIONEXPORT te::da::Having* ReadHaving(te::xml::Reader& reader);

    TESERIALIZATIONEXPORT te::da::Literal* ReadLiteral(te::xml::Reader& reader);

    TESERIALIZATIONEXPORT te::da::OrderBy* ReadOrderBy(te::xml::Reader& reader);

    TESERIALIZATIONEXPORT te::da::PropertyName* ReadPropertyName(te::xml::Reader& reader);

    TESERIALIZATIONEXPORT te::da::Select* ReadSelect(te::xml::Reader& reader);

    TESERIALIZATIONEXPORT te::da::Where* ReadWhere(te::xml::Reader& reader);

    TESERIALIZATIONEXPORT void Save(const te::da::Distinct* distinct, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::Expression* expression, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::Field* field, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::Fields* fields, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::From* from, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::FromItem* fromItem, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::Function* func, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::GroupByItem* groupByItem, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::GroupBy* groupBy, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::Having* having, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::Literal* lit, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::OrderByItem* orderByItem, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::OrderBy* orderBy, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::PropertyName* propertyName, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::Select* select, te::xml::Writer& writer);

    TESERIALIZATIONEXPORT void Save(const te::da::Where* wh, te::xml::Writer& writer);

  } // end namespace serialize
}   // end namespace te

#endif  // __TERRALIB_SERIALIZATION_DATAACCESS_INTERNAL_QUERY_H

