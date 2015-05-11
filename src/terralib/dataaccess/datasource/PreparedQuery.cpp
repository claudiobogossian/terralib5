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
  \file terralib/dataaccess/datasource/PreparedQuery.cpp

  \brief A class that models a prepared query.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../datatype/AbstractData.h"
#include "../../datatype/ByteArray.h"
#include "../../datatype/DateTime.h"
#include "../../datatype/SimpleData.h"
#include "../../geometry/Geometry.h"
#include "../Exception.h"
#include "PreparedQuery.h"

void te::da::PreparedQuery::bind(int i, const te::dt::AbstractData& ad)
{
  switch(ad.getTypeCode())
  {
    case te::dt::CHAR_TYPE :
      {
        const te::dt::Char& value = static_cast<const te::dt::Char&>(ad);

        bind(i, value.getValue());
      }
    break;

    case te::dt::UCHAR_TYPE :
      {
        const te::dt::UChar& value = static_cast<const te::dt::UChar&>(ad);

        bind(i, value.getValue());
      }
    break;

    case te::dt::INT16_TYPE :
      {
        const te::dt::Int16& value = static_cast<const te::dt::Int16&>(ad);

        bind(i, value.getValue());
      }
    break;

    case te::dt::INT32_TYPE :
      {
        const te::dt::Int32& value = static_cast<const te::dt::Int32&>(ad);

        bind(i, value.getValue());
      }
    break;

    case te::dt::INT64_TYPE :
      {
        const te::dt::Int64& value = static_cast<const te::dt::Int64&>(ad);

        bind(i, value.getValue());
      }
    break;

    case te::dt::BOOLEAN_TYPE :
      {
        const te::dt::Boolean& value = static_cast<const te::dt::Boolean&>(ad);

        bind(i, value.getValue());
      }
    break;

    case te::dt::FLOAT_TYPE :
      {
        const te::dt::Float& value = static_cast<const te::dt::Float&>(ad);

        bind(i, value.getValue());
      }
    break;

    case te::dt::DOUBLE_TYPE :
      {
        const te::dt::Double& value = static_cast<const te::dt::Double&>(ad);

        bind(i, value.getValue());
      }
    break;

    case te::dt::NUMERIC_TYPE :
      {
        const te::dt::Numeric& value = static_cast<const te::dt::Numeric&>(ad);

        bind(i, value.getValue());
      }
    break;

    case te::dt::STRING_TYPE :
      {
        const te::dt::String& value = static_cast<const te::dt::String&>(ad);

        bind(i, value.getValue());
      }
    break;

    case te::dt::BYTE_ARRAY_TYPE :
      {
        const te::dt::ByteArray& value = static_cast<const te::dt::ByteArray&>(ad);

        bind(i, value);
      }
    break;

    case te::dt::GEOMETRY_TYPE :
      {
        const te::gm::Geometry& value = static_cast<const te::gm::Geometry&>(ad);

        bind(i, value);
      }
    break;

    case te::dt::DATETIME_TYPE :
      {
        const te::dt::DateTime& value = static_cast<const te::dt::DateTime&>(ad);

        bind(i, value);
      }
    break;

    default :
      throw Exception(TE_TR("The TerraLib data type is not supported by default prepared query implementation!"));
  }
}

