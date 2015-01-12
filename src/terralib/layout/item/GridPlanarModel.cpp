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
#include "../core/ContextItem.h"
#include "../core/pattern/singleton/Context.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Coord2D.h"
#include "../core/property/Property.h"
#include "../core/property/PlanarGridSettingsConfigProperties.h"
#include "../core/enum/Enums.h"

// STL
#include <string>
#include <sstream> 

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

void te::layout::GridPlanarModel::draw( te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box, int srid )
{
  if((!canvas) || (!utils))
    return;

  if(!box.isValid())
    return;

  calculateGaps(box);

  if(!m_visible)
    return;

  m_srid = srid;

  if(srid <= 0)
    return;
  
  te::color::RGBAColor color = te::color::RGBAColor(0, 0, 0, 255);
  canvas->setLineColor(color);
  canvas->setPolygonFillColor(color);
  canvas->setPolygonContourColor(color);
  
  canvas->setTextPointSize(m_pointTextSize);
  canvas->setFontFamily(m_fontText);

  gridTextFreeMemory();

  drawVerticalLines(canvas, utils, box);
  drawHorizontalLines(canvas, utils, box);
}

void te::layout::GridPlanarModel::drawVerticalLines(te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box)
{
  // Draw a horizontal line and the y coordinate change(vertical)
  
  double			y1;
  double			yInit;
  
  WorldTransformer transf = utils->getTransformGeo(box, m_boxMapMM);
  transf.setMirroring(false);

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

  utils = Context::getInstance().getUtils();

  double wtxt = 0;
  double htxt = 0;

  y1 = yInit;
  for( ; y1 < box.getUpperRightY() ; y1 += m_lneVrtGap)
  {
    if(y1 < box.getLowerLeftY())
      continue;
    
    double llx = 0;
    double urx = 0;
    double y = 0;
    transf.system1Tosystem2(box.getLowerLeftX(), y1, llx, y);
    transf.system1Tosystem2(box.getUpperRightX(), y1, urx, y);

    te::gm::Envelope newBox(llx, y, urx, y);

    te::gm::LinearRing* line = 0;
    line = utils->createSimpleLine(newBox);
    utils->drawLineW(line);
    
    std::ostringstream convert;
    convert.precision(10);
    double number = y1 / (double)m_unit;
    convert << number;

    utils->textBoundingBox(wtxt, htxt, convert.str());

    if(m_visibleAllTexts)
    {
      if(m_leftText)
      {
        canvas->drawText(llx - m_lneHrzDisplacement - wtxt, y, convert.str(), 0);
        te::gm::Point* coordLeft = new te::gm::Point(m_mapDisplacementX + llx - m_lneHrzDisplacement - wtxt, m_mapDisplacementY + y);
        m_gridTexts[coordLeft] = convert.str();
      }

      if(m_rightText)
      {
        canvas->drawText(urx + m_lneHrzDisplacement, y, convert.str(), 0);
        te::gm::Point* coordRight = new te::gm::Point(m_mapDisplacementX + urx + m_lneHrzDisplacement, m_mapDisplacementY + y);
        m_gridTexts[coordRight] = convert.str();
      }
    }

    if(line)
    {
      delete line;
      line = 0;
    }
  }
}

