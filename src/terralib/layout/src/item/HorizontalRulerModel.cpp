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
  \file HorizontalRulerModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "HorizontalRulerModel.h"
#include "ContextItem.h"
#include "ItemModelObservable.h"
#include "Context.h"
#include "EnumMode.h"
#include "Utils.h"
#include "../../../maptools/Canvas.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Point.h"
#include "../../../color/RGBAColor.h"

// STL
#include <sstream>
#include <string>    

te::layout::HorizontalRulerModel::HorizontalRulerModel() :
m_visibleHorizontalRuler(true),
m_spacing(1.),
m_horizontalBlockMarks(15),
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

  updateHorizontalListText();
}

te::layout::HorizontalRulerModel::~HorizontalRulerModel()
{

}

void te::layout::HorizontalRulerModel::updateObserver( ContextItem context )
{

}

void te::layout::HorizontalRulerModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = Context::getInstance()->getCanvas();
  Utils* utils = Context::getInstance()->getUtils();
  
  if(context.isResizeCanvas())
    utils->configCanvas(m_box);  
  
  drawRuler(canvas, utils);

  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);
  
  ContextItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

void te::layout::HorizontalRulerModel::drawRuler( te::map::Canvas* canvas, Utils* utils )
{
  te::gm::Envelope envPaper;
  te::gm::Envelope envMargin;
  
  if(m_visibleHorizontalRuler)
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
                
    envPaper = te::gm::Envelope(m_paperBox.getLowerLeftX(), m_backEndBox.getLowerLeftY(),
      m_paperBox.getUpperRightX(), m_backEndBox.getUpperRightY());

    te::color::RGBAColor colorp5(255,255,255, TE_OPAQUE);
    canvas->setPolygonFillColor(colorp5);
    canvas->setPolygonContourColor(colorp5);
    utils->drawRectW(envPaper);

    drawHorizontalRuler(canvas, utils);

    te::color::RGBAColor colorp6(0,0,0, TE_OPAQUE);
    canvas->setLineColor(colorp6);

    if(m_invertedLines)
      envMargin = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_lineMargin, m_backEndBox.getWidth(), 
      m_lineMargin);
    else
      envMargin = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_backEndBox.getUpperRightY() - m_lineMargin, m_backEndBox.getWidth(), 
      m_backEndBox.getUpperRightY() - m_lineMargin);

    te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
    line = utils->createSimpleLine(envMargin);
    utils->drawLineW(line);
    if(line) delete line;
  }
}

void te::layout::HorizontalRulerModel::drawHorizontalRuler(te::map::Canvas* canvas, Utils* utils)
{
  te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
  te::gm::Envelope box;
  m_posCount = m_backEndBox.getLowerLeftX();

  te::color::RGBAColor colorp6(0,0,0, TE_OPAQUE);
  canvas->setLineColor(colorp6);

  for(int i = 0 ; i < m_horizontalBlockMarks ; ++i )
  {
    //TypeRulerHorizontal
    if(m_invertedLines)
      box = te::gm::Envelope(m_posCount, m_backEndBox.getLowerLeftY(), m_posCount, m_backEndBox.getLowerLeftY() + m_longLine);
    else
     box = te::gm::Envelope(m_posCount, m_backEndBox.getUpperRightY(), m_posCount, m_backEndBox.getUpperRightY() - m_longLine);
    line = utils->createSimpleLine(box);
    utils->drawLineW(line);
    if(line) delete line;
    if(m_invertedLines)
      canvas->drawText(m_posCount, m_longLine + 1., m_horizontalTexts[i], 0);
    else      
      canvas->drawText(m_posCount, m_backEndBox.getUpperRightY() - (m_longLine + 1.), m_horizontalTexts[i], 0);
    
    drawMarks(canvas, utils, m_blockSize - 1);
    m_posCount += 1;
  }
  m_posCount = 0;
}

