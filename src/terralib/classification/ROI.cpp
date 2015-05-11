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
  \file terralib/classification/ROI.cpp

  \brief A region of interest (often abbreviated ROI), is a selected subset of 
        samples within a dataset identified for a particular purpose.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../geometry/Polygon.h"
#include "ROI.h"

te::cl::ROI::ROI(std::string label): m_label(label), m_color("#ffffff")
{
}

te::cl::ROI::~ROI()
{
  te::common::FreeContents(m_roiMap);
  m_roiMap.clear();
}

void te::cl::ROI::setLabel(std::string label)
{
  m_label = label;
}

std::string te::cl::ROI::getLabel()
{
  return m_label;
}

void te::cl::ROI::setColor(std::string color)
{
  m_color = color;
}

std::string te::cl::ROI::getColor()
{
  return m_color;
}

std::map<std::string, te::gm::Polygon*>& te::cl::ROI::getPolygons()
{
  return m_roiMap;
}

void te::cl::ROI::addPolygon(te::gm::Polygon* p, std::string id)
{
  m_roiMap[id] = p;
}

void te::cl::ROI::removePolygon(std::string id)
{
  std::map<std::string, te::gm::Polygon*>::iterator it = m_roiMap.find(id);

  if(it != m_roiMap.end())
    m_roiMap.erase(it);
}
