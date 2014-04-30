/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file EnumUtils.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumUtils.h"
#include "AbstractType.h"
#include "../../../../maptools/Enums.h"

// STL
#include <algorithm>


static std::string EnumLayoutGridStyle[] = { 
  "None", "Continuous", "Cross" 
};

static std::string EnumLayoutDashStyle[] = {
  "No Line", 
  "Solid Line",
  "Dash Line",
  "Dot Line",
  "Dash Dot Line",
  "Dash Dot Dot Line",
  "Custom Dash Line"
};

std::string te::layout::getLayoutGridStyle( int enumVal )
{
  return EnumLayoutGridStyle[enumVal];
}

te::layout::LayoutGridStyle te::layout::getLayoutEnumGridStyle( std::string val )
{
  int size = EnumLayoutGridStyle->size();
  int index = std::distance(EnumLayoutGridStyle, std::find(EnumLayoutGridStyle, EnumLayoutGridStyle + (size-1), val));

  return getLayoutEnumGridStyle(index);
}

te::layout::LayoutGridStyle te::layout::getLayoutEnumGridStyle( int enumVal )
{
  switch(enumVal)
  {
  case 2:
    return StyleContinuous;
    break;
  case 3:
    return StyleCross;
    break;
  default:
    return StyleNone;
  }
}

std::string te::layout::getLayoutDashStyle( int enumVal )
{
  return EnumLayoutDashStyle[enumVal];
}

te::map::LineDashStyle te::layout::getLayoutEnumDashStyle( std::string val )
{
  int size = EnumLayoutDashStyle->size();
  int index = std::distance(EnumLayoutDashStyle, std::find(EnumLayoutDashStyle, EnumLayoutDashStyle + (size-1), val));
  return getLayoutEnumDashStyle(index);
}

te::map::LineDashStyle te::layout::getLayoutEnumDashStyle( int enumVal )
{
  switch(enumVal)
  {
  case 1:
    return te::map::SolidLine;
    break;
  case 2:
    return te::map::DashLine;
    break;
  case 3:
    return te::map::DotLine;
    break;
  case 4:
    return te::map::DashDotLine;
    break;
  case 5:
    return te::map::DashDotDotLine;
    break;
  default:
    return te::map::NoLine;
  }
}
