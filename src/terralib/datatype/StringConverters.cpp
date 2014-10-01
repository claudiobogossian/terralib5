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
  \file terralib/datatype/StringConverters.cpp

  \brief A set of function that convert an String type to other types.
*/

// TerraLib
#include "../common/Translator.h"
#include "Enums.h"
#include "Exception.h"
#include "StringConverters.h"
#include "SimpleData.h"

// Boost
#include <boost/lexical_cast.hpp>

te::dt::AbstractData* te::dt::StringToInt16Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertString(d);
  return new Int16(boost::lexical_cast<int16_t>(static_cast<const String*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::StringToUInt16Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertString(d);
  return new UInt16(boost::lexical_cast<uint16_t>(static_cast<const String*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::StringToInt32Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertString(d);
  return new Int32(boost::lexical_cast<int32_t>(static_cast<const String*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::StringToUInt32Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertString(d);
  return new UInt32(boost::lexical_cast<uint32_t>(static_cast<const String*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::StringToInt64Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertString(d);
  return new Int64(boost::lexical_cast<int64_t>(static_cast<const String*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::StringToUInt64Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertString(d);
  return new UInt64(boost::lexical_cast<uint64_t>(static_cast<const String*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::StringToFloatConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertString(d);
  return new Float(boost::lexical_cast<float>(static_cast<const String*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::StringToDoubleConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertString(d);
  return new Double(boost::lexical_cast<double>(static_cast<const String*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::StringToNumericConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertString(d);
  return new Numeric(static_cast<const String*>(d)->getValue());
}

void te::dt::AssertString(AbstractData* d) throw(Exception)
{
  if(d->getTypeCode() != STRING_TYPE)
    throw Exception(TE_TR("The input data type is not a string!"));
}
