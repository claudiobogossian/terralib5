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
 \file terralib/metadata/MetadataManager.cpp
 */

// TerraLib
#include "../common/Exception.h"
#include "../common/Translator.h"
#include "Config.h"
#include "MetadataAnnotation.h"
#include "MetadataManager.h"

// STL
#include <cassert>


te::md::MetadataManager::MetadataManager(){}

te::md::MetadataManager::~MetadataManager()
{
}

void 
te::md::MetadataManager::insert(te::md::MetadataAnnotation* a)
{
  assert(a);
  
  std::map<std::string,MetadataAnnotation*>::const_iterator it = m_metadata.find(a->getAnnotationtId());
  
  if (it != m_metadata.end())
    throw te::common::Exception(TE_TR("A metadata annotation with this id already exists in the manager."));
  
  m_metadata.insert(std::make_pair(a->getAnnotationtId(),a));
}

void 
te::md::MetadataManager::erase(te::md::MetadataAnnotation* a)
{
  assert(a);
  
  std::map<std::string,MetadataAnnotation*>::iterator it = m_metadata.find(a->getAnnotationtId());
  
  if (it == m_metadata.end())
    throw te::common::Exception(TE_TR("A metadata annotation with this id already exists in the manager."));
  
  m_metadata.erase(it);
  
}

const te::md::MetadataAnnotation* 
te::md::MetadataManager::find(const std::string& aid) const
{
  assert(!aid.empty());
  
  std::map<std::string,MetadataAnnotation*>::const_iterator it = m_metadata.find(aid);
  
  if (it == m_metadata.end())
    return 0;
  
  return it->second;
}

const std::map<std::string,te::md::MetadataAnnotation*>& 
te::md::MetadataManager::getAnnotations() const
{
  return m_metadata;
}


bool 
te::md::MetadataManager::isEmpty() const
{
  return m_metadata.empty();
}

void 
te::md::MetadataManager::clear()
{
  m_metadata.clear();
}

const std::map<std::string,te::md::MetadataAnnotation*>::const_iterator
te::md::MetadataManager::getBeginIterator() const
{
  return m_metadata.begin();
}

const std::map<std::string,te::md::MetadataAnnotation*>::const_iterator 
te::md::MetadataManager::getEndIterator() const
{
  return  m_metadata.end();
}




