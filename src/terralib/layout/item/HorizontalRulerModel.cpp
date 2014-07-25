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
#include "../core/ContextItem.h"
#include "../core/pattern/mvc/ItemModelObservable.h"
#include "../core/enum/EnumMode.h"
#include "../core/Utils.h"
#include "../../maptools/Canvas.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Point.h"
#include "../../color/RGBAColor.h"

// STL
#include <sstream>
#include <string>    

te::layout::HorizontalRulerModel::HorizontalRulerModel(PaperConfig* paperConfig) :
  AbstractRulerModel(paperConfig)
{

}

te::layout::HorizontalRulerModel::~HorizontalRulerModel()
{

}

void te::layout::HorizontalRulerModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = context.getCanvas();
  Utils* utils = context.getUtils();

  if((!canvas) || (!utils))
    return;

  /*te::gm::Envelope newBox = m_box;
  double zoomFactor = context.getZoomFactor();

  double w = newBox.getWidth() * (1 / zoomFactor);
  double h = newBox.getHeight() * (1 / zoomFactor);

  if(zoomFactor > 1.)
  {
  w = newBox.getWidth() * zoomFactor;
  h = newBox.getHeight() * zoomFactor;
  }

  newBox.m_urx = newBox.m_llx + w;
  newBox.m_ury = newBox.m_lly + h;

  m_box = newBox;*/

  if(context.isResizeCanvas())
    utils->configCanvas(m_box);  
  
  drawRuler(canvas, utils, context.getZoomFactor());

  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);
  
  context.setPixmap(pixmap);
  notifyAll(context);
}

void te::layout::HorizontalRulerModel::drawRuler( te::map::Canvas* canvas, Utils* utils, double zoomFactor )
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
                
  if(m_paperConfig)
  {
    te::gm::Envelope* paperBox = m_paperConfig->getPaperBoxW();

    envPaper = te::gm::Envelope(paperBox->getLowerLeftX(), m_backEndBox.getLowerLeftY(),
      paperBox->getUpperRightX(), m_backEndBox.getUpperRightY());
    
    te::color::RGBAColor colorp5(255,255,255, TE_OPAQUE);
    drawRectW(envPaper, colorp5, canvas, utils);
  }

  te::color::RGBAColor colorp6(0,0,0, TE_OPAQUE);
  canvas->setLineColor(colorp6);

  drawHorizontalRuler(canvas, utils);

  envMargin = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_backEndBox.getUpperRightY() - m_lineMargin, 
    m_backEndBox.getWidth(), m_backEndBox.getUpperRightY() - m_lineMargin);

  drawLineW(envMargin, utils);
}

void te::layout::HorizontalRulerModel::drawHorizontalRuler(te::map::Canvas* canvas, Utils* utils)
{
  if(!m_box.isValid())
    return;

  double llx = m_backEndBox.getLowerLeftX();
  double urx = m_backEndBox.getUpperRightX();

  te::gm::Envelope box;
  
  for(int i = (int)llx ; i < (int) urx ; ++i)
  {
    if((i % (int)m_blockSize) == 0)
    {
      box = te::gm::Envelope(i, m_backEndBox.getUpperRightY(), i, m_backEndBox.getUpperRightY() - m_longLine);  
      drawLineW(box, utils);

      std::stringstream ss;//create a stringstream
      ss << i;//add number to the stream

      canvas->drawText((double)i, m_backEndBox.getUpperRightY() - (m_longLine + 1.), ss.str(), 0);
    }
    else if((i % (int)m_middleBlockSize) == 0)
    {
      box = te::gm::Envelope(i, m_backEndBox.getUpperRightY(), i, m_backEndBox.getUpperRightY() - m_mediumLine);  
      drawLineW(box, utils);
    }
    else if((i % (int)m_smallBlockSize) == 0)
    {
      box = te::gm::Envelope(i, m_backEndBox.getUpperRightY(), i, m_backEndBox.getUpperRightY() - m_smallLine);  
      drawLineW(box, utils);
    }
  }
}

void te::layout::HorizontalRulerModel::setBox( te::gm::Envelope box )
{
  if(box.getWidth() < 15. || box.getHeight() < 3.)
    return;

  m_box = box;
  m_backEndBox = te::gm::Envelope(m_box.getLowerLeftX() + m_backEndSpacing, m_box.getLowerLeftY() + m_backEndMargin,
    m_box.getUpperRightX(), m_box.getUpperRightY() - m_backEndMargin);
}
