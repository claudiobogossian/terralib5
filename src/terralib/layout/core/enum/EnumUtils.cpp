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
#include "../../../maptools/Enums.h"

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

  return StyleNone;
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
  case te::map::SolidLine:
    return te::map::SolidLine;
    break;
  case te::map::DashLine:
    return te::map::DashLine;
    break;
  case te::map::DotLine:
    return te::map::DotLine;
    break;
  case te::map::DashDotLine:
    return te::map::DashDotLine;
    break;
  case te::map::DashDotDotLine:
    return te::map::DashDotDotLine;
    break;
  default:
    return te::map::NoLine;
  }

  return te::map::NoLine;
}

std::string te::layout::getLayoutAbstractObjectType( int enumVal )
{
  std::string s_val = "TPObjectUnknown";

  switch(enumVal)
  {
  case TPRetangleItem:
    s_val = "TPRetangleItem";
    break;
  case TPMapItem:
    s_val = "TPMapItem";
    break;
  case TPMapGridItem:
    s_val = "TPMapGridItem";
    break;
  case TPPaperItem:
    s_val = "TPPaperItem";
    break;
  case TPLegendItem:
    s_val = "TPLegendItem";
    break;
  case TPPropertiesWindow:
    s_val = "TPPropertiesWindow";
    break;
  case TPDisplayWindow:
    s_val = "TPDisplayWindow";
    break;
  case TPObjectInspectorWindow:
    s_val = "TPObjectInspectorWindow";
    break;
  case TPToolbar:
    s_val = "TPToolbar";
    break;
  case TPGridSettings:
    s_val = "TPGridSettings";
    break;
  case TPGrid:
    s_val = "TPGrid";
    break;
  case TPGridPlanar:
    s_val = "TPGridPlanar";
    break;
  case TPGridGeodesic:
    s_val = "TPGridGeodesic";
    break;
  case TPHorizontalRuler:
    s_val = "TPHorizontalRuler";
    break;
  case TPVerticalRuler:
    s_val = "TPVerticalRuler";
    break;
  case TPTextItem:
    s_val = "TPTextItem";
    break;
  case TPItemGroup:
    s_val = "TPItemGroup";
    break;
  case TPScaleItem:
    s_val = "TPScaleItem";
    break;
  default:
    s_val = "TPObjectUnknown";
  } 

  return s_val;
}

te::layout::LayoutAbstractObjectType te::layout::getLayoutAbstractObjectType( std::string val )
{
  te::layout::LayoutAbstractObjectType type = TPObjectUnknown;

  if(val.compare("TPRetangleItem") == 0)
  {
    type = TPRetangleItem;
  }
  if(val.compare("TPMapItem") == 0)
  {
    type = TPMapItem;
  }
  if(val.compare("TPMapGridItem") == 0)
  {
    type = TPMapGridItem;
  }
  if(val.compare("TPPaperItem") == 0)
  {
    type = TPPaperItem;
  }
  if(val.compare("TPLegendItem") == 0)
  {
    type = TPLegendItem;
  }
  if(val.compare("TPPropertiesWindow") == 0)
  {
    type = TPPropertiesWindow;
  }
  if(val.compare("TPDisplayWindow") == 0)
  {
    type = TPDisplayWindow;
  }
  if(val.compare("TPObjectInspectorWindow") == 0)
  {
    type = TPObjectInspectorWindow;
  }
  if(val.compare("TPToolbar") == 0)
  {
    type = TPToolbar;
  }
  if(val.compare("TPGridSettings") == 0)
  {
    type = TPGridSettings;
  }
  if(val.compare("TPGrid") == 0)
  {
    type = TPGrid;
  }
  if(val.compare("TPGridPlanar") == 0)
  {
    type = TPGridPlanar;
  }
  if(val.compare("TPGridGeodesic") == 0)
  {
    type = TPGridGeodesic;
  }
  if(val.compare("TPHorizontalRuler") == 0)
  {
    type = TPHorizontalRuler;
  }
  if(val.compare("TPVerticalRuler") == 0)
  {
    type = TPVerticalRuler;
  }
  if(val.compare("TPTextItem") == 0)
  {
    type = TPTextItem;
  }
  if(val.compare("TPItemGroup") == 0)
  {
    type = TPItemGroup;
  }
  if(val.compare("TPScaleItem") == 0)
  {
    type = TPScaleItem;
  }

  return type;
}
