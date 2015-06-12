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
  \file terralib/se/ShadedRelief.cpp
  
  \brief ShadedRelief specifies the application of relief shading (or "hill shading") to a DEM raster to give it somewhat of a three-dimensional effect and to make elevation changes more visible.
*/

// TerraLib
#include "ShadedRelief.h"

te::se::ShadedRelief::ShadedRelief()
  : m_brightnessOnly(false),
    m_reliefFactor(0.0)
{
}

te::se::ShadedRelief::~ShadedRelief()
{
}

void te::se::ShadedRelief::setBrightnessOnly(bool b)
{
  m_brightnessOnly = b;
}

bool te::se::ShadedRelief::isBrightnessOnly() const
{
  return m_brightnessOnly;
}

void te::se::ShadedRelief::setReliefFactor(const double& rf)
{
  m_reliefFactor = rf;
}

double te::se::ShadedRelief::getReliefFactor() const
{
  return m_reliefFactor;
}

te::se::ShadedRelief* te::se::ShadedRelief::clone() const
{
  ShadedRelief* sr = new ShadedRelief;
  sr->setBrightnessOnly(m_brightnessOnly);
  sr->setReliefFactor(m_reliefFactor);

  return sr;
}
