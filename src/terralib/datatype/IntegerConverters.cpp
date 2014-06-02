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
  \file terralib/datatype/IntegerConverters.cpp

  \brief A set of function that convert an Integer type to other types.
*/

// TerraLib
#include "../common/Translator.h"
#include "Enums.h"
#include "Exception.h"
#include "IntegerConverters.h"
#include "SimpleData.h"

// Boost
#include <boost/lexical_cast.hpp>

// Int32
te::dt::AbstractData* te::dt::Int32ToCharConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertInt32(d);
  return new Char(boost::lexical_cast<char>(static_cast<const Int32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::Int32ToUCharConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertInt32(d);
  return new UChar(boost::lexical_cast<unsigned char>(static_cast<const Int32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::Int32ToInt16Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertInt32(d);
  return new Int16(boost::lexical_cast<int16_t>(static_cast<const Int32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::Int32ToUInt16Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertInt32(d);
  return new UInt16(boost::lexical_cast<uint16_t>(static_cast<const Int32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::Int32ToUInt32Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertInt32(d);
  return new UInt32(static_cast<uint32_t>(static_cast<const Int32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::Int32ToInt64Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertInt32(d);
  return new Int64(boost::lexical_cast<int64_t>(static_cast<const Int32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::Int32ToUInt64Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertInt32(d);
  return new UInt64(static_cast<uint64_t>(static_cast<const Int32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::Int32ToBooleanConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertInt32(d);
  
  bool nval = true;
  if(static_cast<const Int32*>(d)->getValue() == 0)
    nval = false;

  return new Boolean(nval);
}

te::dt::AbstractData* te::dt::Int32ToFloatConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertInt32(d);
  return new Float(boost::lexical_cast<float>(static_cast<const Int32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::Int32ToDoubleConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertInt32(d);
  return new Double(boost::lexical_cast<double>(static_cast<const Int32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::Int32ToNumericConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertInt32(d);
  return new Numeric(boost::lexical_cast<std::string>(static_cast<const Int32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::Int32ToStringConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertInt32(d);
  return new String(boost::lexical_cast<std::string>(static_cast<const Int32*>(d)->getValue()));
}

void te::dt::AssertInt32(AbstractData* d) throw(Exception)
{
  if(d->getTypeCode() != INT32_TYPE)
    throw Exception(TE_TR("The input data type is not a 32-bit integer!"));
}

// UInt32
te::dt::AbstractData* te::dt::UInt32ToCharConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertUInt32(d);
  return new Char(boost::lexical_cast<char>(static_cast<const UInt32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::UInt32ToUCharConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertUInt32(d);
  return new UChar(boost::lexical_cast<unsigned char>(static_cast<const UInt32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::UInt32ToInt16Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertUInt32(d);
  return new Int16(boost::lexical_cast<int16_t>(static_cast<const UInt32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::UInt32ToUInt16Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertUInt32(d);
  return new UInt16(boost::lexical_cast<uint16_t>(static_cast<const UInt32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::UInt32ToInt32Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertUInt32(d);
  return new Int32(boost::lexical_cast<int32_t>(static_cast<const UInt32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::UInt32ToInt64Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertUInt32(d);
  return new Int64(boost::lexical_cast<int64_t>(static_cast<const UInt32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::UInt32ToUInt64Converter(te::dt::AbstractData* d) throw(Exception)
{
  AssertUInt32(d);
  return new UInt64(static_cast<uint64_t>(static_cast<const UInt32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::UInt32ToBooleanConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertUInt32(d);
  
  bool nval = true;
  if(static_cast<const UInt32*>(d)->getValue() == 0)
    nval = false;

  return new Boolean(nval);
}

te::dt::AbstractData* te::dt::UInt32ToFloatConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertUInt32(d);
  return new Float(boost::lexical_cast<float>(static_cast<const UInt32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::UInt32ToDoubleConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertUInt32(d);
  return new Double(boost::lexical_cast<double>(static_cast<const UInt32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::UInt32ToNumericConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertUInt32(d);
  return new Numeric(boost::lexical_cast<std::string>(static_cast<const UInt32*>(d)->getValue()));
}

te::dt::AbstractData* te::dt::UInt32ToStringConverter(te::dt::AbstractData* d) throw(Exception)
{
  AssertUInt32(d);
  return new String(boost::lexical_cast<std::string>(static_cast<const UInt32*>(d)->getValue()));
}

void te::dt::AssertUInt32(AbstractData* d) throw(Exception)
{
  if(d->getTypeCode() != UINT32_TYPE)
    throw Exception(TE_TR("The input data type is not an unsigned 32-bit integer!"));
}
