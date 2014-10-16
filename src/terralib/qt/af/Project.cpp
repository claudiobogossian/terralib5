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
  \file terralib/qt/af/Project.cpp

  \brief This class models the concept of a project for the TerraLib Application Framework.
*/

// TerraLib
#include "Project.h"

// STL
#include <algorithm>

te::qt::af::Project::Project()
  : m_title(""),
    m_author(""),
    m_topLayers(),
    m_changed(false)
{
}

te::qt::af::Project::~Project()
{

}

void te::qt::af::Project::setTitle(const std::string& title)
{
  m_title = title;
  m_changed = true;
}

const std::string& te::qt::af::Project::getTitle() const
{
  return m_title;
}

void te::qt::af::Project::setAuthor(const std::string& author)
{
  m_author = author;
  m_changed = true;
}

const std::string& te::qt::af::Project::getAuthor() const
{
  return m_author;
}

const std::list<te::map::AbstractLayerPtr>& te::qt::af::Project::getTopLayers() const
{
  return m_topLayers;
}

std::list<te::map::AbstractLayerPtr>& te::qt::af::Project::getTopLayers()
{
  return m_topLayers;
}

std::list<te::map::AbstractLayerPtr> te::qt::af::Project::getAllLayers(bool invalid)
{
  std::list<te::map::AbstractLayerPtr> layers;

  std::list<te::map::AbstractLayerPtr>::const_iterator it;

  for(it = m_topLayers.begin(); it != m_topLayers.end(); ++it)
  {
    te::map::AbstractLayerPtr topLevelLayer = *it;

    if(!invalid && !topLevelLayer->isValid())
    {
      continue;
    }

    layers.push_back(topLevelLayer);

    if(topLevelLayer->getType() == "FOLDERLAYER")
    {
      std::vector<te::map::AbstractLayer*> children = topLevelLayer->getDescendants();
      for(std::size_t i = 0; i < children.size(); ++i)
      {
        if(!invalid && !children[i]->isValid())
        {
          continue;
        }

        layers.push_back(te::map::AbstractLayerPtr(children[i]));
      }
        
    }
  }

  return layers;
}

std::list<te::map::AbstractLayerPtr> te::qt::af::Project::getSingleLayers(bool invalid)
{
  std::list<te::map::AbstractLayerPtr> singleLayers;

  std::list<te::map::AbstractLayerPtr> allLayers = getAllLayers(invalid);

  std::list<te::map::AbstractLayerPtr>::const_iterator it;

  for(it = allLayers.begin(); it != allLayers.end(); ++it)
  {
    te::map::AbstractLayerPtr layer = *it;
    if(layer->getType() == "FOLDERLAYER")
      continue;

    singleLayers.push_back(te::map::AbstractLayerPtr(layer));
  }

  return singleLayers;
}

std::list<te::map::AbstractLayerPtr> te::qt::af::Project::getVisibleSingleLayers(bool invalid)
{
  std::list<te::map::AbstractLayerPtr> visibleSingleLayers;

  std::list<te::map::AbstractLayerPtr> singleLayers = getSingleLayers(invalid);

  std::list<te::map::AbstractLayerPtr>::const_iterator it;
  for(it = singleLayers.begin(); it != singleLayers.end(); ++it)
  {
    te::map::AbstractLayerPtr singleLayer = *it;

    if(singleLayer->getVisibility() == te::map::VISIBLE)
      visibleSingleLayers.push_back(singleLayer);
  }

   return visibleSingleLayers;
}

void te::qt::af::Project::setTopLayers(const std::list<te::map::AbstractLayerPtr>& layers)
{
  m_topLayers.clear();
  m_topLayers = layers;
}

const std::list<te::map::AbstractLayerPtr> te::qt::af::Project::getSelectedLayers(bool invalid) const
{
  if(!invalid)
  {
    std::list<te::map::AbstractLayerPtr>::const_iterator it = m_selectedLayers.begin();

    std::list<te::map::AbstractLayerPtr> validLayers;

    while(it != m_selectedLayers.end())
    {
      if(it->get()->isValid())
        validLayers.push_back(it->get());

      ++it;
    }

    return validLayers;
  }
  else
  {
    return m_selectedLayers;
  }
}

void te::qt::af::Project::setSelectedLayers(const std::list<te::map::AbstractLayerPtr>& selectedLayers)
{
  m_selectedLayers.clear();

  m_selectedLayers = selectedLayers;
}

void te::qt::af::Project::add(const te::map::AbstractLayerPtr& layer, const te::map::AbstractLayerPtr& parentLayer)
{
  if(!parentLayer)
    m_topLayers.push_back(layer);
  else
    parentLayer->add(layer);

  m_changed = true;
}

void te::qt::af::Project::remove(const te::map::AbstractLayerPtr& layer)
{
  te::map::AbstractLayerPtr parentLayer = static_cast<te::map::AbstractLayer*>(layer->getParent());

  if(!parentLayer)
    m_topLayers.remove(layer);
  else
    parentLayer->remove(layer->getIndex());

  m_changed = true;
}

bool te::qt::af::Project::hasChanged()
{
  return m_changed;
}

void te::qt::af::Project::setFileName(const std::string& fName)
{
  m_fileName = fName;
  m_changed = true;
}

const std::string& te::qt::af::Project::getFileName() const
{
  return m_fileName;
}

void te::qt::af::Project::setProjectAsChanged(const bool& changed)
{
  m_changed = changed;
}

void te::qt::af::Project::clear()
{
  m_topLayers.clear();
  m_changed = true;
}
