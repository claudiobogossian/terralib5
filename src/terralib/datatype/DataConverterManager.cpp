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
  \file terralib/datatype/DataConverterManager.cpp

  \brief A singleton for managing the data type converter available in the system.
*/

// TerraLib
#include "../common/Translator.h"
#include "DataConverterManager.h"

// Boost
#include <boost/format.hpp>

void te::dt::DataConverterManager::add(int src, int dst, te::dt::DataTypeConverter conv) throw(Exception)
{
  std::pair<int, int> mapping(src, dst);

  std::map<std::pair<int, int>, DataTypeConverter>::iterator it = m_convMap.find(mapping);
  if(it != m_convMap.end())
    throw Exception((boost::format(TE_TR("There is already a converter registered for the data type conversion: %1% -> %2%.")) % src % dst).str()); 

  m_convMap[mapping] = conv;
}

const te::dt::DataTypeConverter& te::dt::DataConverterManager::get(const std::pair<int, int>& typeMap) throw(Exception)
{
  std::map<std::pair<int, int>, DataTypeConverter>::iterator it = m_convMap.find(typeMap);
  if(it == m_convMap.end())
    throw Exception((boost::format(TE_TR("There is not a converter registered for the data type conversion: %1% -> %2%.")) % typeMap.first % typeMap.second).str()); 

  return it->second;
}

te::dt::DataConverterManager::DataConverterManager()
{
}

te::dt::DataConverterManager::~DataConverterManager()
{
}
