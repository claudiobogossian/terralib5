/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/maptools/AbstractMarkFactory.cpp

  \brief This is the abstract factory for conversion of Symbology Enconding Mark elements to an image pattern.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../se/Mark.h"
#include "AbstractMarkFactory.h"

// STL
#include <cassert>
#include <string>
#include <map>
#include <vector>

te::map::AbstractMarkFactory::~AbstractMarkFactory()
{
}

te::map::AbstractMarkFactory::AbstractMarkFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<te::map::AbstractMarkFactory, std::string>(factoryKey)
{
}

te::color::RGBAColor** te::map::AbstractMarkFactory::make(const te::se::Mark* mark, std::size_t size)
{
  assert(mark);

  const std::string* name = mark->getWellKnownName();
  assert(name);

  // Try find the factory key
  std::vector<std::string> tokens;
  te::common::Tokenize(*name, tokens, "://");

  // Default
  std::string factoryKey("");

  if(tokens.size() > 1)
    factoryKey = tokens[0];

  return make(factoryKey, mark, size);
}

te::color::RGBAColor** te::map::AbstractMarkFactory::make(const std::string& factoryKey, const te::se::Mark* mark, std::size_t size)
{
  assert(mark);
  assert(size > 0);

  te::map::AbstractMarkFactory* f = te::common::AbstractFactory<te::map::AbstractMarkFactory, std::string>::make(factoryKey);
  assert(f);

  te::color::RGBAColor** rgba = f->create(mark, size);

  return rgba;
}

void te::map::AbstractMarkFactory::SupportedMarks(std::vector<std::string>& marks)
{
  te::common::AbstractFactory<te::map::AbstractMarkFactory, std::string>::dictionary_type& dic =  te::common::AbstractFactory<te::map::AbstractMarkFactory, std::string>::getDictionary();
  std::map<std::string, te::common::AbstractFactory<te::map::AbstractMarkFactory, std::string>*>::const_iterator it = dic.begin();
  while(it != dic.end())
  {
    static_cast<te::map::AbstractMarkFactory*>(it->second)->getSupportedMarks(marks);
    ++it;
  }
}
