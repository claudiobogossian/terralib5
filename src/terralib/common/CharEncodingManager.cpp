/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/CharEncodingManager.cpp

  \brief A singleton for managing the character encodings registered in the system.
*/

// TerraLib
#include "CharEncoding.h"
#include "CharEncodingManager.h"

// STL
#include <cassert>

//void te::common::CharEncodingManager::add(CharEncodingCategory* category)
//{
//  assert(category);
//
//  m_categories.push_back(category);
//}
//
//const std::vector<te::common::CharEncodingCategory*>& te::common::CharEncodingManager::getCategoryList() const
//{
//  return m_categories;
//}
//
//bool te::common::CharEncodingManager::existsCategory(const std::string& name) const
//{
//  const std::size_t size = m_categories.size();
//
//  for(std::size_t i = 0; i < size; ++i)
//  {
//    if(m_categories[i] && m_categories[i]->getName() == name)
//      return true;
//  }
//
//  return false;
//}
//
//bool te::common::CharEncodingManager::existsCharEncoding(const std::string& name) const
//{
//  const std::size_t size = m_categories.size();
//
//  for(std::size_t i = 0; i < size; ++i)
//  {
//    CharEncodingCategory* category = m_categories[i];
//
//    if(category)
//    {
//      const std::size_t ssize = category->getCharEncodingList().size();
//
//      for(std::size_t j = 0; j < ssize; ++j)
//      {
//        CharEncoding* charset = category->getCharEncodingList()[i];
//
//        if(charset && charset->getName() == name)
//          return true;
//      }
//    }
//  }
//
//  return false;
//}

te::common::CharEncodingManager::CharEncodingManager()
{
}

te::common::CharEncodingManager::~CharEncodingManager()
{

}
