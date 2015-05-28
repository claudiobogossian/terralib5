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
#include "../core/property/GridSettingsConfigProperties.h"

te::layout::GridMapModel::GridMapModel() :
  m_mapName(""),
  m_settingsConfig(0),
  m_srid(TE_UNKNOWN_SRS),
  m_systematic(0),
  m_mapDisplacementX(0),
  m_mapDisplacementY(0),
  m_visible(true),  
  m_lneHrzGap(0),
  m_lneVrtGap(0),
  m_initialGridPointX(0),
  m_initialGridPointY(0),
  m_gridStyle(0),
  m_lineStyle(0),
  m_lineWidth(1),  
  m_pointTextSize(6),
  m_fontText("Arial"),
  m_visibleAllTexts(true),

  m_superscriptText(false),
  m_lneVrtDisplacement(1),
  m_lneHrzDisplacement(1),
  m_bottomText(true),
  m_leftText(true),
  m_rightText(true),
  m_topText(true),
  m_bottomRotateText(false),
  m_leftRotateText(false),
  m_rightRotateText(false),
  m_topRotateText(false)
{
  init();
}

 te::layout::GridMapModel::GridMapModel( GridSettingsConfigProperties* settingsConfig ) :
  m_mapName(""),
  m_settingsConfig(settingsConfig),
  m_srid(TE_UNKNOWN_SRS),
  m_systematic(0),
  m_mapDisplacementX(0),
  m_mapDisplacementY(0),
  m_visible(true),  
  m_lneHrzGap(0),
  m_lneVrtGap(0),
  m_initialGridPointX(0),
  m_initialGridPointY(0),
  m_gridStyle(0),
  m_lineStyle(0),
  m_lineWidth(1),  
  m_pointTextSize(6),
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
  init();
}

void te::layout::GridMapModel::init()
{
  m_gridStyle = Enums::getInstance().getEnumGridStyleType()->getStyleContinuous();
  m_lineStyle = Enums::getInstance().getEnumLineStyleType()->getStyleSolid();

  m_type = Enums::getInstance().getEnumObjectType()->getGridMapItem();

  m_borderColor = te::color::RGBAColor(0, 0, 0, 255);
  m_box = te::gm::Envelope(0., 0., 20., 20.);
  m_border = false;

  m_lineColor  = te::color::RGBAColor(0, 0, 0, 255);
  m_textColor = te::color::RGBAColor(0, 0, 0, 255);

  if(!m_settingsConfig)
  {
    m_settingsConfig = new GridSettingsConfigProperties;
  }
}

