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
  \file terralib/datatype/BooleanConverters.cpp

  \brief A set of function that convert an Boolean type to other types.
*/

// TerraLib
#include "../common/Translator.h"
#include "Enums.h"
#include "Exception.h"
#include "BooleanConverters.h"
#include "SimpleData.h"

// Boost
#include <boost/lexical_cast.hpp>

te::dt::AbstractData* te::dt::BooleanToCharConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertBoolean(d);
  return new Char(boost::lexical_cast<char>(static_cast<const Boolean*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::BooleanToUCharConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertBoolean(d);
  return new UChar(boost::lexical_cast<unsigned char>(static_cast<const Boolean*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::BooleanToInt16Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertBoolean(d);
  return new Int16(static_cast<int16_t>(static_cast<const Boolean*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::BooleanToUInt16Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertBoolean(d);
  return new UInt16(boost::lexical_cast<uint16_t>(static_cast<const Boolean*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::BooleanToInt32Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertBoolean(d);
  return new Int32(static_cast<int32_t>(static_cast<const Boolean*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::BooleanToUInt32Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertBoolean(d);
  return new UInt32(boost::lexical_cast<uint32_t>(static_cast<const Boolean*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::BooleanToInt64Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertBoolean(d);
  return new Int64(static_cast<int64_t>(static_cast<const Boolean*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::BooleanToUInt64Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertBoolean(d);
  return new UInt64(boost::lexical_cast<uint64_t>(static_cast<const Boolean*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::BooleanToFloatConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertBoolean(d);
  return new Float(boost::lexical_cast<float>(static_cast<const Boolean*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::BooleanToDoubleConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertBoolean(d);
  return new Double(boost::lexical_cast<double>(static_cast<const Boolean*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::BooleanToNumericConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertBoolean(d);
  return new Numeric(boost::lexical_cast<std::string>(static_cast<const Boolean*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::BooleanToStringConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertBoolean(d);
  return new String(boost::lexical_cast<std::string>(static_cast<const Boolean*>(d)->getValue()));
}

void te::dt::AssertBoolean(AbstractData* d) throw(Exception)
{
  if(d->getTypeCode() != BOOLEAN_TYPE)
    throw Exception(TE_TR("The input data type is not a boolean!"));
}
