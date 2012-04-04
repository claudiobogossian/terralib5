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
  \file terralib/datatype/DataConverterManager.cpp

  \brief A singleton for managing the data type converter available in the system.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "AbstractDataConverter.h"
#include "DataConverterManager.h"

// Boost
#include <boost/format.hpp>

void te::dt::DataConverterManager::add(AbstractDataConverter* conv) throw(Exception)
{
  std::pair<int, int> mapping(conv->getInputDataTypeCode(), conv->getOutputDataTypeCode());

  AbstractDataConverter* regConv = te::common::GetPValue(m_convMap, mapping);

  if(regConv)
    throw Exception((boost::format(TR_DATATYPE("There is already a converter registered for the data type conversion: %1% -> %2%.")) % conv->getInputDataTypeCode() % conv->getOutputDataTypeCode()).str()); 

  m_convMap[mapping] = conv;
}

te::dt::AbstractDataConverter* te::dt::DataConverterManager::get(const std::pair<int, int>& typeMap)
{
  return te::common::GetPValue(m_convMap, typeMap);
}

te::dt::DataConverterManager::DataConverterManager()
{
}

te::dt::DataConverterManager::~DataConverterManager()
{
  te::common::FreeContents(m_convMap);
}