te::layout::GridMapModel::~GridMapModel()
{

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

void te::layout::GridMapModel::visitDependent( ContextItem context )
{
  MapModel* map = dynamic_cast<MapModel*>(m_visitable);

  if(map)
  {
    if(!map->isLoadedLayer())
    {
      return;
    }

    std::list<te::map::AbstractLayerPtr> layerListMap = map->getLayers();
    std::list<te::map::AbstractLayerPtr>::iterator it;
    it = layerListMap.begin();

    te::map::AbstractLayerPtr layer = (*it);

    m_srid = layer->getSRID();

    m_mapScale = map->getScale();
    m_boxMapMM = map->getMapBox();
    m_mapDisplacementX = map->getDisplacementX();
    m_mapDisplacementY = map->getDisplacementY();
    m_mapName = map->getName();
  }	
}

te::layout::Properties* te::layout::GridMapModel::getProperties() const
{
  ItemModelObservable::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pro_grid_name(m_hashCode);
  pro_grid_name.setName("GridSettings");
  pro_grid_name.setMenu(true);
  pro_grid_name.setPublic(true);
  pro_grid_name.setValue(m_name, dataType->getDataTypeGridSettings());  
  m_properties->addProperty(pro_grid_name);

  Property pro_mapName(m_hashCode);
  pro_mapName.setName(m_sharedProps->getMapName());
  pro_mapName.setComposeWidget(true);
  pro_mapName.setValue(m_mapName, dataType->getDataTypeStringList());
  
  Variant v;
  v.setValue(m_mapName, dataType->getDataTypeString());
  pro_mapName.addOption(v);
  m_properties->addProperty(pro_mapName);

  /* Grid */

  Property pro_visible(m_hashCode);
  pro_visible.setName(m_settingsConfig->getVisible());
  pro_visible.setComposeWidget(true);
  pro_visible.setValue(m_visible, dataType->getDataTypeBool());  
  m_properties->addProperty(pro_visible);

  Property pro_lneHrzGap(m_hashCode);
  pro_lneHrzGap.setName(m_settingsConfig->getLneHrzGap());
  pro_lneHrzGap.setComposeWidget(true);
  pro_lneHrzGap.setValue(m_lneHrzGap, dataType->getDataTypeDouble());  
  m_properties->addProperty(pro_lneHrzGap);

  Property pro_lneVrtGap(m_hashCode);
  pro_lneVrtGap.setName(m_settingsConfig->getLneVrtGap());
  pro_lneVrtGap.setComposeWidget(true);
  pro_lneVrtGap.setValue(m_lneVrtGap, dataType->getDataTypeDouble());  
  m_properties->addProperty(pro_lneVrtGap);

  Property pro_initialGridPointX(m_hashCode);
  pro_initialGridPointX.setName(m_settingsConfig->getInitialGridPointX());
  pro_initialGridPointX.setComposeWidget(true);
  pro_initialGridPointX.setValue(m_initialGridPointX, dataType->getDataTypeDouble());  
  m_properties->addProperty(pro_initialGridPointX);

  Property pro_initialGridPointY(m_hashCode);
  pro_initialGridPointY.setName(m_settingsConfig->getInitialGridPointY());
  pro_initialGridPointY.setComposeWidget(true);
  pro_initialGridPointY.setValue(m_initialGridPointY, dataType->getDataTypeDouble());  
  m_properties->addProperty(pro_initialGridPointY);

  /* Just one is visible */
  Property pro_gridStyle(m_hashCode);
  pro_gridStyle.setName(m_settingsConfig->getStyle());
  pro_gridStyle.setComposeWidget(true);
  pro_gridStyle.setValue(m_gridStyle->getName(), dataType->getDataTypeString());  
  m_properties->addProperty(pro_gridStyle);

  /* Line */
  Property pro_lineStyle(m_hashCode);
  pro_lineStyle.setName(m_settingsConfig->getLineStyle());
  pro_lineStyle.setComposeWidget(true);
  pro_lineStyle.setValue(m_lineStyle->getName(), dataType->getDataTypeString());  
  m_properties->addProperty(pro_lineStyle);

  Property pro_lineColor(m_hashCode);
  pro_lineColor.setName(m_settingsConfig->getLineColor());
  pro_lineColor.setComposeWidget(true);
  pro_lineColor.setValue(m_lineColor, dataType->getDataTypeColor());  
  m_properties->addProperty(pro_lineColor);

  Property pro_lineWidth(m_hashCode);
  pro_lineWidth.setName(m_settingsConfig->getLineWidth());
  pro_lineWidth.setComposeWidget(true);
  pro_lineWidth.setValue(m_lineWidth, dataType->getDataTypeInt());  
  m_properties->addProperty(pro_lineWidth);

  /* Text: Basic Configuration */
  Property pro_pointTextSize(m_hashCode);
  pro_pointTextSize.setName(m_settingsConfig->getPointTextSize());
  pro_pointTextSize.setComposeWidget(true);
  pro_pointTextSize.setValue(m_pointTextSize, dataType->getDataTypeInt());  
  m_properties->addProperty(pro_pointTextSize);

  Property pro_fontText(m_hashCode);
  pro_fontText.setName(m_settingsConfig->getFontText());
  pro_fontText.setComposeWidget(true);
  pro_fontText.setValue(m_fontText, dataType->getDataTypeString());  
  m_properties->addProperty(pro_fontText);

  Property pro_textColor(m_hashCode);
  pro_textColor.setName(m_settingsConfig->getTextColor());
  pro_textColor.setComposeWidget(true);
  pro_textColor.setValue(m_textColor, dataType->getDataTypeColor());  
  m_properties->addProperty(pro_textColor);

  /* Text: Advanced configuration */
  Property pro_visibleAllTexts(m_hashCode);
  pro_visibleAllTexts.setName(m_settingsConfig->getVisibleAllTexts());
  pro_visibleAllTexts.setComposeWidget(true);
  pro_visibleAllTexts.setValue(m_visibleAllTexts, dataType->getDataTypeBool());  
  m_properties->addProperty(pro_visibleAllTexts);

  Property pro_superscriptText(m_hashCode);
  pro_superscriptText.setName(m_settingsConfig->getSuperscriptText());
  pro_superscriptText.setComposeWidget(true);
  pro_superscriptText.setValue(m_superscriptText, dataType->getDataTypeBool());  
  m_properties->addProperty(pro_superscriptText);

  Property pro_lneVrtDisplacement(m_hashCode);
  pro_lneVrtDisplacement.setName(m_settingsConfig->getLneVrtDisplacement());
  pro_lneVrtDisplacement.setComposeWidget(true);
  pro_lneVrtDisplacement.setValue(m_lneVrtDisplacement, dataType->getDataTypeDouble());  
  m_properties->addProperty(pro_lneVrtDisplacement);

  Property pro_lneHrzDisplacement(m_hashCode);
  pro_lneHrzDisplacement.setName(m_settingsConfig->getLneHrzDisplacement());
  pro_lneHrzDisplacement.setComposeWidget(true);
  pro_lneHrzDisplacement.setValue(m_lneHrzDisplacement, dataType->getDataTypeDouble());  
  m_properties->addProperty(pro_lneHrzDisplacement);

  Property pro_bottomText(m_hashCode);
  pro_bottomText.setName(m_settingsConfig->getBottomText());
  pro_bottomText.setComposeWidget(true);
  pro_bottomText.setValue(m_bottomText, dataType->getDataTypeBool());  
  m_properties->addProperty(pro_bottomText);

  Property pro_leftText(m_hashCode);
  pro_leftText.setName(m_settingsConfig->getLeftText());
  pro_leftText.setComposeWidget(true);
  pro_leftText.setValue(m_leftText, dataType->getDataTypeBool());  
  m_properties->addProperty(pro_leftText);

  Property pro_rightText(m_hashCode);
  pro_rightText.setName(m_settingsConfig->getRightText());
  pro_rightText.setComposeWidget(true);
  pro_rightText.setValue(m_rightText, dataType->getDataTypeBool());  
  m_properties->addProperty(pro_rightText);

  Property pro_topText(m_hashCode);
  pro_topText.setName(m_settingsConfig->getTopText());
  pro_topText.setComposeWidget(true);
  pro_topText.setValue(m_topText, dataType->getDataTypeBool());  
  m_properties->addProperty(pro_topText);

  Property pro_bottomRotateText(m_hashCode);
  pro_bottomRotateText.setName(m_settingsConfig->getBottomRotateText());
  pro_bottomRotateText.setComposeWidget(true);
  pro_bottomRotateText.setValue(m_bottomRotateText, dataType->getDataTypeBool());  
  m_properties->addProperty(pro_bottomRotateText);

  Property pro_leftRotateText(m_hashCode);
  pro_leftRotateText.setName(m_settingsConfig->getLeftRotateText());
  pro_leftRotateText.setComposeWidget(true);
  pro_leftRotateText.setValue(m_leftRotateText, dataType->getDataTypeBool());  
  m_properties->addProperty(pro_leftRotateText);

  Property pro_rightRotateText(m_hashCode);
  pro_rightRotateText.setName(m_settingsConfig->getRightRotateText());
  pro_rightRotateText.setComposeWidget(true);
  pro_rightRotateText.setValue(m_rightRotateText, dataType->getDataTypeBool());  
  m_properties->addProperty(pro_rightRotateText);

  Property pro_topRotateText(m_hashCode);
  pro_topRotateText.setName(m_settingsConfig->getTopRotateText());
  pro_topRotateText.setComposeWidget(true);
  pro_topRotateText.setValue(m_topRotateText, dataType->getDataTypeBool());  
  m_properties->addProperty(pro_topRotateText);

  return m_properties;
}

void te::layout::GridMapModel::updateProperties( te::layout::Properties* properties, bool notify )
{
  ItemModelObservable::updateProperties(properties, false);

  Properties* vectorProps = const_cast<Properties*>(properties);

  if(!vectorProps)
  {
    return;
  }

  Property pro_mapName = vectorProps->contains(m_sharedProps->getMapName());

  if(!pro_mapName.isNull())
  {
    m_mapName = pro_mapName.getOptionByCurrentChoice().toString();
  }

  Property pro_visible = vectorProps->contains(m_settingsConfig->getVisible());

  if(!pro_visible.isNull())
  {
    m_visible = pro_visible.getValue().toBool();
  }

  Property pro_lneHrzGap = vectorProps->contains(m_settingsConfig->getLneHrzGap());
  if(!pro_lneHrzGap.isNull())
  {
    m_lneHrzGap = pro_lneHrzGap.getValue().toDouble();
  }

  Property pro_lneVrtGap = vectorProps->contains(m_settingsConfig->getLneVrtGap());
  if(!pro_lneVrtGap.isNull())
  {
    m_lneVrtGap = pro_lneVrtGap.getValue().toDouble();
  }

  Property pro_initialGridPointX = vectorProps->contains(m_settingsConfig->getInitialGridPointX());
  if(!pro_initialGridPointX.isNull())
  {
    m_initialGridPointX = pro_initialGridPointX.getValue().toDouble();
  }

  Property pro_initialGridPointY = vectorProps->contains(m_settingsConfig->getInitialGridPointY());
  if(!pro_initialGridPointY.isNull())
  {
    m_initialGridPointY = pro_initialGridPointY.getValue().toDouble();
  }

  Property pro_gridStyle = vectorProps->contains(m_settingsConfig->getStyle());
  if(!pro_gridStyle.isNull())
  {
    std::string style = pro_gridStyle.getValue().toString();
    EnumType* styleType = Enums::getInstance().getEnumGridStyleType()->getEnum(style);
    if(!styleType)
    {
      styleType = Enums::getInstance().getEnumGridStyleType()->searchLabel(style);
    }
    if(styleType)
      m_gridStyle = styleType;
  }

  Property pro_lineStyle = vectorProps->contains(m_settingsConfig->getLineStyle());
  if(!pro_lineStyle.isNull())
  {
    std::string style = pro_lineStyle.getValue().toString();
    EnumType* lineStyle = Enums::getInstance().getEnumLineStyleType()->getEnum(style);
    if(!lineStyle)
    {
      lineStyle = Enums::getInstance().getEnumLineStyleType()->searchLabel(style);
    }
    if(lineStyle)
      m_lineStyle = lineStyle;
  }

  Property pro_lineColor = vectorProps->contains(m_settingsConfig->getLineColor());
  if(!pro_lineColor.isNull())
  {
    m_lineColor = pro_lineColor.getValue().toColor();
  }

  Property pro_lineWidth = vectorProps->contains(m_settingsConfig->getLineWidth());
  if(!pro_lineWidth.isNull())
  {
    m_lineWidth = pro_lineWidth.getValue().toInt();
  }

  Property pro_pointTextSize = vectorProps->contains(m_settingsConfig->getPointTextSize());
  if(!pro_pointTextSize.isNull())
  {
    m_pointTextSize = pro_pointTextSize.getValue().toInt();
  }

  Property pro_fontText = vectorProps->contains(m_settingsConfig->getFontText());
  if(!pro_fontText.isNull())
  {
    m_fontText= pro_fontText.getValue().toString();
  }

  Property pro_textColor = vectorProps->contains(m_settingsConfig->getTextColor());
  if(!pro_textColor.isNull())
  {
    m_textColor = pro_textColor.getValue().toColor();
  }

  Property pro_visibleAllTexts = vectorProps->contains(m_settingsConfig->getVisibleAllTexts());
  if(!pro_visibleAllTexts.isNull())
  {
    m_visibleAllTexts = pro_visibleAllTexts.getValue().toBool();
  }

  Property pro_superscriptText = vectorProps->contains(m_settingsConfig->getSuperscriptText());
  if(!pro_superscriptText.isNull())
  {
    m_superscriptText = pro_superscriptText.getValue().toBool();
  }

  Property pro_lneVrtDisplacement = vectorProps->contains(m_settingsConfig->getLneVrtDisplacement());
  if(!pro_lneVrtDisplacement.isNull())
  {
    m_lneVrtDisplacement = pro_lneVrtDisplacement.getValue().toInt();
  }

  Property pro_lneHrzDisplacement = vectorProps->contains(m_settingsConfig->getLneHrzDisplacement());
  if(!pro_lneHrzDisplacement.isNull())
  {
    m_lneHrzDisplacement = pro_lneHrzDisplacement.getValue().toInt();
  }

  Property pro_bottomText = vectorProps->contains(m_settingsConfig->getBottomText());
  if(!pro_bottomText.isNull())
  {
    m_bottomText = pro_bottomText.getValue().toBool();
  }

  Property pro_leftText = vectorProps->contains(m_settingsConfig->getLeftText());
  if(!pro_leftText.isNull())
  {
    m_leftText = pro_leftText.getValue().toBool();
  }

  Property pro_rightText = vectorProps->contains(m_settingsConfig->getRightText());
  if(!pro_rightText.isNull())
  {
    m_rightText = pro_rightText.getValue().toBool();
  }

  Property pro_topText = vectorProps->contains(m_settingsConfig->getTopText());
  if(!pro_topText.isNull())
  {
    m_topText = pro_topText.getValue().toBool();
  }

  Property pro_bottomRotateText = vectorProps->contains(m_settingsConfig->getBottomRotateText());
  if(!pro_bottomRotateText.isNull())
  {
    m_bottomRotateText = pro_bottomRotateText.getValue().toBool();
  }

  Property pro_leftRotateText = vectorProps->contains(m_settingsConfig->getLeftRotateText());
  if(!pro_leftRotateText.isNull())
  {
    m_leftRotateText = pro_leftRotateText.getValue().toBool();
  }

  Property pro_rightRotateText = vectorProps->contains(m_settingsConfig->getRightRotateText());
  if(!pro_rightRotateText.isNull())
  {
    m_rightRotateText = pro_rightRotateText.getValue().toBool();
  }

  Property pro_topRotateText = vectorProps->contains(m_settingsConfig->getTopRotateText());
  if(!pro_topRotateText.isNull())
  {
    m_topRotateText = pro_topRotateText.getValue().toBool();
  }

  if(notify)
  {
    ContextItem context;
    notifyAll(context);
  }
}

void te::layout::GridMapModel::calculateGaps( te::gm::Envelope box )
{
  //Do nothing
}

void te::layout::GridMapModel::setMapName( std::string name )
{
  m_mapName = name;
}

double te::layout::GridMapModel::getMapDisplacementX()
{
  return m_mapDisplacementX;
}

double te::layout::GridMapModel::getMapDisplacementY()
{
  return m_mapDisplacementY;
}

std::string te::layout::GridMapModel::getMapName()
{
  return m_mapName;
}

double te::layout::GridMapModel::getMapScale()
{
  return m_mapScale;
}

int te::layout::GridMapModel::getSRID()
{
  return m_srid;
}

te::gm::Envelope te::layout::GridMapModel::getBoxMapMM()
{
  return m_boxMapMM;
}

te::gm::Envelope te::layout::GridMapModel::getWorldBox()
{
  return m_worldBox;
}

double te::layout::GridMapModel::getLneHrzGap()
{
  return m_lneHrzGap;
}

double te::layout::GridMapModel::getLneVrtGap()
{
  return m_lneVrtGap;
}

double te::layout::GridMapModel::getInitialGridPointX()
{
  return m_initialGridPointX;
}

double te::layout::GridMapModel::getInitialGridPointY()
{
  return m_initialGridPointY;
}

te::layout::EnumType* te::layout::GridMapModel::getGridStyle()
{
  return m_gridStyle;
}

te::layout::EnumType* te::layout::GridMapModel::getLineStyle()
{
  return m_lineStyle;
}

te::color::RGBAColor te::layout::GridMapModel::getLineColor()
{
  return m_lineColor;
}

int te::layout::GridMapModel::getLineWidth()
{
  return m_lineWidth;
}

int te::layout::GridMapModel::getTextPointSize()
{
  return m_pointTextSize;
}

std::string te::layout::GridMapModel::getFontFamily()
{
  return m_fontText;
}

te::color::RGBAColor te::layout::GridMapModel::getTextColor()
{
  return m_textColor;
}

bool te::layout::GridMapModel::isVisibleAllTexts()
{
  return m_visibleAllTexts;
}

bool te::layout::GridMapModel::isSuperscriptText()
{
  return m_superscriptText;
}

double te::layout::GridMapModel::getLneVrtDisplacement()
{
  return m_lneVrtDisplacement;
}

double te::layout::GridMapModel::getLneHrzDisplacement()
{
  return m_lneHrzDisplacement;
}

bool te::layout::GridMapModel::isBottomText()
{
  return m_bottomText;
}

bool te::layout::GridMapModel::isLeftText()
{
  return m_leftText;
}

bool te::layout::GridMapModel::isRightText()
{
  return m_rightText;
}

bool te::layout::GridMapModel::isTopText()
{
  return m_topText;
}

bool te::layout::GridMapModel::isBottomRotateText()
{
  return m_bottomRotateText;
}

bool te::layout::GridMapModel::isLeftRotateText()
{
  return m_leftRotateText;
}

bool te::layout::GridMapModel::isRightRotateText()
{
  return m_rightRotateText;
}

bool te::layout::GridMapModel::isTopRotateText()
{
  return m_topRotateText;
}




