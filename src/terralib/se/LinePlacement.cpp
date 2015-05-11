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
  \file terralib/se/LinePlacement.cpp
  
  \brief A LinePlacement specifies how a text label should be rendered relative to a linear geometry.
*/

// TerraLib
#include "LinePlacement.h"
#include "ParameterValue.h"

te::se::LinePlacement::LinePlacement()
  : m_perpendicularOffset(0),
    m_isRepeated(false),
    m_initialGap(0),
    m_gap(0),
    m_isAligned(true),
    m_generalizeLine(false)
{
}

te::se::LinePlacement::~LinePlacement()
{
  delete m_perpendicularOffset;
  delete m_initialGap;
  delete m_gap;
}

void te::se::LinePlacement::setPerpendicularOffset(ParameterValue* p)
{
  delete m_perpendicularOffset;
  m_perpendicularOffset = p;
}

void te::se::LinePlacement::setIsRepeated(bool i)
{
  m_isRepeated = i;
}

void te::se::LinePlacement::setInitialGap(ParameterValue* p)
{
  delete m_initialGap;
  m_initialGap = p;
}

void te::se::LinePlacement::setGap(ParameterValue* p)
{
  delete m_gap;
  m_gap = p;
}

void te::se::LinePlacement::setIsAligned(bool i)
{
  m_isAligned = i;
}

void te::se::LinePlacement::setGeneralizeLine(bool g)
{
  m_generalizeLine = g;
}

te::se::LinePlacement* te::se::LinePlacement::clone() const
{
  LinePlacement* lp = new LinePlacement;

  lp->setIsRepeated(m_isRepeated);
  lp->setIsAligned(m_isAligned);
  lp->setGeneralizeLine(m_generalizeLine);

  if(m_perpendicularOffset)
    lp->setPerpendicularOffset(m_perpendicularOffset->clone());

  if(m_gap)
    lp->setGap(m_gap->clone());

  if(m_initialGap)
    lp->setInitialGap(m_initialGap->clone());

  return lp;
}