void te::layout::GridPlanarModel::drawHorizontalLines(te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box)
{
  // Draw a vertical line and the x coordinate change(horizontal)
    
  double			x1;
  double			xInit;
  
  WorldTransformer transf = utils->getTransformGeo(box, m_boxMapMM);
  transf.setMirroring(false);

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

  utils = Context::getInstance().getUtils();

  double wtxt = 0;
  double htxt = 0;

  x1 = xInit;
  for( ; x1 < box.getUpperRightX() ; x1 += m_lneHrzGap)
  {
    if(x1 < box.getLowerLeftX())
      continue;
    
    double lly = 0;
    double ury = 0;
    double x = 0;
    transf.system1Tosystem2(x1, box.getLowerLeftY(), x, lly);
    transf.system1Tosystem2(x1, box.getUpperRightY(), x, ury);
    
    te::gm::Envelope newBox(x, lly, x, ury);

    if(lly > ury)
    {
      double ycopy = lly;
      lly = ury;
      ury = ycopy;
    }

    te::gm::LinearRing* line = 0;
    line = utils->createSimpleLine(newBox);
    utils->drawLineW(line);

    std::ostringstream convert;
    convert.precision(10);
    double number = x1 / (double)m_unit;
    convert << number;

    utils->textBoundingBox(wtxt, htxt, convert.str());

    if(m_visibleAllTexts)
    {
      if(m_bottomText)
      {
        canvas->drawText(x - (wtxt/2.), lly - m_lneVrtDisplacement - htxt, convert.str(), 0);
        te::gm::Point* coordBottom = new te::gm::Point(m_mapDisplacementX + x - (wtxt/2.), m_mapDisplacementY + lly - m_lneVrtDisplacement - htxt);
        m_gridTexts[coordBottom] = convert.str();
      }

      if(m_topText)
      {
        canvas->drawText(x - (wtxt/2.), ury + m_lneVrtDisplacement, convert.str(), 0);
        te::gm::Point* coordTop = new te::gm::Point(m_mapDisplacementX + x - (wtxt/2.), m_mapDisplacementY + ury + m_lneVrtDisplacement);
        m_gridTexts[coordTop] = convert.str();
      }
    }

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

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  m_property.setName(m_settingsConfig->getName());
  m_property.setId("unknown");
  std::string sValuePlanar = "Settings";
  m_property.setValue(sValuePlanar, dataType->getDataTypeGridSettings());

  Property pro_name;
  pro_name.setName(m_settingsConfig->getName());
  pro_name.setValue(m_name, dataType->getDataTypeString());
  m_property.addSubProperty(pro_name);
  
  /* Grid */

  Property pro_visible;
  pro_visible.setName(m_settingsConfig->getVisible());
  pro_visible.setValue(m_visible, dataType->getDataTypeBool());  
  m_property.addSubProperty(pro_visible);

  Property pro_lneHrzGap;
  pro_lneHrzGap.setName(m_settingsConfig->getLneHrzGap());
  pro_lneHrzGap.setValue(m_lneHrzGap, dataType->getDataTypeDouble());  
  m_property.addSubProperty(pro_lneHrzGap);

  Property pro_lneVrtGap;
  pro_lneVrtGap.setName(m_settingsConfig->getLneVrtGap());
  pro_lneVrtGap.setValue(m_lneVrtGap, dataType->getDataTypeDouble());  
  m_property.addSubProperty(pro_lneVrtGap);

  Property pro_initialGridPointX;
  pro_initialGridPointX.setName(m_settingsConfig->getInitialGridPointX());
  pro_initialGridPointX.setValue(m_initialGridPointX, dataType->getDataTypeDouble());  
  m_property.addSubProperty(pro_initialGridPointX);

  Property pro_initialGridPointY;
  pro_initialGridPointY.setName(m_settingsConfig->getInitialGridPointY());
  pro_initialGridPointY.setValue(m_initialGridPointY, dataType->getDataTypeDouble());  
  m_property.addSubProperty(pro_initialGridPointY);

  /* Just one is visible */
  Property pro_gridStyle;
  pro_gridStyle.setName(m_settingsConfig->getStyle());
  pro_gridStyle.setValue(m_gridStyle->getName(), dataType->getDataTypeString());  
  m_property.addSubProperty(pro_gridStyle);

  /* Line */
  Property pro_lineStyle;
  pro_lineStyle.setName(m_settingsConfig->getLineStyle());
  pro_lineStyle.setValue(m_lineStyle->getName(), dataType->getDataTypeString());  
  m_property.addSubProperty(pro_lineStyle);

  Property pro_lineColor;
  pro_lineColor.setName(m_settingsConfig->getLineColor());
  pro_lineColor.setValue(m_lineColor, dataType->getDataTypeColor());  
  m_property.addSubProperty(pro_lineColor);

  Property pro_lineWidth;
  pro_lineWidth.setName(m_settingsConfig->getLineWidth());
  pro_lineWidth.setValue(m_lineWidth, dataType->getDataTypeInt());  
  m_property.addSubProperty(pro_lineWidth);

  /* Text: Basic Configuration */
  Property pro_pointTextSize;
  pro_pointTextSize.setName(m_settingsConfig->getPointTextSize());
  pro_pointTextSize.setValue(m_pointTextSize, dataType->getDataTypeInt());  
  m_property.addSubProperty(pro_pointTextSize);

  Property pro_fontText;
  pro_fontText.setName(m_settingsConfig->getFontText());
  pro_fontText.setValue(m_fontText, dataType->getDataTypeString());  
  m_property.addSubProperty(pro_fontText);

  Property pro_textColor;
  pro_textColor.setName(m_settingsConfig->getTextColor());
  pro_textColor.setValue(m_textColor, dataType->getDataTypeColor());  
  m_property.addSubProperty(pro_textColor);

  /* Text: Advanced configuration */
  Property pro_visibleAllTexts;
  pro_visibleAllTexts.setName(m_settingsConfig->getVisibleAllTexts());
  pro_visibleAllTexts.setValue(m_visibleAllTexts, dataType->getDataTypeBool());  
  m_property.addSubProperty(pro_visibleAllTexts);

  Property pro_superscriptText;
  pro_superscriptText.setName(m_settingsConfig->getSuperscriptText());
  pro_superscriptText.setValue(m_superscriptText, dataType->getDataTypeBool());  
  m_property.addSubProperty(pro_superscriptText);

  Property pro_lneVrtDisplacement;
  pro_lneVrtDisplacement.setName(m_settingsConfig->getLneVrtDisplacement());
  pro_lneVrtDisplacement.setValue(m_lneVrtDisplacement, dataType->getDataTypeDouble());  
  m_property.addSubProperty(pro_lneVrtDisplacement);

  Property pro_lneHrzDisplacement;
  pro_lneHrzDisplacement.setName(m_settingsConfig->getLneHrzDisplacement());
  pro_lneHrzDisplacement.setValue(m_lneHrzDisplacement, dataType->getDataTypeDouble());  
  m_property.addSubProperty(pro_lneHrzDisplacement);

  Property pro_bottomText;
  pro_bottomText.setName(m_settingsConfig->getBottomText());
  pro_bottomText.setValue(m_bottomText, dataType->getDataTypeBool());  
  m_property.addSubProperty(pro_bottomText);
  
  Property pro_leftText;
  pro_leftText.setName(m_settingsConfig->getLeftText());
  pro_leftText.setValue(m_leftText, dataType->getDataTypeBool());  
  m_property.addSubProperty(pro_leftText);

  Property pro_rightText;
  pro_rightText.setName(m_settingsConfig->getRightText());
  pro_rightText.setValue(m_rightText, dataType->getDataTypeBool());  
  m_property.addSubProperty(pro_rightText);

  Property pro_topText;
  pro_topText.setName(m_settingsConfig->getTopText());
  pro_topText.setValue(m_topText, dataType->getDataTypeBool());  
  m_property.addSubProperty(pro_topText);
  
  Property pro_bottomRotateText;
  pro_bottomRotateText.setName(m_settingsConfig->getBottomRotateText());
  pro_bottomRotateText.setValue(m_bottomRotateText, dataType->getDataTypeBool());  
  m_property.addSubProperty(pro_bottomRotateText);

  Property pro_leftRotateText;
  pro_leftRotateText.setName(m_settingsConfig->getLeftRotateText());
  pro_leftRotateText.setValue(m_leftRotateText, dataType->getDataTypeBool());  
  m_property.addSubProperty(pro_leftRotateText);

  Property pro_rightRotateText;
  pro_rightRotateText.setName(m_settingsConfig->getRightRotateText());
  pro_rightRotateText.setValue(m_rightRotateText, dataType->getDataTypeBool());  
  m_property.addSubProperty(pro_rightRotateText);
 
  Property pro_topRotateText;
  pro_topRotateText.setName(m_settingsConfig->getTopRotateText());
  pro_topRotateText.setValue(m_topRotateText, dataType->getDataTypeBool());  
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
    EnumType* styleType = Enums::getInstance().getEnumGridStyleType()->getEnum(style);
    m_gridStyle = styleType;
  }

  Property pro_lineStyle = property.containsSubProperty(m_settingsConfig->getLineStyle());
  if(!pro_lineStyle.isNull())
  {
    std::string style = pro_lineStyle.getValue().toString();
    EnumType* lineStyle = Enums::getInstance().getEnumLineStyleType()->getEnum(style);
    m_lineStyle = lineStyle;
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