void te::layout::HorizontalRulerModel::drawMarks(te::map::Canvas* canvas, Utils* utils, int marks)
{
  te::gm::Envelope box;
  te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
  m_posCount += 1;
  if(marks == m_middleBlockSize)
  {
      //TypeRulerHorizontal
      if(m_invertedLines)
        box = te::gm::Envelope(m_posCount, m_backEndBox.getLowerLeftY(), m_posCount, m_backEndBox.getLowerLeftY() + m_mediumLine);
      else
        box = te::gm::Envelope(m_posCount, m_backEndBox.getUpperRightY(), m_posCount, m_backEndBox.getUpperRightY() - m_mediumLine);
      line = utils->createSimpleLine(box);
      utils->drawLineW(line);
      if(line) delete line;
  }
  else
  { 
      //TypeRulerHorizontal
      if(m_invertedLines)
        box = te::gm::Envelope(m_posCount, m_backEndBox.getLowerLeftY(), m_posCount, m_backEndBox.getLowerLeftY() + m_smallLine);
      else
        box = te::gm::Envelope(m_posCount, m_backEndBox.getUpperRightY(), m_posCount, m_backEndBox.getUpperRightY() - m_smallLine);
      line = utils->createSimpleLine(box);
      utils->drawLineW(line);
      if(line) delete line;

      drawMarks(canvas, utils, marks - 1);
      m_posCount += 1;
      //TypeRulerHorizontal
      if(m_invertedLines)
        box = te::gm::Envelope(m_posCount, m_backEndBox.getLowerLeftY(), m_posCount, m_backEndBox.getLowerLeftY() + m_smallLine);  
      else
        box = te::gm::Envelope(m_posCount, m_backEndBox.getUpperRightY(), m_posCount, m_backEndBox.getUpperRightY() - m_smallLine);  
      line = utils->createSimpleLine(box);
      utils->drawLineW(line);
      if(line) delete line;
  }
}

void te::layout::HorizontalRulerModel::updateHorizontalListText()
{
  m_horizontalTexts.clear();
  int x = (int)m_backEndBox.getLowerLeftX();
  int totalx = (int)m_backEndBox.getUpperRightX();
  
  int lastDigit = std::abs(x % 10);
  int is_dozen = m_blockSize - lastDigit;

  int count = x;
  bool is_negative = count < 0;

  //Round number
  if(is_dozen > 0 && is_dozen < 10)
  {
    if(is_negative)
      count = count - is_dozen; 
    else
      count = count + is_dozen; 
  }

  while(count <= totalx)
  {
    std::stringstream ss;//create a stringstream
    ss << count;//add number to the stream
    m_horizontalTexts.push_back(ss.str());
    count += m_blockSize;
  }
}

void te::layout::HorizontalRulerModel::setBox( te::gm::Envelope box )
{
  if(box.getWidth() < 15. || box.getHeight() < 3.)
    return;

  m_box = box;
  m_backEndBox = te::gm::Envelope(m_box.getLowerLeftX() + m_backEndSpacing, m_box.getLowerLeftY() + m_backEndMargin,
    m_box.getUpperRightX(), m_box.getUpperRightY() - m_backEndMargin);
  m_horizontalBlockMarks = (int)std::abs(std::ceil(m_backEndBox.getWidth() / m_blockSize));
  updateHorizontalListText();
}

void te::layout::HorizontalRulerModel::setVisibleHorizontalRuler( bool visible )
{
  m_visibleHorizontalRuler = visible;
}

bool te::layout::HorizontalRulerModel::isVisibleHorizontalRuler()
{
  return m_visibleHorizontalRuler;
}

void te::layout::HorizontalRulerModel::setPaperBox( te::gm::Envelope box )
{
  m_paperBox = box;
}

te::gm::Envelope te::layout::HorizontalRulerModel::getPaperBox()
{
  return m_paperBox;
}

void te::layout::HorizontalRulerModel::invertedLines( bool invert )
{
  m_invertedLines = invert;
}
