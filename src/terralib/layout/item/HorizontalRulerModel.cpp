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
#include "../core/pattern/singleton/Context.h"
#include "../core/enum/Enums.h"

// STL
#include <sstream>
#include <string>    

te::layout::HorizontalRulerModel::HorizontalRulerModel(PaperConfig* paperConfig) :
  AbstractRulerModel(paperConfig)
{
  m_type = Enums::getInstance().getEnumObjectType()->getHorizontalRuler();
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

  if(context.isResizeCanvas())
    utils->configCanvas(m_box, true, false);  

  m_backEndBox = te::gm::Envelope(m_box.getLowerLeftX() + m_backEndSpacing, m_box.getLowerLeftY() + m_backEndMargin,
    m_box.getUpperRightX(), m_box.getUpperRightY() - m_backEndMargin);

  canvas->setLineWidth(1.);
  canvas->setTextPointSize(6);
  canvas->setFontFamily("Verdana");

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
    double w = 0;
    double h = 0;

    m_paperConfig->getPaperSize(w, h);
    te::gm::Envelope paperBox(0, 0, w, h);
            
    /*envPaper = te::gm::Envelope(paperBox.getLowerLeftX(), m_backEndBox.getLowerLeftY(),
      paperBox.getLowerLeftX() + (w * zoomFactor), m_backEndBox.getUpperRightY());*/

    double x1 = 0;
    double zoom = 1. / zoomFactor;
    x1 = m_backEndBox.m_llx + paperBox.getWidth();
    
    double x2 = m_backEndBox.m_urx - paperBox.getWidth();
    x2 = x2 * zoomFactor;

    x1 = x1 * zoomFactor;

    if(zoomFactor >= 1.)
      x1 = 0;

    envPaper = te::gm::Envelope(x1, m_backEndBox.getLowerLeftY(),
      x1 + (w * zoomFactor), m_backEndBox.getUpperRightY());

    /*te::gm::Envelope newBox1(m_backEndBox.m_llx, 0, m_backEndBox.m_llx + w, h);
    WorldTransformer transf = utils->getTransformGeo(m_backEndBox, newBox1);
    transf.setMirroring(false);*/
    
    te::color::RGBAColor colorp5(255,255,255, TE_OPAQUE);
    drawRectW(envPaper, colorp5, canvas, utils);
  }

  te::color::RGBAColor colorp6(0,0,0, TE_OPAQUE);
  canvas->setLineColor(colorp6);

  drawHorizontalRuler(canvas, utils, zoomFactor);

  /* Line on the marks */
  envMargin = te::gm::Envelope(m_backEndBox.getLowerLeftX(), m_backEndBox.getLowerLeftY() + m_lineMargin, 
    m_backEndBox.getWidth(), m_backEndBox.getLowerLeftY() + m_lineMargin);
  
  drawLineW(envMargin, utils);

  /* Outside line */
  envMargin = te::gm::Envelope(m_box.getLowerLeftX(), m_box.getLowerLeftY() + 0.2, 
    m_box.getWidth(), m_box.getLowerLeftY() + 0.2);
  
  drawLineW(envMargin, utils);
}

void te::layout::HorizontalRulerModel::drawHorizontalRuler(te::map::Canvas* canvas, Utils* utils, double zoomFactor)
{
  if(!m_box.isValid())
    return;

  double wtxt = 0;
  double htxt = 0;

  double llx = m_backEndBox.getLowerLeftX();
  double urx = m_backEndBox.getUpperRightX();
  
  double w = 0;
  double x = 0;

  te::gm::Envelope box;
   
  double l = llx;
  for(int i = (int)llx ; i < (int) urx ; ++i)
  {
    if((i % (int)(m_blockSize)) == 0)
    {
      box = te::gm::Envelope(l, m_backEndBox.getLowerLeftY(), l, m_backEndBox.getLowerLeftY() + m_longLine);  

      std::stringstream ss;//create a stringstream
      ss << i;//add number to the stream

      utils->textBoundingBox(wtxt, htxt, ss.str());
      canvas->drawText((double)l - (wtxt/2.), m_backEndBox.getLowerLeftY() + (m_longLine), ss.str(), 0);
    }
    else if((i % (int)(m_middleBlockSize)) == 0)
    {
      box = te::gm::Envelope(l, m_backEndBox.getLowerLeftY(), l, m_backEndBox.getLowerLeftY() + m_mediumLine); 
    }
    else if((i % (int)(m_smallBlockSize)) == 0)
    {
      box = te::gm::Envelope(l, m_backEndBox.getLowerLeftY(), l, m_backEndBox.getLowerLeftY() + m_smallLine);  
    }

    drawLineW(box, utils);
    l += zoomFactor;
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
