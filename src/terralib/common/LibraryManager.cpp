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
  \file terralib/common/LibraryManager.cpp

  \brief A singleton that can be used to register the available libraries in the system.
 */

// TerraLib
#include "LibraryManager.h"

// STL
#include <map>

// Boost
#include <boost/weak_ptr.hpp>

class te::common::LibraryManager::Impl
{
  public:

    typedef boost::weak_ptr<te::common::Library> LibraryWeakPtr;

    Impl()
    {
    }

    ~Impl()
    {
    }

    void add(const std::string& id, const te::common::LibraryPtr& l) throw()
    {
      m_libraryMap[id] = l;
    }

    te::common::LibraryPtr find(const std::string& id) throw()
    {
      std::map<std::string, LibraryWeakPtr>::const_iterator it = m_libraryMap.find(id);

      if(it != m_libraryMap.end())
        return it->second.lock();

      return te::common::LibraryPtr();
    }

  private:

    std::map<std::string, LibraryWeakPtr> m_libraryMap;
};

void te::common::LibraryManager::add(const std::string& id, const LibraryPtr& l) throw()
{
  m_pImpl->add(id, l);
}

te::common::LibraryPtr te::common::LibraryManager::find(const std::string& id) throw()
{
  return m_pImpl->find(id);
}

te::common::LibraryManager::LibraryManager()
  : m_pImpl(0)
{
  m_pImpl = new Impl;
}

te::common::LibraryManager::~LibraryManager()
{
  delete m_pImpl;
}

