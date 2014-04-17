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
  \file terralib/datatype/Module.cpp
   
  \brief This singleton defines the TerraLib Data Type module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "BooleanConverters.h"
#include "Config.h"
#include "DataConverterManager.h"
#include "Enums.h"
#include "IntegerConverters.h"
#include "Module.h"
#include "StringConverters.h"

const te::dt::Module& sm_module = te::dt::Module::getInstance();

te::dt::Module::Module()
{
  TerraLib::Module m = { TE_DATATYPE_MODULE_NAME,
                         te::dt::Module::initialize,
                         te::dt::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::dt::Module::~Module()
{
  TerraLib::getInstance().remove(TE_DATATYPE_MODULE_NAME);
}

void te::dt::Module::initialize()
{
  /* Registering data type converters */

  DataConverterManager& manager = DataConverterManager::getInstance();

  // Int32
  manager.add(te::dt::INT32_TYPE, te::dt::CHAR_TYPE, Int32ToCharConverter);
  manager.add(te::dt::INT32_TYPE, te::dt::UCHAR_TYPE, Int32ToUCharConverter);
  manager.add(te::dt::INT32_TYPE, te::dt::INT16_TYPE, Int32ToInt16Converter);
  manager.add(te::dt::INT32_TYPE, te::dt::UINT16_TYPE, Int32ToUInt16Converter);
  manager.add(te::dt::INT32_TYPE, te::dt::UINT32_TYPE, Int32ToUInt32Converter);
  manager.add(te::dt::INT32_TYPE, te::dt::INT64_TYPE, Int32ToInt64Converter);
  manager.add(te::dt::INT32_TYPE, te::dt::UINT64_TYPE, Int32ToUInt64Converter);
  manager.add(te::dt::INT32_TYPE, te::dt::BOOLEAN_TYPE, Int32ToBooleanConverter);
  manager.add(te::dt::INT32_TYPE, te::dt::FLOAT_TYPE, Int32ToFloatConverter);
  manager.add(te::dt::INT32_TYPE, te::dt::DOUBLE_TYPE, Int32ToDoubleConverter);
  manager.add(te::dt::INT32_TYPE, te::dt::NUMERIC_TYPE, Int32ToNumericConverter);
  manager.add(te::dt::INT32_TYPE, te::dt::STRING_TYPE, Int32ToStringConverter);

  // Unsigned Int32
  manager.add(te::dt::UINT32_TYPE, te::dt::CHAR_TYPE, UInt32ToCharConverter);
  manager.add(te::dt::UINT32_TYPE, te::dt::UCHAR_TYPE, UInt32ToUCharConverter);
  manager.add(te::dt::UINT32_TYPE, te::dt::INT16_TYPE, UInt32ToInt16Converter);
  manager.add(te::dt::UINT32_TYPE, te::dt::UINT16_TYPE, UInt32ToUInt16Converter);
  manager.add(te::dt::UINT32_TYPE, te::dt::INT32_TYPE, UInt32ToInt32Converter);
  manager.add(te::dt::UINT32_TYPE, te::dt::INT64_TYPE, UInt32ToInt64Converter);
  manager.add(te::dt::UINT32_TYPE, te::dt::UINT64_TYPE, UInt32ToUInt64Converter);
  manager.add(te::dt::UINT32_TYPE, te::dt::BOOLEAN_TYPE, UInt32ToBooleanConverter);
  manager.add(te::dt::UINT32_TYPE, te::dt::FLOAT_TYPE, UInt32ToFloatConverter);
  manager.add(te::dt::UINT32_TYPE, te::dt::DOUBLE_TYPE, UInt32ToDoubleConverter);
  manager.add(te::dt::UINT32_TYPE, te::dt::NUMERIC_TYPE, UInt32ToNumericConverter);
  manager.add(te::dt::UINT32_TYPE, te::dt::STRING_TYPE, UInt32ToStringConverter);

  // Boolean
  manager.add(te::dt::BOOLEAN_TYPE, te::dt::CHAR_TYPE, BooleanToCharConverter);
  manager.add(te::dt::BOOLEAN_TYPE, te::dt::UCHAR_TYPE, BooleanToUCharConverter);
  manager.add(te::dt::BOOLEAN_TYPE, te::dt::INT16_TYPE, BooleanToInt16Converter);
  manager.add(te::dt::BOOLEAN_TYPE, te::dt::UINT16_TYPE, BooleanToUInt16Converter);
  manager.add(te::dt::BOOLEAN_TYPE, te::dt::INT32_TYPE, BooleanToInt32Converter);
  manager.add(te::dt::BOOLEAN_TYPE, te::dt::UINT32_TYPE, BooleanToUInt32Converter);
  manager.add(te::dt::BOOLEAN_TYPE, te::dt::INT64_TYPE, BooleanToInt64Converter);
  manager.add(te::dt::BOOLEAN_TYPE, te::dt::UINT64_TYPE, BooleanToUInt64Converter);
  manager.add(te::dt::BOOLEAN_TYPE, te::dt::FLOAT_TYPE, BooleanToFloatConverter);
  manager.add(te::dt::BOOLEAN_TYPE, te::dt::DOUBLE_TYPE, BooleanToDoubleConverter);
  manager.add(te::dt::BOOLEAN_TYPE, te::dt::NUMERIC_TYPE, BooleanToNumericConverter);
  manager.add(te::dt::BOOLEAN_TYPE, te::dt::STRING_TYPE, BooleanToStringConverter);

  // String
  manager.add(te::dt::STRING_TYPE, te::dt::INT16_TYPE, StringToInt16Converter);
  manager.add(te::dt::STRING_TYPE, te::dt::UINT16_TYPE, StringToUInt16Converter);
  manager.add(te::dt::STRING_TYPE, te::dt::INT32_TYPE, StringToInt32Converter);
  manager.add(te::dt::STRING_TYPE, te::dt::UINT32_TYPE, StringToUInt32Converter);
  manager.add(te::dt::STRING_TYPE, te::dt::INT64_TYPE, StringToInt64Converter);
  manager.add(te::dt::STRING_TYPE, te::dt::UINT64_TYPE, StringToUInt64Converter);
  manager.add(te::dt::STRING_TYPE, te::dt::FLOAT_TYPE, StringToFloatConverter);
  manager.add(te::dt::STRING_TYPE, te::dt::DOUBLE_TYPE, StringToDoubleConverter);
  manager.add(te::dt::STRING_TYPE, te::dt::NUMERIC_TYPE, StringToNumericConverter);

  // TO DO: more converters...

  TE_LOG_TRACE(TE_TR("TerraLib Data Type module initialized!"));
}

void te::dt::Module::finalize()
{
  DataConverterManager::getInstance().clear();

  TE_LOG_TRACE(TE_TR("TerraLib Data Type module finalized!"));
}
