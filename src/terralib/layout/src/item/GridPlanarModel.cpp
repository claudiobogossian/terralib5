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
  \file GridPlanarModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridPlanarModel.h"
#include "ContextItem.h"
#include "Context.h"
#include "../../../../../geometry/Envelope.h"
#include "../../../../../geometry/Coord2D.h"
#include "Property.h"
#include "EnumUtils.h"
#include "PlanarGridSettingsConfigProperties.h"

te::layout::GridPlanarModel::GridPlanarModel() :
  m_unit(StyleKilometer)
{
  m_settingsConfig = new PlanarGridSettingsConfigProperties;

  m_name = "GRID_PLANAR_MODEL";
  m_lneVrtDisplacement = 10;
  m_lneHrzDisplacement = 10;
}

te::layout::GridPlanarModel::~GridPlanarModel()
{
  if(m_settingsConfig)
  {
    delete m_settingsConfig;
    m_settingsConfig = 0;
  }
}

void te::layout::GridPlanarModel::draw( te::map::Canvas* canvas, te::gm::Envelope box, int srid )
{
  if(!m_visible)
    return;

  if(!box.isValid())
    return;

  m_srid = srid;

  calculateGaps(box);

  Utils* utils = Context::getInstance()->getUtils();

  te::color::RGBAColor color = te::color::RGBAColor(0, 0, 0, 255);
  canvas->setLineColor(color);
  
  drawVerticalLines(canvas, box);
  drawHorizontalLines(canvas, box);
}

void te::layout::GridPlanarModel::drawVerticalLines(te::map::Canvas* canvas, te::gm::Envelope box)
{
  // Draw a horizontal line and the y coordinate change(vertical)

  Utils* utils = Context::getInstance()->getUtils();

  double			y1;
  double			yInit;

  yInit = m_initialGridPointY;
  if(yInit < box.getLowerLeftY())
  {
    double dify = box.getLowerLeftY() - yInit;
    int nParts = (int)(dify/m_lneVrtGap);
    if(nParts == 0)
    {
      yInit = m_initialGridPointY;
    }
    else
    {
      yInit = yInit + (nParts * m_lneVrtGap);
    }
  }

  y1 = yInit;
  for( ; y1 < box.getUpperRightY() ; y1 += m_lneVrtGap)
  {
    if(y1 < box.getLowerLeftY())
      continue;

    te::gm::Envelope newBox(box.getLowerLeftX(), y1, 
      box.getUpperRightX(), y1);

    te::gm::LinearRing* line = 0;
    line = utils->createSimpleLine(newBox);
    utils->drawLineW(line);

    if(line)
    {
      delete line;
      line = 0;
    }
  }
}

void te::layout::GridPlanarModel::drawHorizontalLines(te::map::Canvas* canvas, te::gm::Envelope box)
{
  // Draw a vertical line and the x coordinate change(horizontal)

  Utils* utils = Context::getInstance()->getUtils();
  
  double			x1;
  double			xInit;
  
  xInit = m_initialGridPointX;
  if(xInit < box.getLowerLeftX())
  {
    double difx = box.getLowerLeftX() - xInit;
    int nParts = (int)(difx/m_lneHrzGap);
    if(nParts == 0)
    {
      xInit = m_initialGridPointX;
    }
    else
    {
      xInit = xInit + (nParts * m_lneHrzGap);
    }
  }

  x1 = xInit;

  for( ; x1 < box.getUpperRightX() ; x1 += m_lneHrzGap)
  {
    if(x1 < box.getLowerLeftX())
      continue;

    te::gm::Envelope newBox(x1, box.getLowerLeftY(), 
      x1, box.getUpperRightY());

    te::gm::LinearRing* line = 0;
    line = utils->createSimpleLine(newBox);
    utils->drawLineW(line);

    if(line)
    {
      delete line;
      line = 0;
    }
  }
}

void te::layout::GridPlanarModel::calculateGaps( te::gm::Envelope box )
{
  if(m_lneHrzGap <= 0)
  {
    m_lneHrzGap = m_mapScale * 0.05;
  }

  if(m_lneVrtGap <= 0)
  {
    m_lneVrtGap = m_lneHrzGap;
  }

  if(m_initialGridPointX <= 0)
  {
    m_initialGridPointX = box.getLowerLeftX();
  }
  if(m_initialGridPointY <= 0)
  {
    m_initialGridPointY = box.getLowerLeftY();
  }
}

