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
  \file EnumObjectType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumObjectType.h"

te::layout::EnumObjectType::EnumObjectType() :
  m_objectUnknown(0),
  m_retangleItem(0),
  m_mapItem(0),
  m_mapGridItem(0),
  m_paperItem(0),
  m_legendItem(0),
  m_propertiesWindow(0),
  m_displayWindow(0),
  m_objectInspectorWindow(0),
  m_toolbar(0),
  m_gridSettings(0),
  m_gridPlanar(0),
  m_gridGeodesic(0),
  m_horizontalRuler(0),
  m_verticalRuler(0),
  m_defaultTextItem(0),
  m_itemGroup(0),
  m_scaleItem(0),
  m_pageSetup(0),
  m_imageItem(0),
  m_systematicScale(0),
  m_pointItem(0),
  m_arrowItem(0),
  m_ellipseItem(0),
  m_titleItem(0),
  m_textGridItem(0),
  m_multiLineTextItem(0),
  m_editTemplate(0),
  m_menuItem(0),
  m_textGridSettings(0)
{
  init();
}

te::layout::EnumObjectType::~EnumObjectType()
{
  if(m_objectUnknown)
  {
    delete m_objectUnknown;
    m_objectUnknown = 0;
  }

  if(m_retangleItem)
  {
    delete m_retangleItem;
    m_retangleItem = 0;
  }
   
  if(m_mapItem)
  {
    delete m_mapGridItem;
    m_mapGridItem = 0;
  }

  if(m_paperItem)
  {
    delete m_paperItem;
    m_paperItem = 0;
  }

  if(m_legendItem)
  {
    delete m_legendItem;
    m_legendItem = 0;
  }
    
  if(m_propertiesWindow)
  {
    delete m_propertiesWindow;
    m_propertiesWindow = 0;
  }

  if(m_displayWindow)
  {
    delete m_displayWindow;
    m_displayWindow = 0;
  }

  if(m_objectInspectorWindow)
  {
    delete m_objectInspectorWindow;
    m_objectInspectorWindow = 0;
  }

  if(m_toolbar)
  {
    delete m_toolbar;
    m_toolbar = 0;
  }

  if(m_gridSettings)
  {
    delete m_gridSettings;
    m_gridSettings = 0;
  }
    
  if(m_gridPlanar)
  {
    delete m_gridPlanar;
    m_gridPlanar = 0;
  }
    
  if(m_gridGeodesic)
  {
    delete m_gridGeodesic;
    m_gridGeodesic = 0;
  }
    
  if(m_horizontalRuler)
  {
    delete m_horizontalRuler;
    m_horizontalRuler = 0;
  }

  if(m_verticalRuler)
  {
    delete m_verticalRuler;
    m_verticalRuler = 0;
  }
  
  if(m_itemGroup)
  {
    delete m_itemGroup;
    m_itemGroup = 0;
  }

  if(m_scaleItem)
  {
    delete m_scaleItem;
    m_scaleItem = 0;
  }
    
  if(m_pageSetup)
  {
    delete m_pageSetup;
    m_pageSetup = 0;
  }

  if(m_imageItem)
  {
    delete m_imageItem;
    m_imageItem = 0;
  }

  if(m_systematicScale)
  {
    delete m_systematicScale;
    m_systematicScale = 0;
  }

  if(m_pointItem)
  {
    delete m_pointItem;
    m_pointItem = 0;
  }

  if(m_arrowItem)
  {
    delete m_arrowItem;
    m_arrowItem = 0;
  }

  if(m_ellipseItem)
  {
    delete m_ellipseItem;
    m_ellipseItem = 0;
  }
    
  if(m_titleItem)
  {
    delete m_titleItem;
    m_titleItem = 0;
  }

  if(m_textGridItem)
  {
    delete m_textGridItem;
    m_textGridItem = 0;
  }

  if(m_multiLineTextItem)
  {
    delete m_multiLineTextItem;
    m_multiLineTextItem = 0;
  }

  if(m_editTemplate)
  {
    delete m_editTemplate;
    m_editTemplate = 0;
  }

  if(m_menuItem)
  {
    delete m_menuItem;
    m_menuItem = 0;
  }

  if(m_textGridSettings)
  {
    delete m_textGridSettings;
    m_textGridSettings = 0;
  }

  if(m_objectUnknown)
  {
    delete m_objectUnknown;
    m_objectUnknown = 0;
  }

  if(m_defaultTextItem)
  {
    delete m_defaultTextItem;
    m_defaultTextItem = 0;
  }
}

