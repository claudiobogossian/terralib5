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
  \file PaperModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PaperModel.h"
#include "../core/ContextItem.h"
#include "PaperConfig.h"
#include "../../maptools/Canvas.h"

te::layout::PaperModel::PaperModel() :
  m_paperConfig(0)
{
  m_shadowPadding = 10.;

  m_backgroundColor = te::color::RGBAColor(255, 255, 255, 255);
  m_shadowColor = te::color::RGBAColor(0, 0, 0, 255);


  config();
}

te::layout::PaperModel::PaperModel( PaperConfig* paperConfig ) :
  m_paperConfig(paperConfig)
{
  m_shadowPadding = 10.;

  m_backgroundColor = te::color::RGBAColor(255, 255, 255, 255);
  m_shadowColor = te::color::RGBAColor(0, 0, 0, 255);

  config();
}

te::layout::PaperModel::~PaperModel()
{

}

void te::layout::PaperModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  te::map::Canvas* canvas = context.getCanvas();
  Utils* utils = context.getUtils();

  if((!canvas) || (!utils))
    return;

 // m_box = te::gm::Envelope(0., 0., m_boxShadow.getUpperRightX(), m_boxPaper.getUpperRightY());

  //m_box = te::gm::Envelope(0., - m_shadowPadding, m_boxShadow.getUpperRightX(), m_boxPaper.getUpperRightY());

  if(context.isResizeCanvas())
    utils->configCanvas(m_box);
  
  double pw = 0.;
  double ph = 0.;

  double llx = m_box.getLowerLeftX();
  double lly = m_box.getLowerLeftY();
  double urx = m_box.getUpperRightX();
  double ury = m_box.getUpperRightY();

  canvas->setPolygonContourColor(m_shadowColor);
  canvas->setPolygonFillColor(m_shadowColor);

  m_boxShadow = te::gm::Envelope(llx + m_shadowPadding, lly, urx, ury - m_shadowPadding);
  utils->drawRectW(m_boxShadow);

  canvas->setPolygonContourColor(m_borderColor);
  canvas->setPolygonFillColor(m_backgroundColor);

  m_boxPaper = te::gm::Envelope(llx, lly + m_shadowPadding, urx - m_shadowPadding, ury);
  utils->drawRectW(m_boxPaper);
    
  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);
  
  context.setPixmap(pixmap);
  notifyAll(context);
}

void te::layout::PaperModel::config()
{
  if(!m_paperConfig)
    return;

  double pw = 0.;
  double ph = 0.;

  m_paperConfig->getPaperSize(pw, ph);

  double width = m_shadowPadding + pw;
  double height = m_shadowPadding + ph;

  m_boxPaper = te::gm::Envelope(0, m_shadowPadding, pw, height);
  m_boxShadow = te::gm::Envelope(m_shadowPadding, 0, width, ph);
  m_box = te::gm::Envelope(0., 0., width, height);
}

te::color::RGBAColor te::layout::PaperModel::getShadowColor()
{
  return m_shadowColor;
}

void te::layout::PaperModel::setShadowColor( te::color::RGBAColor color )
{
  m_shadowColor = color;
}

void te::layout::PaperModel::setPaperConfig( PaperConfig* pConfig )
{
  m_paperConfig = pConfig;
  config();
}

te::layout::PaperConfig* te::layout::PaperModel::getPaperConfig() const
{
  return m_paperConfig;
}

void te::layout::PaperModel::setShadowPadding( double padding )
{
  m_shadowPadding = padding;
}

double te::layout::PaperModel::getShadowPadding()
{
  return m_shadowPadding;
}
