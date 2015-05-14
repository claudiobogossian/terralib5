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
  GridMapModel(new PlanarGridSettingsConfigProperties),
  m_unit(StyleKilometer)
{
  m_type = Enums::getInstance().getEnumObjectType()->getGridPlanarItem();

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

te::layout::LayoutUnit te::layout::GridPlanarModel::getUnit()
{
  return m_unit;
}