void te::layout::EnumObjectType::init()
{
  m_objectUnknown = new EnumType(0, "None");
  m_enums.push_back(m_objectUnknown);

  m_retangleItem = new EnumType(1, "Retangle_Item");
  m_enums.push_back(m_retangleItem);

  m_mapItem = new EnumType(2, "Map_Item");
  m_enums.push_back(m_mapItem);

  m_mapGridItem = new EnumType(3, "MapGrid_Item");
  m_enums.push_back(m_mapGridItem);

  m_paperItem = new EnumType(4, "Paper_Item");
  m_enums.push_back(m_paperItem);

  m_legendItem = new EnumType(5, "Legend_Item");
  m_enums.push_back(m_legendItem);

  m_propertiesWindow = new EnumType(6, "Properties_Window");
  m_enums.push_back(m_propertiesWindow);

  m_displayWindow = new EnumType(7, "Display_Window");
  m_enums.push_back(m_displayWindow);

  m_objectInspectorWindow = new EnumType(8, "ObjectInspector_Window");
  m_enums.push_back(m_objectInspectorWindow);

  m_toolbar = new EnumType(9, "Toolbar");
  m_enums.push_back(m_toolbar);

  m_gridSettings  = new EnumType(10, "Grid_Settings");
  m_enums.push_back(m_gridSettings);

  m_gridPlanar = new EnumType(11, "Grid_Planar");
  m_enums.push_back(m_gridPlanar);

  m_gridGeodesic = new EnumType(12, "Grid_Geodesic");
  m_enums.push_back(m_gridGeodesic);

  m_horizontalRuler = new EnumType(13, "Horizontal_Ruler");
  m_enums.push_back(m_horizontalRuler);

  m_verticalRuler = new EnumType(14, "Vertical_Ruler");
  m_enums.push_back(m_verticalRuler);

  m_defaultTextItem = new EnumType(15, "Text_Item");
  m_enums.push_back(m_defaultTextItem);

  m_itemGroup = new EnumType(16, "Item_Group");
  m_enums.push_back(m_itemGroup);

  m_scaleItem = new EnumType(17, "Scale_Item");
  m_enums.push_back(m_scaleItem);

  m_pageSetup = new EnumType(18, "Page_Setup");
  m_enums.push_back(m_pageSetup);

  m_imageItem = new EnumType(19, "Image_Item");
  m_enums.push_back(m_imageItem);

  m_systematicScale = new EnumType(20, "Systematic_Scale");
  m_enums.push_back(m_systematicScale);

  m_pointItem = new EnumType(21, "Point_Item");
  m_enums.push_back(m_pointItem);

  m_arrowItem = new EnumType(22, "Arrow_Item");
  m_enums.push_back(m_arrowItem);

  m_ellipseItem = new EnumType(23, "Ellipse_Item");
  m_enums.push_back(m_ellipseItem);

  m_titleItem = new EnumType(24, "Title_Item");
  m_enums.push_back(m_titleItem);

  m_textGridItem = new EnumType(25, "TextGrid_Item");
  m_enums.push_back(m_textGridItem);

  m_multiLineTextItem = new EnumType(26, "MultiLineText_Item");
  m_enums.push_back(m_multiLineTextItem);

  m_editTemplate = new EnumType(27, "Edit_Template");
  m_enums.push_back(m_editTemplate);

  m_menuItem = new EnumType(28, "Menu_Item");
  m_enums.push_back(m_menuItem);

  m_textGridSettings = new EnumType(29, "Text_Grid_Settings");
  m_enums.push_back(m_textGridSettings);
}

te::layout::EnumType* te::layout::EnumObjectType::getRetangleItem() const
{
  return m_retangleItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getDefaultTextItem() const
{
  return m_defaultTextItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getMapItem() const
{
  return m_mapItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getMapGridItem() const
{
  return m_mapGridItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getPaperItem() const
{
  return m_paperItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getLegendItem() const
{
  return m_legendItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getPropertiesWindow() const
{
  return m_propertiesWindow;
}

te::layout::EnumType* te::layout::EnumObjectType::getDisplayWindow() const
{
  return m_displayWindow;
}

te::layout::EnumType* te::layout::EnumObjectType::getObjectInspectorWindow() const
{
  return m_objectInspectorWindow;
}

te::layout::EnumType* te::layout::EnumObjectType::getToolbar() const
{
  return m_toolbar;
}

te::layout::EnumType* te::layout::EnumObjectType::getGridSettings() const
{
  return m_gridSettings;
}

te::layout::EnumType* te::layout::EnumObjectType::getGridPlanar() const
{
  return m_gridPlanar;
}

te::layout::EnumType* te::layout::EnumObjectType::getGridGeodesic() const
{
  return m_gridGeodesic;
}

te::layout::EnumType* te::layout::EnumObjectType::getHorizontalRuler() const
{
  return m_horizontalRuler;
}

te::layout::EnumType* te::layout::EnumObjectType::getVerticalRuler() const
{
  return m_verticalRuler;
}

te::layout::EnumType* te::layout::EnumObjectType::getItemGroup() const
{
  return m_itemGroup;
}

te::layout::EnumType* te::layout::EnumObjectType::getScaleItem() const
{
  return m_scaleItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getPageSetup() const
{
  return m_pageSetup;
}

te::layout::EnumType* te::layout::EnumObjectType::getImageItem() const
{
  return m_imageItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getSystematicScale() const
{
  return m_systematicScale;
}

te::layout::EnumType* te::layout::EnumObjectType::getPointItem() const
{
  return m_pointItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getArrowItem() const
{
  return m_arrowItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getEllipseItem() const
{
  return m_ellipseItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getTitleItem() const
{
  return m_titleItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getTextGridItem() const
{
  return m_textGridItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getMultiLineTextItem() const
{
  return m_multiLineTextItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getEditTemplate() const
{
  return m_editTemplate;
}

te::layout::EnumType* te::layout::EnumObjectType::getMenuItem() const
{
  return m_menuItem;
}

te::layout::EnumType* te::layout::EnumObjectType::getTextGridSettings() const
{
  return m_textGridSettings;
}

te::layout::EnumType* te::layout::EnumObjectType::getObjectUnknown() const
{
  return m_objectUnknown;
}