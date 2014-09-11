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
  \file terralib/edit/RepositoryManager.cpp

  \brief This is a singleton for managing edit repositories.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "Repository.h"
#include "RepositoryManager.h"

// Boost
#include <boost/format.hpp>

// STL
#include <cassert>

void te::edit::RepositoryManager::addEditedGeometry(const std::string& source, te::da::ObjectId* id, te::gm::Geometry* geom)
{
  Repository* repository = getRepository(source);
  if(repository == 0)
  {
    // Not found! Create a new repository associated with the given source
    repository = new Repository(source);

    // Add the edited geometry
    repository->add(id, geom);

    // Store!
    m_repositories[source] = repository;
  }
  else
    repository->add(id, geom);
}

te::edit::Repository* te::edit::RepositoryManager::getRepository(const std::string& source) const
{
  std::map<std::string, Repository*>::const_iterator it = m_repositories.find(source);

  if(it == m_repositories.end())
    return 0;
  
  return it->second;
}

std::vector<te::edit::IdGeometry*> te::edit::RepositoryManager::getGeometries(const std::string& source, const te::gm::Envelope& e, int srid) const
{
  Repository* repository = getRepository(source);

  if(repository == 0)
    return std::vector<te::edit::IdGeometry*>();

  return repository->getGeometries(e, srid);
}

te::edit::IdGeometry* te::edit::RepositoryManager::getGeometry(const std::string& source, const te::gm::Envelope& e, int srid) const
{
  Repository* repository = getRepository(source);

  if(repository == 0)
    return 0;

  return repository->getGeometry(e, srid);
}

void te::edit::RepositoryManager::clearAll()
{
  clearEditedGeometries();
}

void te::edit::RepositoryManager::clearEditedGeometries()
{
  te::common::FreeContents(m_repositories);
  m_repositories.clear();
}

te::edit::RepositoryManager::RepositoryManager()
{
}

te::edit::RepositoryManager::~RepositoryManager()
{
  clearAll();
}
