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
  \file VerticalRulerLayoutModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "VerticalRulerLayoutModel.h"
#include "ContextLayoutItem.h"
#include "LayoutItemModelObservable.h"
#include "LayoutContext.h"
#include "LayoutMode.h"
#include "LayoutUtils.h"
#include "../../../maptools/Canvas.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Point.h"
#include "../../../color/RGBAColor.h"

// STL
#include <sstream>
#include <string>     // std::string, std::to_string

te::layout::VerticalRulerLayoutModel::VerticalRulerLayoutModel() :
m_visibleVerticalRuler(true),
m_spacing(1.),
m_verticalBlockMarks(15),
m_blockSize(10),
m_middleBlockSize(5),
m_longLine(6.),
m_mediumLine(4.),
m_smallLine(3.),
m_borderLineSize(0.3),
m_posCount(0),
m_lineMargin(2.),
m_invertedLines(false),
m_backEndSpacing(10),
m_backEndMargin(2.)
{
  m_box = te::gm::Envelope(0., 0., 150., 150.);
  m_paperBox = te::gm::Envelope(0., 0., 210., 297.); // A4 default
  m_backEndBox = te::gm::Envelope(0., 0., 150., 150.);

  updateVerticalListText();
}

te::layout::VerticalRulerLayoutModel::~VerticalRulerLayoutModel()
{

}

void te::layout::VerticalRulerLayoutModel::updateObserver( ContextLayoutItem context )
{

}

void te::layout::VerticalRulerLayoutModel::draw( ContextLayoutItem context )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();
  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();
 
  if(context.isResizeCanvas())
    utils->configCanvas(m_box);  
  
  drawRuler(canvas, utils);

  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);
  
  ContextLayoutItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

void te::layout::VerticalRulerLayoutModel::drawRuler( te::map::Canvas* canvas, LayoutUtils* utils )
{
  te::gm::Envelope envPaper;
  te::gm::Envelope envMargin;
  
  if(m_visibleVerticalRuler)
  {
    te::color::RGBAColor colorp4(180,180,180, TE_OPAQUE);
    canvas->setPolygonFillColor(colorp4);
    canvas->setPolygonContourColor(colorp4);
    //Cor 80
    utils->drawRectW(m_box);

    te::color::RGBAColor colorp85(145,145,145, TE_OPAQUE);
    canvas->setPolygonFillColor(colorp85);
    canvas->setPolygonContourColor(colorp85);
    //Cor 80
    utils->drawRectW(m_backEndBox);

    envPaper = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_paperBox.getLowerLeftY(),
      m_backEndBox.getUpperRightX(), m_paperBox.getUpperRightY());

    te::color::RGBAColor colorp2(255,255,255, TE_OPAQUE);
    canvas->setPolygonFillColor(colorp2);
    canvas->setPolygonContourColor(colorp2);
    //Cor 255
    utils->drawRectW(envPaper);

    drawVerticalRuler(canvas, utils);

    te::color::RGBAColor colorp3(0,0,0, TE_OPAQUE);
    canvas->setLineColor(colorp3);
    //Cor 0
    if(m_invertedLines)
      envMargin = te::gm::Envelope(m_backEndBox.getUpperRightX() - m_lineMargin, m_backEndBox.getLowerLeftY(), 
      m_backEndBox.getUpperRightX() - m_lineMargin, m_backEndBox.getHeight());
    else
    envMargin = te::gm::Envelope(m_lineMargin, m_backEndBox.getLowerLeftY(), m_lineMargin, 
      m_backEndBox.getHeight());

    te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
    line = utils->createSimpleLine(envMargin);
    utils->drawLineW(line);
    if(line) delete line;
  } 
}

void te::layout::VerticalRulerLayoutModel::drawVerticalRuler(te::map::Canvas* canvas, LayoutUtils* utils)
{
  te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
  te::gm::Envelope box;
  m_posCount = m_backEndBox.getLowerLeftY();

  te::color::RGBAColor colorp6(0,0,0, TE_OPAQUE);
  canvas->setLineColor(colorp6);
  for(int i = 0 ; i < m_verticalBlockMarks ; ++i )
  {
    //TypeRulerVertical
    if(m_invertedLines)
      box = te::gm::Envelope(m_backEndBox.getUpperRightX(), m_posCount, m_backEndBox.getUpperRightX() - m_longLine, m_posCount);
    else
      box = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_posCount, m_backEndBox.getLowerLeftX() + m_longLine, m_posCount);
    line = utils->createSimpleLine(box);
    utils->drawLineW(line);
    if(line) delete line;
    if(m_invertedLines)
      canvas->drawText(m_backEndBox.getUpperRightX() - (m_longLine + 1.), m_posCount, m_verticalTexts[i], -90);
    else
      canvas->drawText(m_backEndBox.getLowerLeftX() + (m_longLine + 1.), m_posCount, m_verticalTexts[i], -90);
    drawMarks(canvas, utils, m_blockSize - 1);
    m_posCount += 1;
  }	
  m_posCount = 0;
}

