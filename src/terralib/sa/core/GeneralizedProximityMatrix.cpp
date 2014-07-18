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
  \file GeneralizedProximityMatrix.cpp

  \brief This class defines the GPM class.
*/

// Terralib Includes
#include "GeneralizedProximityMatrix.h"

te::sa::GeneralizedProximityMatrix::GeneralizedProximityMatrix() :
  m_dataSetName(""),
  m_attributeName("")
{
}

te::sa::GeneralizedProximityMatrix::~GeneralizedProximityMatrix()
{
}

void te::sa::GeneralizedProximityMatrix::setDataSetName(const std::string& dataSetName)
{
  m_dataSetName = dataSetName;
}

std::string te::sa::GeneralizedProximityMatrix::getDataSetName()
{
  return m_dataSetName;
}

void te::sa::GeneralizedProximityMatrix::setAttributeName( const std::string& attrName)
{
  m_attributeName = attrName;
}

std::string te::sa::GeneralizedProximityMatrix::getAttributeName()
{
  return m_attributeName;
}

void te::sa::GeneralizedProximityMatrix::setGraph(te::graph::AbstractGraph* graph)
{
  m_graph.reset(graph);
}

te::graph::AbstractGraph* te::sa::GeneralizedProximityMatrix::getGraph()
{
  return m_graph.get();
}