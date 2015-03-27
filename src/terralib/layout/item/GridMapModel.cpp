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
  \file GridMapModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridMapModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Coord2D.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../../srs/Config.h"
#include "../core/enum/Enums.h"
#include "MapModel.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/property/SharedProperties.h"

te::layout::GridMapModel::GridMapModel() :
  m_mapName(""),
  m_name("GRID_MODEL"),
  m_srid(TE_UNKNOWN_SRS),
  m_systematic(0),
  m_mapDisplacementX(0),
  m_mapDisplacementY(0),
  m_visible(false),  
  m_lneHrzGap(0),
  m_lneVrtGap(0),
  m_initialGridPointX(0),
  m_initialGridPointY(0),
  m_gridStyle(0),
  m_lineStyle(0),
  m_lineWidth(1),  
  m_pointTextSize(12),
  m_fontText("Arial"),
  m_visibleAllTexts(true),

  m_superscriptText(false),
  m_lneVrtDisplacement(7),
  m_lneHrzDisplacement(7),
  m_bottomText(true),
  m_leftText(true),
  m_rightText(true),
  m_topText(true),
  m_bottomRotateText(false),
  m_leftRotateText(false),
  m_rightRotateText(false),
  m_topRotateText(false)
{
  m_gridStyle = Enums::getInstance().getEnumGridStyleType()->getStyleNone();
  m_lineStyle = Enums::getInstance().getEnumLineStyleType()->getStyleNone();

  m_type = Enums::getInstance().getEnumObjectType()->getGridMapItem();

  m_borderColor = te::color::RGBAColor(0, 0, 0, 255);
  m_box = te::gm::Envelope(0., 0., 20., 20.);
  m_border = false;
}

te::layout::GridMapModel::~GridMapModel()
{

}

std::string te::layout::GridMapModel::getName()
{
  return m_name;
}

void te::layout::GridMapModel::setName( std::string name )
{
  m_name = name;
}

bool te::layout::GridMapModel::isVisible()
{
  return m_visible;
}

void te::layout::GridMapModel::setVisible( bool visible )
{
  m_visible = visible;
}

void te::layout::GridMapModel::setMapScale( double scale )
{
  m_mapScale = scale;
}

void te::layout::GridMapModel::setWorldBox( te::gm::Envelope box )
{
  m_worldBox = box;
}

void te::layout::GridMapModel::setBoxMapMM( te::gm::Envelope box )
{
  m_boxMapMM = box;
}

void te::layout::GridMapModel::setBoundingBoxItemMM( te::gm::Envelope box )
{
  m_boundingBoxItemMM = box;
}

void te::layout::GridMapModel::setSystematic( Systematic* sys )
{
  m_systematic = sys;
}

void te::layout::GridMapModel::setVisibleAllTexts( bool visible )
{
  m_visibleAllTexts = visible;

  m_bottomText = visible;
  m_leftText = visible;
  m_rightText = visible;
  m_topText = visible;
}

bool te::layout::GridMapModel::isVisibleAllTexts()
{
  return m_visibleAllTexts;
}

void te::layout::GridMapModel::gridTextFreeMemory()
{
  std::map<te::gm::Point*, std::string>::iterator it;
  for(it = m_gridTexts.begin() ; it != m_gridTexts.end() ; ++it)
  {
    te::gm::Point* p = it->first;
    if(p)
    {
      delete p;
      p = 0;
    }
  }
  m_gridTexts.clear();
}

std::map<te::gm::Point*, std::string> te::layout::GridMapModel::getGridInfo()
{
  return m_gridTexts;
}

void te::layout::GridMapModel::setMapDisplacementX( double displacement )
{
  m_mapDisplacementX = displacement;
}

void te::layout::GridMapModel::setMapDisplacementY( double displacement )
{
  m_mapDisplacementY = displacement;
}

int te::layout::GridMapModel::getPointSize()
{
  return m_pointTextSize;
}

std::string te::layout::GridMapModel::getFontFamily()
{
  return m_fontText;
}

void te::layout::GridMapModel::visitDependent( ContextItem context )
{
  MapModel* map = dynamic_cast<MapModel*>(m_visitable);

  if(map)
  {
    draw(context);
  }	
}

te::layout::Properties* te::layout::GridMapModel::getProperties() const
{
  ItemModelObservable::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pro_mapName;
  pro_mapName.setName(m_sharedProps->getMapName());
  pro_mapName.setId("");
  pro_mapName.setValue(m_mapName, dataType->getDataTypeStringList());
  
  Variant v;
  v.setValue(m_mapName, dataType->getDataTypeString());
  pro_mapName.addOption(v);
  m_properties->addProperty(pro_mapName);

  return m_properties;
}

void te::layout::GridMapModel::updateProperties( te::layout::Properties* properties )
{
  ItemModelObservable::updateProperties(properties);

  Properties* vectorProps = const_cast<Properties*>(properties);

  Property pro_mapName = vectorProps->contains(m_sharedProps->getMapName());

  if(!pro_mapName.isNull())
  {
    m_mapName = pro_mapName.getOptionByCurrentChoice().toString();
  }
}

te::layout::Property te::layout::GridMapModel::getProperty()
{
  Property propertyGridMap;
  return propertyGridMap;
}

void te::layout::GridMapModel::updateProperty( Property property )
{
  
}

void te::layout::GridMapModel::calculateGaps( te::gm::Envelope box )
{

}
