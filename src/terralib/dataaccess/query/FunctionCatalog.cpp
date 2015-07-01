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
  \file terralib/dataaccess/query/FunctionCatalog.cpp

  \brief A FunctionCatalog can be used to keep track of registered functions.
*/

// TerraLib
#include "../../common/STLUtils.h"
#include "FunctionCatalog.h"
#include "FunctionDefn.h"

te::da::FunctionCatalog::~FunctionCatalog()
{
  std::map<std::string, std::vector<FunctionDefn*>* >::const_iterator it = m_fMap.begin();
  std::map<std::string, std::vector<FunctionDefn*>* >::const_iterator it_end = m_fMap.end();

  while(it != it_end)
  {
    te::common::FreeContents(*(it->second));
    delete it->second;
    ++it;
  }
}

void te::da::FunctionCatalog::insert(FunctionDefn* fdfn)
{
  std::map<std::string, std::vector<FunctionDefn*>* >::const_iterator it = m_fMap.find(fdfn->getName());

  if(it != m_fMap.end())
  {
    it->second->push_back(fdfn);
  }
  else
  {
    std::vector<FunctionDefn*>* fcns = new std::vector<FunctionDefn*>(1);
    fcns->push_back(fdfn);
    m_fMap.insert(std::map<std::string, std::vector<FunctionDefn*>* >::value_type(fdfn->getName(), fcns));
  }
}

std::vector<te::da::FunctionDefn*>* te::da::FunctionCatalog::find(const std::string& functionName) const
{
  return te::common::GetPValue(m_fMap, functionName);
}

