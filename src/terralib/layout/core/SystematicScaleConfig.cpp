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
  \file SystematicScaleConfig.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "SystematicScaleConfig.h"
#include "Systematic.h"

te::layout::SystematicScaleConfig::SystematicScaleConfig() 
{
  
}

te::layout::SystematicScaleConfig::~SystematicScaleConfig()
{

}

te::layout::Systematic* te::layout::SystematicScaleConfig::getSystematic( SystematicScaleType type )
{
  Systematic* systematic = new Systematic;

  double w = 0.;
  double h = 0.;
  int scale = 0;
  double planarGap = 0.;
  double geoGap = 0.;

  switch(type)
  {
  case S250000:
    w = 1.5;
    h = 1.;
    scale = 250000;
    planarGap = 10000;
    geoGap = 15;//minutes
    break;
  case S100000:
    w = 0.5;
    h = 0.5;
    scale = 100000;
    planarGap = 4000;
    geoGap = 10;//minutes
    break;
  case S50000:
    w = 0.25;
    h = 0.25;
    scale = 50000;
    planarGap = 2000;
    geoGap = 5;//minutes
    break;
  case S25000:
    w = 0.125;
    h = 0.125;
    scale = 25000;
    planarGap = 10000;
    geoGap = 2.5;//minutes
    break;
  case S0:
      break;
  default:
      break;
  }

  systematic->setWidth(w);
  systematic->setHeight(h);
  systematic->setScale(scale);
  systematic->setPlanarGap(planarGap);
  systematic->setGeoGap(geoGap);

  return systematic;
}