te::layout::Property te::layout::GridPlanarModel::getProperty() 
{
  m_property.clear();

  m_property.setName(m_settingsConfig->getName());
  m_property.setId("unknown");
  std::string sValuePlanar = "Settings";
  m_property.setValue(sValuePlanar, DataTypeGridSettings);

  Property pro_name;
  pro_name.setName(m_settingsConfig->getName());
  pro_name.setValue(m_name, DataTypeString);
  m_property.addSubProperty(pro_name);
  
  /* Grid */

  Property pro_visible;
  pro_visible.setName(m_settingsConfig->getVisible());
  pro_visible.setValue(m_visible, DataTypeBool);  
  m_property.addSubProperty(pro_visible);

  Property pro_lneHrzGap;
  pro_lneHrzGap.setName(m_settingsConfig->getLneHrzGap());
  pro_lneHrzGap.setValue(m_lneHrzGap, DataTypeDouble);  
  m_property.addSubProperty(pro_lneHrzGap);

  Property pro_lneVrtGap;
  pro_lneVrtGap.setName(m_settingsConfig->getLneVrtGap());
  pro_lneVrtGap.setValue(m_lneVrtGap, DataTypeDouble);  
  m_property.addSubProperty(pro_lneVrtGap);

  Property pro_initialGridPointX;
  pro_initialGridPointX.setName(m_settingsConfig->getInitialGridPointX());
  pro_initialGridPointX.setValue(m_initialGridPointX, DataTypeDouble);  
  m_property.addSubProperty(pro_initialGridPointX);

  Property pro_initialGridPointY;
  pro_initialGridPointY.setName(m_settingsConfig->getInitialGridPointY());
  pro_initialGridPointY.setValue(m_initialGridPointY, DataTypeDouble);  
  m_property.addSubProperty(pro_initialGridPointY);

  /* Just one is visible */
  Property pro_gridStyle;
  pro_gridStyle.setName(m_settingsConfig->getStyle());
  pro_gridStyle.setValue(te::layout::getLayoutGridStyle(m_gridStyle), DataTypeString);  
  m_property.addSubProperty(pro_gridStyle);

  /* Line */
  Property pro_lineStyle;
  pro_lineStyle.setName(m_settingsConfig->getLineStyle());
  pro_lineStyle.setValue(te::layout::getLayoutDashStyle(m_lineStyle), DataTypeString);  
  m_property.addSubProperty(pro_lineStyle);

  Property pro_lineColor;
  pro_lineColor.setName(m_settingsConfig->getLineColor());
  pro_lineColor.setValue(m_lineColor, DataTypeColor);  
  m_property.addSubProperty(pro_lineColor);

  Property pro_lineWidth;
  pro_lineWidth.setName(m_settingsConfig->getLineWidth());
  pro_lineWidth.setValue(m_lineWidth, DataTypeInt);  
  m_property.addSubProperty(pro_lineWidth);

  /* Text: Basic Configuration */
  Property pro_pointTextSize;
  pro_pointTextSize.setName(m_settingsConfig->getPointTextSize());
  pro_pointTextSize.setValue(m_pointTextSize, DataTypeInt);  
  m_property.addSubProperty(pro_pointTextSize);

  Property pro_fontText;
  pro_fontText.setName(m_settingsConfig->getFontText());
  pro_fontText.setValue(m_fontText, DataTypeString);  
  m_property.addSubProperty(pro_fontText);

  Property pro_textColor;
  pro_textColor.setName(m_settingsConfig->getTextColor());
  pro_textColor.setValue(m_textColor, DataTypeColor);  
  m_property.addSubProperty(pro_textColor);

  /* Text: Advanced configuration */
  Property pro_visibleAllTexts;
  pro_visibleAllTexts.setName(m_settingsConfig->getVisibleAllTexts());
  pro_visibleAllTexts.setValue(m_visibleAllTexts, DataTypeBool);  
  m_property.addSubProperty(pro_visibleAllTexts);

  Property pro_superscriptText;
  pro_superscriptText.setName(m_settingsConfig->getSuperscriptText());
  pro_superscriptText.setValue(m_superscriptText, DataTypeBool);  
  m_property.addSubProperty(pro_superscriptText);

  Property pro_lneVrtDisplacement;
  pro_lneVrtDisplacement.setName(m_settingsConfig->getLneVrtDisplacement());
  pro_lneVrtDisplacement.setValue(m_lneVrtDisplacement, DataTypeDouble);  
  m_property.addSubProperty(pro_lneVrtDisplacement);

  Property pro_lneHrzDisplacement;
  pro_lneHrzDisplacement.setName(m_settingsConfig->getLneHrzDisplacement());
  pro_lneHrzDisplacement.setValue(m_lneHrzDisplacement, DataTypeDouble);  
  m_property.addSubProperty(pro_lneHrzDisplacement);

  Property pro_bottomText;
  pro_bottomText.setName(m_settingsConfig->getBottomText());
  pro_bottomText.setValue(m_bottomText, DataTypeBool);  
  m_property.addSubProperty(pro_bottomText);
  
  Property pro_leftText;
  pro_leftText.setName(m_settingsConfig->getLeftText());
  pro_leftText.setValue(m_leftText, DataTypeBool);  
  m_property.addSubProperty(pro_leftText);

  Property pro_rightText;
  pro_rightText.setName(m_settingsConfig->getRightText());
  pro_rightText.setValue(m_rightText, DataTypeBool);  
  m_property.addSubProperty(pro_rightText);

  Property pro_topText;
  pro_topText.setName(m_settingsConfig->getTopText());
  pro_topText.setValue(m_topText, DataTypeBool);  
  m_property.addSubProperty(pro_topText);
  
  Property pro_bottomRotateText;
  pro_bottomRotateText.setName(m_settingsConfig->getBottomRotateText());
  pro_bottomRotateText.setValue(m_bottomRotateText, DataTypeBool);  
  m_property.addSubProperty(pro_bottomRotateText);

  Property pro_leftRotateText;
  pro_leftRotateText.setName(m_settingsConfig->getLeftRotateText());
  pro_leftRotateText.setValue(m_leftRotateText, DataTypeBool);  
  m_property.addSubProperty(pro_leftRotateText);

  Property pro_rightRotateText;
  pro_rightRotateText.setName(m_settingsConfig->getRightRotateText());
  pro_rightRotateText.setValue(m_rightRotateText, DataTypeBool);  
  m_property.addSubProperty(pro_rightRotateText);
 
  Property pro_topRotateText;
  pro_topRotateText.setName(m_settingsConfig->getTopRotateText());
  pro_topRotateText.setValue(m_topRotateText, DataTypeBool);  
  m_property.addSubProperty(pro_topRotateText);

  //-----------------------------------------------------------

  return m_property;
}

