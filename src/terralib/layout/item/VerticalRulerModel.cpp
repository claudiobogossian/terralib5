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
#include "../core/ContextItem.h"
#include "../core/pattern/mvc/ItemModelObservable.h"
#include "../core/enum/EnumMode.h"
#include "../core/Utils.h"
#include "../../maptools/Canvas.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Point.h"

// STL
#include <sstream>
#include <string>    

te::layout::VerticalRulerModel::VerticalRulerModel(PaperConfig* paperConfig):
  AbstractRulerModel(paperConfig)
{

}

te::layout::VerticalRulerModel::~VerticalRulerModel()
{

}

void te::layout::VerticalRulerModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = context.getCanvas();
  Utils* utils = context.getUtils();
 
  if((!canvas) || (!utils))
    return;
      
  if(context.isResizeCanvas())
    utils->configCanvas(m_box, true, false);  

  canvas->setTextPointSize(5);
  canvas->setFontFamily("Verdana");
    
  drawRuler(canvas, utils, context.getZoomFactor());

  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);
  
  context.setPixmap(pixmap);
  notifyAll(context);
}

void te::layout::VerticalRulerModel::drawRuler( te::map::Canvas* canvas, Utils* utils, double zoomFactor )
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
   
    if(paperBox)
    {
      double ury = paperBox->getUpperRightY();
      if(zoomFactor >= 1.)
      {
        ury = m_backEndBox.getUpperRightY();
      }

      envPaper = te::gm::Envelope(m_backEndBox.getLowerLeftX(), paperBox->getLowerLeftY() * zoomFactor,
        m_backEndBox.getUpperRightX(), ury * zoomFactor);
      
      te::color::RGBAColor colorp2(255,255,255, TE_OPAQUE);
      drawRectW(envPaper, colorp2, canvas, utils);
    }
  }

  te::color::RGBAColor colorp3(0,0,0, TE_OPAQUE);
  canvas->setLineColor(colorp3);

  drawVerticalRuler(canvas, utils, zoomFactor);
  
  /* Line on the marks */
  envMargin = te::gm::Envelope(m_backEndBox.getUpperRightX() - m_lineMargin, m_backEndBox.getLowerLeftY(), 
    m_backEndBox.getUpperRightX() - m_lineMargin, m_backEndBox.getUpperRightY());

  drawLineW(envMargin, utils);

  /* Outside line */
  envMargin = te::gm::Envelope(m_box.getUpperRightX() - 0.2, m_box.getLowerLeftY(), 
    m_box.getUpperRightX() - 0.2, m_backEndBox.getUpperRightY() );
  
  drawLineW(envMargin, utils);
}

void te::layout::VerticalRulerModel::drawVerticalRuler(te::map::Canvas* canvas, Utils* utils, double zoomFactor)
{
  if(!m_box.isValid())
    return;

  double wtxt = 0;
  double htxt = 0;

  double lly = m_backEndBox.getLowerLeftY();
  double ury = m_backEndBox.getUpperRightY();

  te::gm::Envelope box;
      
  for(int i = (int)lly ; i < (int) ury ; ++i)
  {
    if((i % (int)m_blockSize) == 0)
    {
      box = te::gm::Envelope(m_backEndBox.getUpperRightX(), i, m_backEndBox.getUpperRightX() - m_longLine, i);
      
      std::stringstream ss;//create a stringstream
      ss << i;//add number to the stream

      utils->textBoundingBox(wtxt, htxt, ss.str());
      canvas->drawText(m_backEndBox.getUpperRightX() - (m_longLine), (double)(i * zoomFactor) - (wtxt/2.), ss.str(), -90);
    }
    else if((i % (int)m_middleBlockSize) == 0)
    {
      box = te::gm::Envelope(m_backEndBox.getUpperRightX(), i, m_backEndBox.getUpperRightX() - m_mediumLine, i);
    }
    else if((i % (int)m_smallBlockSize) == 0)
    {
      box = te::gm::Envelope(m_backEndBox.getUpperRightX(), i, m_backEndBox.getUpperRightX() - m_smallLine, i);
    }
    
    /* Spacing between marks depending on the zoom level */
    box.m_lly = box.m_lly * zoomFactor;
    box.m_ury = box.m_ury * zoomFactor;

    drawLineW(box, utils);
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
