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
  \file terralib/datatype/Int32ToStringConverter.cpp

  \brief A converter from Int32 data values to String.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "AbstractData.h"
#include "Enums.h"
#include "Exception.h"
#include "Int32ToStringConverter.h"
#include "SimpleData.h"

te::dt::AbstractData* te::dt::Int32ToStringConverter::convert(const AbstractData* d) const throw(te::dt::Exception)
{
  if(d->getTypeCode() != INT32_TYPE)
    throw Exception(TR_DATATYPE("The input data type is not a 32-bit integer!"));

  std::string nval(te::common::Convert2String(static_cast<const Int32*>(d)->getValue()));

  return new String(nval);
}

int te::dt::Int32ToStringConverter::getInputDataTypeCode() const
{
  return INT32_TYPE;
}

int te::dt::Int32ToStringConverter::getOutputDataTypeCode() const
{
  return STRING_TYPE;
}
