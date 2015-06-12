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
  \file terralib/dataaccess/dataset/AttributeConverterManager.cpp

  \brief A singleton to keep all the registered Attribute Converter.
*/

// Terralib
#include "../../common/Translator.h"
#include "../Exception.h"
#include "AttributeConverters.h"
#include "AttributeConverterManager.h"

// Boost
#include <boost/format.hpp>

void te::da::AttributeConverterManager::addConverter(const std::string& name, AttributeConverter converter)
{
  std::map<std::string, AttributeConverter>::iterator it = m_converters.find(name);

  if(it != m_converters.end())
    throw Exception((boost::format(TE_TR("There is already a converter registered with the given name %1%.")) % name).str());

  m_converters[name] = converter;
}

te::da::AttributeConverter te::da::AttributeConverterManager::getConverter(const std::string& name)
{
  std::map<std::string, AttributeConverter>::iterator it = m_converters.find(name);

  if(it == m_converters.end())
    throw Exception((boost::format(TE_TR("There is no converter registered with the given name %1%.")) % name).str());

  return m_converters[name];
}

void te::da::AttributeConverterManager::getConverters(std::vector<std::string>& names) const
{
  std::map<std::string, AttributeConverter>::const_iterator it = m_converters.begin();

  while(it != m_converters.end())
  {
    names.push_back(it->first);

    ++it;
  }
}