void te::layout::GridPlanarModel::updateProperty( Property property )
{
  if(property.isNull())
  {
    return;
  }
  
  Property pro_visible = property.containsSubProperty(m_settingsConfig->getVisible());
  if(!pro_visible.isNull())
  {
    m_visible = pro_visible.getValue().toBool();
  }

  Property pro_lneHrzGap = property.containsSubProperty(m_settingsConfig->getLneHrzGap());
  if(!pro_lneHrzGap.isNull())
  {
    m_lneHrzGap = pro_lneHrzGap.getValue().toDouble();
  }

  Property pro_lneVrtGap = property.containsSubProperty(m_settingsConfig->getLneVrtGap());
  if(!pro_lneVrtGap.isNull())
  {
    m_lneVrtGap = pro_lneVrtGap.getValue().toDouble();
  }

  Property pro_initialGridPointX = property.containsSubProperty(m_settingsConfig->getInitialGridPointX());
  if(!pro_initialGridPointX.isNull())
  {
    m_initialGridPointX = pro_initialGridPointX.getValue().toDouble();
  }
  
  Property pro_initialGridPointY = property.containsSubProperty(m_settingsConfig->getInitialGridPointY());
  if(!pro_initialGridPointY.isNull())
  {
    m_initialGridPointY = pro_initialGridPointY.getValue().toDouble();
  }
  
  Property pro_gridStyle = property.containsSubProperty(m_settingsConfig->getStyle());
  if(!pro_gridStyle.isNull())
  {
    std::string style = pro_gridStyle.getValue().toString();
    m_gridStyle = te::layout::getLayoutEnumGridStyle(style);
  }

  Property pro_lineStyle = property.containsSubProperty(m_settingsConfig->getLineStyle());
  if(!pro_lineStyle.isNull())
  {
    std::string style = pro_lineStyle.getValue().toString();
    m_lineStyle = te::layout::getLayoutEnumDashStyle(style);
  }

  Property pro_lineColor = property.containsSubProperty(m_settingsConfig->getLineColor());
  if(!pro_lineColor.isNull())
  {
    m_lineColor = pro_lineColor.getValue().toColor();
  }

  Property pro_lineWidth = property.containsSubProperty(m_settingsConfig->getLineWidth());
  if(!pro_lineWidth.isNull())
  {
    m_lineWidth = pro_lineWidth.getValue().toInt();
  }
      
  Property pro_pointTextSize = property.containsSubProperty(m_settingsConfig->getPointTextSize());
  if(!pro_pointTextSize.isNull())
  {
    m_pointTextSize = pro_pointTextSize.getValue().toInt();
  }
  
  Property pro_fontText = property.containsSubProperty(m_settingsConfig->getFontText());
  if(!pro_fontText.isNull())
  {
    m_fontText= pro_fontText.getValue().toString();
  }

  Property pro_textColor = property.containsSubProperty(m_settingsConfig->getTextColor());
  if(!pro_textColor.isNull())
  {
    m_textColor = pro_textColor.getValue().toColor();
  }

  Property pro_visibleAllTexts = property.containsSubProperty(m_settingsConfig->getVisibleAllTexts());
  if(!pro_visibleAllTexts.isNull())
  {
    m_visibleAllTexts = pro_visibleAllTexts.getValue().toBool();
  }

  Property pro_superscriptText = property.containsSubProperty(m_settingsConfig->getSuperscriptText());
  if(!pro_superscriptText.isNull())
  {
    m_superscriptText = pro_superscriptText.getValue().toBool();
  }

  Property pro_lneVrtDisplacement = property.containsSubProperty(m_settingsConfig->getLneVrtDisplacement());
  if(!pro_lneVrtDisplacement.isNull())
  {
    m_lneVrtDisplacement = pro_lneVrtDisplacement.getValue().toInt();
  }

  Property pro_lneHrzDisplacement = property.containsSubProperty(m_settingsConfig->getLneHrzDisplacement());
  if(!pro_lneHrzDisplacement.isNull())
  {
    m_lneHrzDisplacement = pro_lneHrzDisplacement.getValue().toInt();
  }

  Property pro_bottomText = property.containsSubProperty(m_settingsConfig->getBottomText());
  if(!pro_bottomText.isNull())
  {
    m_bottomText = pro_bottomText.getValue().toBool();
  }

  Property pro_leftText = property.containsSubProperty(m_settingsConfig->getLeftText());
  if(!pro_leftText.isNull())
  {
    m_leftText = pro_leftText.getValue().toBool();
  }

  Property pro_rightText = property.containsSubProperty(m_settingsConfig->getRightText());
  if(!pro_rightText.isNull())
  {
    m_rightText = pro_rightText.getValue().toBool();
  }

  Property pro_topText = property.containsSubProperty(m_settingsConfig->getTopText());
  if(!pro_topText.isNull())
  {
    m_topText = pro_topText.getValue().toBool();
  }

  Property pro_bottomRotateText = property.containsSubProperty(m_settingsConfig->getBottomRotateText());
  if(!pro_bottomRotateText.isNull())
  {
    m_bottomRotateText = pro_bottomRotateText.getValue().toBool();
  }

  Property pro_leftRotateText = property.containsSubProperty(m_settingsConfig->getLeftRotateText());
  if(!pro_leftRotateText.isNull())
  {
    m_leftRotateText = pro_leftRotateText.getValue().toBool();
  }

  Property pro_rightRotateText = property.containsSubProperty(m_settingsConfig->getRightRotateText());
  if(!pro_rightRotateText.isNull())
  {
    m_rightRotateText = pro_rightRotateText.getValue().toBool();
  }

  Property pro_topRotateText = property.containsSubProperty(m_settingsConfig->getTopRotateText());
  if(!pro_topRotateText.isNull())
  {
    m_topRotateText = pro_topRotateText.getValue().toBool();
  }
}

