/*  Copyright (C) 2010-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/GeometryConverters.h

  \brief A set of function that convert a Geometry type to other types and vice-versa.
*/

// TerraLib
#include "../common/Translator.h"
#include "../datatype/ByteArray.h"
#include "../datatype/Enums.h"
#include "../datatype/SimpleData.h"
#include "Exception.h"
#include "Geometry.h"
#include "GeometryConverters.h"
#include "WKBReader.h"
#include "WKTReader.h"

te::dt::AbstractData* te::gm::GeometryToByteArrayConverter(te::dt::AbstractData* d) throw(Exception)
{
  if(d->getTypeCode() != te::dt::GEOMETRY_TYPE)
    throw Exception(TE_TR("The input data type is not a geometry!"));

  std::size_t size = 0;
  char* wkb = static_cast<const Geometry*>(d)->asBinary(size);

  return new te::dt::ByteArray(wkb, size);
}

te::dt::AbstractData* te::gm::GeometryToStringConverter(te::dt::AbstractData* d) throw(Exception)
{
  if(d->getTypeCode() != te::dt::GEOMETRY_TYPE)
    throw Exception(TE_TR("The input data type is not a geometry!"));

  return new te::dt::String(static_cast<const Geometry*>(d)->asText());
}

te::dt::AbstractData* te::gm::ByteArrayToGeometryConverter(te::dt::AbstractData* d)
{
  if(d->getTypeCode() != te::dt::BYTE_ARRAY_TYPE)
    throw Exception(TE_TR("The input data type is not a byte array!"));

  return WKBReader::read(static_cast<const te::dt::ByteArray*>(d)->getData());
}

te::dt::AbstractData* te::gm::StringToGeometryConverter(te::dt::AbstractData* d)
{
  if(d->getTypeCode() != te::dt::STRING_TYPE)
    throw Exception(TE_TR("The input data type is not a string!"));

  return WKTReader::read(static_cast<const te::dt::String*>(d)->getValue().c_str());
}
