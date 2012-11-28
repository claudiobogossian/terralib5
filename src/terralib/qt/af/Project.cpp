/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Project.cpp

  \brief Application project.
*/

// TerraLib
#include <terralib/maptools/AbstractLayer.h>
#include "Project.h"

// STL
#include <string>

te::qt::af::Project::Project()
  : m_title(""),
  m_author(""),
  m_layerList(boost::ptr_vector<te::map::AbstractLayer*>())
{

}

te::qt::af::Project::~Project()
{

}

void te::qt::af::Project::setTitle(std::string title)
{
  m_title = title;
}

std::string te::qt::af::Project::getTitle()
{
  return m_title;
}

void te::qt::af::Project::setAuthor(std::string author)
{
  m_author = author;
}

std::string te::qt::af::Project::getAuthor()
{
  return m_author;
}

void te::qt::af::Project::setLayers(boost::ptr_vector<te::map::AbstractLayer*> layerList)
{
  m_layerList = layerList;
}

boost::ptr_vector<te::map::AbstractLayer*> te::qt::af::Project::getLayers()
{
  return m_layerList;
}

void te::qt::af::Project::addLayer(te::map::AbstractLayer* layer)
{
  m_layerList.push_back(&layer);
}

void te::qt::af::Project::removeLayer(te::map::AbstractLayer* layer)
{
  m_layerList.erase(std::find(m_layerList.begin(), m_layerList.end(), layer));
}