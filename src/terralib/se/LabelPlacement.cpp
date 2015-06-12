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
  \file terralib/se/LabelPlacement.cpp
  
  \brief The LabelPlacement specifies where and how a text label should be rendered relative to a geometry.
*/

// TerraLib
#include "LabelPlacement.h"
#include "LinePlacement.h"
#include "PointPlacement.h"

te::se::LabelPlacement::LabelPlacement()
  : m_pointPlacement(0),
    m_linePlacement(0)
{
}

te::se::LabelPlacement::~LabelPlacement()
{
  delete m_pointPlacement;
  delete m_linePlacement;
}

void te::se::LabelPlacement::setPointPlacement(PointPlacement* p)
{
  delete m_pointPlacement;
  m_pointPlacement = p;
}

void te::se::LabelPlacement::setLinePlacement(LinePlacement* l)
{
  delete m_linePlacement;
  m_linePlacement = l;
}

te::se::LabelPlacement* te::se::LabelPlacement::clone() const
{
  LabelPlacement* lp = new LabelPlacement;

  if(m_pointPlacement)
    lp->setPointPlacement(m_pointPlacement->clone());

  if(m_linePlacement)
    lp->setLinePlacement(m_linePlacement->clone());

  return lp;
}
