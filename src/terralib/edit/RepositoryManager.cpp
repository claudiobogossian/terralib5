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

void te::edit::RepositoryManager::addGeometry(const std::string& source, te::gm::Geometry* geom, OperationType operation)
{
  Repository* repository = getRepository(source);

  if (repository == 0)
  {
    // Not found! Create a new repository associated with the given source
    repository = new Repository(source);

    // Add the geometry
    repository->add(geom, operation);

    // Store!
    m_repositories[source] = repository;

  }
  else
    repository->add(geom, operation);
}

void te::edit::RepositoryManager::addGeometry(const std::string& source, te::da::ObjectId* id, te::gm::Geometry* geom, OperationType operation)
{
  Repository* repository = getRepository(source);

  if (repository == 0)
  {
    // Not found! Create a new repository associated with the given source
    repository = new Repository(source);

    // Add the geometry
    repository->add(id, geom, operation);

    // Store!
    m_repositories[source] = repository;
  }
  else
    repository->add(id, geom, operation);
}

void te::edit::RepositoryManager::addFeature(const std::string& source, Feature* f)
{
  Repository* repository = getRepository(source);

  if (repository == 0)
  {
    // Not found! Create a new repository associated with the given source
    repository = new Repository(source);

    // Add the feature
    repository->add(f);

    // Store!
    m_repositories[source] = repository;
  }
  else
    repository->add(f);
}

bool te::edit::RepositoryManager::hasIdentify(const std::string& source, te::da::ObjectId* id)
{
  assert(id);

  Repository* repository = getRepository(source);

  if (repository == 0)
    return false;

  return repository->hasIdentifier(id);
}

const std::map<std::string, te::edit::Repository*>& te::edit::RepositoryManager::getRepositories() const
{
  return m_repositories;
}

te::edit::Repository* te::edit::RepositoryManager::getRepository(const std::string& source) const
{
  std::map<std::string, Repository*>::const_iterator it = m_repositories.find(source);

  if (it == m_repositories.end())
    return 0;

  return it->second;
}

std::vector<te::edit::Feature*> te::edit::RepositoryManager::getFeatures(const std::string& source, const te::gm::Envelope& e, int srid) const
{
  Repository* repository = getRepository(source);

  if (repository == 0)
    return std::vector<te::edit::Feature*>();

  return repository->getFeatures(e, srid);
}

te::edit::Feature* te::edit::RepositoryManager::getFeature(const std::string& source, const te::gm::Envelope& e, int srid) const
{
  Repository* repository = getRepository(source);

  if (repository == 0)
    return 0;

  return repository->getFeature(e, srid);
}

void te::edit::RepositoryManager::clearAll()
{
  std::map<std::string, Repository*>::const_iterator it;
  for (it = m_repositories.begin(); it != m_repositories.end(); ++it)
    it->second->clear();
}

void te::edit::RepositoryManager::clear(const std::string& source)
{
  Repository* repository = getRepository(source);

  if (repository == 0)
    return;

  repository->clear();
}

void te::edit::RepositoryManager::removeAll()
{
  te::common::FreeContents(m_repositories);
  m_repositories.clear();
}

void te::edit::RepositoryManager::remove(const std::string& source)
{
  std::map<std::string, Repository*>::iterator it = m_repositories.find(source);

  if (it == m_repositories.end())
    return;

  m_repositories.erase(it);

  delete it->second;
}

void te::edit::RepositoryManager::removeFeature(const std::string& source, te::da::ObjectId* id)
{
  Repository* repository = getRepository(source);

  if (repository == 0)
  {
    // Not found! Create a new repository associated with the given source
    repository = new Repository(source);

    // Remove the feature
    repository->remove(id);

    // Store!
    m_repositories[source] = repository;
  }
  else
    repository->remove(id);
}

te::edit::RepositoryManager::RepositoryManager()
{
}

te::edit::RepositoryManager::~RepositoryManager()
{
  removeAll();
}


