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
  \file terralib/datatype/Utils.cpp

  \brief Utilitary function for data type module.
*/

// TerraLib
#include "../common/Translator.h"
#include "CompositeProperty.h"
#include "Property.h"
#include "Utils.h"

void te::dt::GetPropertiesPosition(const std::vector<Property*>& properties,
                                   const CompositeProperty* cp,
                                   std::vector<std::size_t>& poslist)
{
  const std::size_t size = properties.size();

  for(std::size_t i = 0; i < size; ++i)
    poslist.push_back(cp->getPropertyPosition(properties[i]));
}

int te::dt::Convert2Terralib(const std::string& dataType)
{
  if(dataType == "VOID_TYPE")
    return te::dt::VOID_TYPE;
  else if(dataType == "BIT_TYPE")
    return te::dt::BIT_TYPE;
  else if(dataType == "CHAR_TYPE")
    return te::dt::CHAR_TYPE;
  else if(dataType == "UCHAR_TYPE")
    return te::dt::UCHAR_TYPE;
  else if(dataType == "INT16_TYPE")
    return te::dt::INT16_TYPE;
  else if(dataType == "INT32_TYPE")
    return te::dt::INT32_TYPE;
  else if(dataType == "UINT32_TYPE")
    return te::dt::UINT32_TYPE;
  else if(dataType == "INT64_TYPE")
    return te::dt::INT64_TYPE;
    else if(dataType == "UINT16_TYPE")
    return te::dt::UINT16_TYPE;
  else if(dataType == "UINT64_TYPE")
    return te::dt::UINT64_TYPE;
  else if(dataType == "BOOLEAN_TYPE")
    return te::dt::BOOLEAN_TYPE;
  else if(dataType == "FLOAT_TYPE")
    return te::dt::FLOAT_TYPE;
  else if(dataType == "DOUBLE_TYPE")
    return te::dt::DOUBLE_TYPE;
  else if(dataType == "NUMERIC_TYPE")
    return te::dt::NUMERIC_TYPE;
  else if(dataType == "STRING_TYPE")
    return te::dt::STRING_TYPE;
  else if(dataType == "BYTE_ARRAY_TYPE")
    return te::dt::BYTE_ARRAY_TYPE;
  else if(dataType == "GEOMETRY_TYPE")
    return te::dt::GEOMETRY_TYPE;
  else if(dataType == "DATETIME_TYPE")
    return te::dt::DATETIME_TYPE;
  else if(dataType == "ARRAY_TYPE")
    return te::dt::ARRAY_TYPE;
  else if(dataType == "COMPOSITE_TYPE")
    return te::dt::COMPOSITE_TYPE;
  else if(dataType == "DATASET_TYPE")
    return te::dt::DATASET_TYPE;
  else if(dataType == "RASTER_TYPE")
    return te::dt::RASTER_TYPE;
  else if(dataType == "CINT16_TYPE")
    return te::dt::CINT16_TYPE;
  else if(dataType == "CINT32_TYPE")
    return te::dt::CINT32_TYPE;
  else if(dataType == "CFLOAT_TYPE")
    return te::dt::CFLOAT_TYPE;
  else if(dataType == "CDOUBLE_TYPE")
    return te::dt::CDOUBLE_TYPE;
  else if(dataType == "XML_TYPE")
    return te::dt::XML_TYPE;
  else if(dataType == "DATASETITEM_TYPE")
    return te::dt::DATASETITEM_TYPE;
  else if(dataType == "POLYMORPHIC_TYPE")
    return te::dt::POLYMORPHIC_TYPE;
  else if(dataType == "R4BITS_TYPE")
    return te::dt::R4BITS_TYPE;
  else if(dataType == "R2BITS_TYPE")
    return te::dt::R2BITS_TYPE;
  else if(dataType == "R1BIT_TYPE")
    return te::dt::R1BIT_TYPE;
  else
    return te::dt::UNKNOWN_TYPE;
}

std::string te::dt::ConvertDataTypeToString(const int& dataType)
{
  if(dataType == te::dt::VOID_TYPE)
    return TE_TR("Void");
  else if(dataType == te::dt::BIT_TYPE)
    return TE_TR("Bit");
  else if(dataType == te::dt::CHAR_TYPE)
    return TE_TR("Char");
  else if(dataType == te::dt::UCHAR_TYPE)
    return TE_TR("Unsigned Char");
  else if(dataType == te::dt::INT16_TYPE)
    return TE_TR("Integer 16");
  else if(dataType == te::dt::INT32_TYPE)
    return TE_TR("Integer 32");
  else if(dataType == te::dt::UINT32_TYPE)
    return TE_TR("Unsigned Integer 32");
  else if(dataType == te::dt::INT64_TYPE)
    return TE_TR("Integer 64");
  else if(dataType == te::dt::UINT16_TYPE)
    return TE_TR("Unsigned Integer 16");
  else if(dataType == te::dt::UINT64_TYPE)
    return TE_TR("Unsigned Integer 64");
  else if(dataType == te::dt::BOOLEAN_TYPE)
    return TE_TR("Boolean");
  else if(dataType == te::dt::FLOAT_TYPE)
    return TE_TR("Float");
  else if(dataType == te::dt::DOUBLE_TYPE)
    return TE_TR("Double");
  else if(dataType == te::dt::NUMERIC_TYPE)
    return TE_TR("Numeric");
  else if(dataType == te::dt::STRING_TYPE)
    return TE_TR("String");
  else if(dataType == te::dt::BYTE_ARRAY_TYPE)
    return TE_TR("Byte Array");
  else if(dataType == te::dt::GEOMETRY_TYPE)
    return TE_TR("Geometry");
  else if(dataType == te::dt::DATETIME_TYPE)
    return TE_TR("Date Time");
  else if(dataType == te::dt::ARRAY_TYPE)
    return TE_TR("Array");
  else if(dataType == te::dt::COMPOSITE_TYPE)
    return TE_TR("Composite");
  else if(dataType == te::dt::DATASET_TYPE)
    return TE_TR("Dataset");
  else if(dataType == te::dt::RASTER_TYPE)
    return TE_TR("Raster");
  else if(dataType == te::dt::CINT16_TYPE)
    return TE_TR("Complex Integer 16");
  else if(dataType == te::dt::CINT32_TYPE)
    return TE_TR("Complex Integer 32");
  else if(dataType == te::dt::CFLOAT_TYPE)
    return TE_TR("Complex Float");
  else if(dataType == te::dt::CDOUBLE_TYPE)
    return TE_TR("Complex Double");
  else if(dataType == te::dt::XML_TYPE)
    return TE_TR("XML");
  else if(dataType == te::dt::DATASETITEM_TYPE)
    return TE_TR("Dataset Item");
  else if(dataType == te::dt::POLYMORPHIC_TYPE)
    return TE_TR("Polymorphic");
  else if(dataType == te::dt::R4BITS_TYPE)
    return TE_TR("Raster 4 Bits");
  else if(dataType == te::dt::R2BITS_TYPE)
    return TE_TR("Raster 2 Bits");
  else if(dataType == te::dt::R1BIT_TYPE)
    return TE_TR("Raster 1 Bit");
  else
    return TE_TR("Unknown");
}
