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
  \file terralib/datatype/DataTypeManager.cpp

  \brief A singleton for managing all data types in the system.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "DataType.h"
#include "DataTypeManager.h"
#include "Exception.h"

// STL
#include <memory>

int te::dt::DataTypeManager::sm_lastId(0);

int te::dt::DataTypeManager::add(const DataType& dt) throw(te::dt::Exception)
{
  return add(dt.getName(), dt.getDescription());
}

int te::dt::DataTypeManager::add(const std::string& name, const std::string& description) throw(te::dt::Exception)
{
  int id = sm_lastId + 1;

  std::auto_ptr<DataType> dt(new DataType(id, name, description));

  if(m_types.find(dt.get()) != m_types.end())
    throw Exception(TE_TR("There is already a data type with the given id!"));

  if(m_nameIdx.find(dt.get()) != m_nameIdx.end())
    throw Exception(TE_TR("There is already a data type with the given name!"));

  m_types.insert(dt.get());
  m_nameIdx.insert(dt.get());

  dt.release();
  ++sm_lastId;

  return sm_lastId;
}

void te::dt::DataTypeManager::remove(const DataType* dt)
{
  DataType* odt = 0;
  std::set<DataType*, IdComparer>::iterator it = m_types.find((DataType*)dt);

  if(it != m_types.end())
  {
    odt = *it;
    m_types.erase(it);
    std::set<DataType*, NameComparer>::iterator it2 = m_nameIdx.find(odt);

    if(it2 != m_nameIdx.end())
      m_nameIdx.erase(it2);
  }

  delete odt;
}

const te::dt::DataType* te::dt::DataTypeManager::find(int id) const
{
  DataType dummy(id, "", "");

  std::set<DataType*, IdComparer>::const_iterator it = m_types.find(&dummy);

  if(it != m_types.end())
    return *it;

  return 0;
}

const te::dt::DataType* te::dt::DataTypeManager::find(const std::string& name) const
{

  DataType dummy(0, name, "");

  std::set<DataType*, NameComparer>::const_iterator it = m_nameIdx.find(&dummy);

  if(it != m_nameIdx.end())
    return *it;

  return 0;
}

te::dt::DataTypeManager::DataTypeManager()
{
}

te::dt::DataTypeManager::~DataTypeManager()
{
  te::common::FreeContents(m_types);
}

