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
  \file terralib/color/ColorScheme.cpp

  \brief Implementation of the color scheme concept.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "ColorScheme.h"
#include "ColorSchemeGroup.h"

te::color::ColorScheme::ColorScheme(const std::string& name)
  : m_parent(0),
    m_name(name)
{
}

te::color::ColorScheme::~ColorScheme()
{
  te::common::FreeContents(m_colorSet);
}

const std::string& te::color::ColorScheme::getName() const
{
  return m_name;
}

void te::color::ColorScheme::setName(const std::string& name)
{
  assert(name.empty() == false);

  if(m_parent == 0)
  {
    m_name = name;
    return;
  }
// disconnect from the parent, change name and re-connect
  m_parent->disconnect(this);
  m_name = name;
  m_parent->push_back(this);  
}

void te::color::ColorScheme::push_back(std::vector<RGBAColor>* colors)
{
  assert(colors);
  m_colorSet.push_back(colors);
}

const std::vector<std::vector<te::color::RGBAColor>* >& te::color::ColorScheme::getColors() const
{
  return m_colorSet;
}

std::vector<te::color::RGBAColor>* te::color::ColorScheme::getColors(size_t size) const
{
  const size_t n = m_colorSet.size();

  for(size_t i = 0; i < n; ++i)
    if(m_colorSet[i]->size() == size)
      return m_colorSet[i];

  return 0;
}

te::color::ColorSchemeGroup* te::color::ColorScheme::getParent() const
{
  return m_parent;
}

void te::color::ColorScheme::setParent(ColorSchemeGroup* parent)
{
  m_parent = parent;
}

