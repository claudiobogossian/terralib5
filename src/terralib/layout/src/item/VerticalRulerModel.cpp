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
  \file VerticalRulerModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "VerticalRulerModel.h"
#include "ContextItem.h"
#include "ItemModelObservable.h"
#include "Context.h"
#include "EnumMode.h"
#include "Utils.h"
#include "../../../maptools/Canvas.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Point.h"

// STL
#include <sstream>
#include <string>     // std::string, std::to_string

te::layout::VerticalRulerModel::VerticalRulerModel() :
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

te::layout::VerticalRulerModel::~VerticalRulerModel()
{

}

void te::layout::VerticalRulerModel::updateObserver( ContextItem context )
{

}

void te::layout::VerticalRulerModel::draw( ContextItem context )
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

void te::layout::VerticalRulerModel::drawRuler( te::map::Canvas* canvas, Utils* utils )
{
  te::gm::Envelope envPaper;
  te::gm::Envelope envMargin;
  
  if(m_visibleVerticalRuler)
  {
    te::color::RGBAColor colorp4(180,180,180, TE_OPAQUE);
    drawRectW(m_box, colorp4, canvas, utils);

    te::color::RGBAColor colorp85(145,145,145, TE_OPAQUE);
    drawRectW(m_backEndBox, colorp85, canvas, utils);

    envPaper = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_paperBox.getLowerLeftY(),
      m_backEndBox.getUpperRightX(), m_backEndBox.getUpperRightY());

    te::color::RGBAColor colorp2(255,255,255, TE_OPAQUE);
    drawRectW(envPaper, colorp2, canvas, utils);

    drawVerticalRuler(canvas, utils);

    te::color::RGBAColor colorp3(0,0,0, TE_OPAQUE);
    canvas->setLineColor(colorp3);

    //Cor 0
    envMargin = te::gm::Envelope(m_backEndBox.getLowerLeftX() + m_lineMargin, m_backEndBox.getLowerLeftY(), 
      m_backEndBox.getLowerLeftX() + m_lineMargin, m_backEndBox.getUpperRightY());

    drawLineW(envMargin, utils);
  } 
}

void te::layout::VerticalRulerModel::drawVerticalRuler(te::map::Canvas* canvas, Utils* utils)
{
  te::gm::Envelope box;
  m_posCount = m_backEndBox.getLowerLeftY();

  te::color::RGBAColor colorp6(0,0,0, TE_OPAQUE);
  canvas->setLineColor(colorp6);
  for(int i = 0 ; i < m_verticalBlockMarks ; ++i )
  {
    //TypeRulerVertical
    box = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_posCount, m_backEndBox.getLowerLeftX() + m_longLine, m_posCount);
    drawLineW(box, utils);

    canvas->drawText(m_backEndBox.getLowerLeftX() + (m_longLine + 1.), m_posCount, m_verticalTexts[i], -90);
    drawMarks(canvas, utils, m_blockSize - 1);
    m_posCount += 1;
  }	
  m_posCount = 0;
}

void te::layout::VerticalRulerModel::drawMarks(te::map::Canvas* canvas, Utils* utils, int marks)
{
  te::gm::Envelope box;
  m_posCount += 1;
  if(marks == m_middleBlockSize)
  {      
      //TypeRulerVertical
      box = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_posCount, m_backEndBox.getLowerLeftX() + m_mediumLine, m_posCount);
      drawLineW(box, utils);
  }
  else
  { 
      //TypeRulerVertical
      box = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_posCount, m_backEndBox.getLowerLeftX() + m_smallLine, m_posCount);
      drawLineW(box, utils);

      drawMarks(canvas, utils, marks - 1);
      m_posCount += 1;

      //TypeRulerVertical
      box = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_posCount, m_backEndBox.getLowerLeftX() + m_smallLine, m_posCount);
      drawLineW(box, utils);
  }
}

void te::layout::VerticalRulerModel::updateVerticalListText()
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

void te::layout::VerticalRulerModel::setBox( te::gm::Envelope box )
{
  if(box.getWidth() < 3. || box.getHeight() < 10.)
    return;

  m_box = box;
  m_backEndBox = te::gm::Envelope(m_box.getLowerLeftX() + m_backEndMargin, m_box.getLowerLeftY(),
    m_box.getUpperRightX() - m_backEndMargin , m_box.getUpperRightY() - m_backEndSpacing);
  m_verticalBlockMarks = (int)std::abs(std::ceil(m_backEndBox.getHeight() / m_blockSize));
  updateVerticalListText();
}

void te::layout::VerticalRulerModel::setVisibleVerticalRuler( bool visible )
{
  m_visibleVerticalRuler = visible;
}

bool te::layout::VerticalRulerModel::isVisibleVerticalRuler()
{
  return m_visibleVerticalRuler;
}

void te::layout::VerticalRulerModel::setPaperBox( te::gm::Envelope box )
{
  m_paperBox = box;
}

te::gm::Envelope te::layout::VerticalRulerModel::getPaperBox()
{
  return m_paperBox;
}

void te::layout::VerticalRulerModel::invertedLines( bool invert )
{
  m_invertedLines = invert;
}

void te::layout::VerticalRulerModel::drawLineW( te::gm::Envelope box, Utils* utils )
{
  te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
  line = utils->createSimpleLine(box);
  utils->drawLineW(line);
  if(line) delete line;
}

void te::layout::VerticalRulerModel::drawRectW( te::gm::Envelope box, te::color::RGBAColor color, 
  te::map::Canvas* canvas, Utils* utils )
{
  canvas->setPolygonFillColor(color);
  canvas->setPolygonContourColor(color);
  utils->drawRectW(box);
}
