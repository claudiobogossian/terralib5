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

te::layout::VerticalRulerModel::VerticalRulerModel() 
{

}

te::layout::VerticalRulerModel::~VerticalRulerModel()
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
  if(!m_visible)
  {
    return;
  }

  te::gm::Envelope envPaper;
  te::gm::Envelope envMargin;

  te::color::RGBAColor colorp4(180,180,180, TE_OPAQUE);
  drawRectW(m_box, colorp4, canvas, utils);

  te::color::RGBAColor colorp85(145,145,145, TE_OPAQUE);
  drawRectW(m_backEndBox, colorp85, canvas, utils);

  envPaper = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_paperBox.getLowerLeftY(),
    m_backEndBox.getUpperRightX(), m_backEndBox.getUpperRightY());

  te::color::RGBAColor colorp2(255,255,255, TE_OPAQUE);
  drawRectW(envPaper, colorp2, canvas, utils);

  te::color::RGBAColor colorp3(0,0,0, TE_OPAQUE);
  canvas->setLineColor(colorp3);

  drawVerticalRuler(canvas, utils);

  envMargin = te::gm::Envelope(m_backEndBox.getLowerLeftX() + m_lineMargin, m_backEndBox.getLowerLeftY(), 
    m_backEndBox.getLowerLeftX() + m_lineMargin, m_backEndBox.getUpperRightY());

  drawLineW(envMargin, utils);
}

void te::layout::VerticalRulerModel::drawVerticalRuler(te::map::Canvas* canvas, Utils* utils)
{
  if(!m_box.isValid())
    return;

  double lly = m_backEndBox.getLowerLeftY();
  double ury = m_backEndBox.getUpperRightY();

  te::gm::Envelope box;

  for(int i = (int)lly ; i < (int) ury ; ++i)
  {
    if((i % (int)m_blockSize) == 0)
    {
      box = te::gm::Envelope(m_backEndBox.getLowerLeftX(), i, m_backEndBox.getLowerLeftX() + m_longLine, i);
      drawLineW(box, utils);

      std::stringstream ss;//create a stringstream
      ss << i;//add number to the stream

      canvas->drawText(m_backEndBox.getLowerLeftX() + (m_longLine + 1.), (double)i, ss.str(), -90);
    }
    else if((i % (int)m_middleBlockSize) == 0)
    {
      box = te::gm::Envelope(m_backEndBox.getLowerLeftX(), i, m_backEndBox.getLowerLeftX() + m_mediumLine, i);
      drawLineW(box, utils);
    }
    else if((i % (int)m_smallBlockSize) == 0)
    {
      box = te::gm::Envelope(m_backEndBox.getLowerLeftX(), i, m_backEndBox.getLowerLeftX() + m_smallLine, i);
      drawLineW(box, utils);
    }
  }
}

void te::layout::VerticalRulerModel::setBox( te::gm::Envelope box )
{
  if(box.getWidth() < 3. || box.getHeight() < 10.)
    return;

  m_box = box;
  m_backEndBox = te::gm::Envelope(m_box.getLowerLeftX() + m_backEndMargin, m_box.getLowerLeftY(),
    m_box.getUpperRightX() - m_backEndMargin , m_box.getUpperRightY() - m_backEndSpacing);
}
