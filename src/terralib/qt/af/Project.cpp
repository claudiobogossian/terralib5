/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
#include "../../maptools/AbstractLayer.h"
#include "Project.h"

// STL
#include <algorithm>

te::qt::af::Project::Project()
  : m_title(""),
    m_author(""),
    m_layers()
{
}

te::qt::af::Project::~Project()
{

}

void te::qt::af::Project::setTitle(const std::string& title)
{
  m_title = title;
}

const std::string& te::qt::af::Project::getTitle() const
{
  return m_title;
}

void te::qt::af::Project::setAuthor(const std::string& author)
{
  m_author = author;
}

const std::string& te::qt::af::Project::getAuthor() const
{
  return m_author;
}

const std::list<te::map::AbstractLayer*>& te::qt::af::Project::getLayers() const
{
  return m_layers;
}

std::list<te::map::AbstractLayer*>& te::qt::af::Project::getLayers()
{
  return m_layers;
}

void te::qt::af::Project::add(te::map::AbstractLayer* layer)
{
  m_layers.push_back(layer);
}

void te::qt::af::Project::remove(te::map::AbstractLayer* layer)
{
  std::remove(m_layers.begin(), m_layers.end(), layer);
}