void te::layout::VerticalRulerLayoutModel::drawMarks(te::map::Canvas* canvas, LayoutUtils* utils, int marks)
{
  te::gm::Envelope box;
  te::gm::LinearRing* line = 0;
  m_posCount += 1;
  if(marks == m_middleBlockSize)
  {      
      //TypeRulerVertical
      if(m_invertedLines)
        box = te::gm::Envelope(m_backEndBox.getUpperRightX(), m_posCount, m_backEndBox.getUpperRightX() - m_mediumLine, m_posCount);
      else
        box = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_posCount, m_backEndBox.getLowerLeftX() + m_mediumLine, m_posCount);
      line = utils->createSimpleLine(box);
      utils->drawLineW(line);    
      if(line) delete line;
  }
  else
  { 
      //TypeRulerVertical
      if(m_invertedLines)
        box = te::gm::Envelope(m_backEndBox.getUpperRightX(), m_posCount, m_backEndBox.getUpperRightX() - m_smallLine, m_posCount);
      else
        box = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_posCount, m_backEndBox.getLowerLeftX() + m_smallLine, m_posCount);
      line = utils->createSimpleLine(box);
      utils->drawLineW(line);
      if(line) delete line;
      drawMarks(canvas, utils, marks - 1);
      m_posCount += 1;
      //TypeRulerVertical
      if(m_invertedLines)
        box = te::gm::Envelope(m_backEndBox.getUpperRightX(), m_posCount, m_backEndBox.getUpperRightX() - m_smallLine, m_posCount);
      else
        box = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_posCount, m_backEndBox.getLowerLeftX() + m_smallLine, m_posCount);
      line = utils->createSimpleLine(box);
      utils->drawLineW(line);
      if(line) delete line;
  }
}

void te::layout::VerticalRulerLayoutModel::updateVerticalListText()
{
  m_verticalTexts.clear();
  int y = (int)m_backEndBox.getLowerLeftY();
  int totaly = (int)m_backEndBox.getUpperRightY();

  int lastDigit = std::abs(y % 10);
  int is_dozen = m_blockSize - lastDigit;

  int count = y;
  bool is_negative = count < 0;

  //Round number
  if(is_dozen > 0 && is_dozen < 10)
  {
    if(is_negative)
      count = count - is_dozen; 
    else
      count = count + is_dozen; 
  }

  while(count <= totaly)
  {
    std::stringstream ss;//create a stringstream
    ss << count;//add number to the stream
    m_verticalTexts.push_back(ss.str());
    count += m_blockSize;
  }
}

void te::layout::VerticalRulerLayoutModel::setBox( te::gm::Envelope box )
{
  if(box.getWidth() < 3. || box.getHeight() < 10.)
    return;

  m_box = box;
  m_backEndBox = te::gm::Envelope(m_box.getLowerLeftX() + m_backEndMargin, m_box.getLowerLeftY(),
    m_box.getUpperRightX() - m_backEndMargin , m_box.getUpperRightY() - m_backEndSpacing);
  m_verticalBlockMarks = (int)std::abs(std::ceil(m_backEndBox.getHeight() / m_blockSize));
  updateVerticalListText();
}

void te::layout::VerticalRulerLayoutModel::setVisibleVerticalRuler( bool visible )
{
  m_visibleVerticalRuler = visible;
}

bool te::layout::VerticalRulerLayoutModel::isVisibleVerticalRuler()
{
  return m_visibleVerticalRuler;
}

void te::layout::VerticalRulerLayoutModel::setPaperBox( te::gm::Envelope box )
{
  m_paperBox = box;
}

te::gm::Envelope te::layout::VerticalRulerLayoutModel::getPaperBox()
{
  return m_paperBox;
}

void te::layout::VerticalRulerLayoutModel::invertedLines( bool invert )
{
  m_invertedLines = invert;
}
