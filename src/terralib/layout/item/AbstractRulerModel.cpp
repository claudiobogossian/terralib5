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
  \file AbstractRulerModel.cpp
   
  \brief 

  \ingroup layout
*/

#include "AbstractRulerModel.h"

te::layout::AbstractRulerModel::AbstractRulerModel(PaperConfig* paperConfig) :
  m_visible(true),
  m_spacing(1.),
  m_blockSize(10),
  m_middleBlockSize(5),
  m_smallBlockSize(1),
  m_longLine(3.5),
  m_mediumLine(2.5),
  m_smallLine(1.5),
  m_borderLineSize(0.3),
  m_lineMargin(1.),
  m_backEndSpacing(10),
  m_backEndMargin(2.),
  m_paperConfig(paperConfig)
{
  m_box = te::gm::Envelope(0., 0., 150., 150.);
  m_backEndBox = te::gm::Envelope(0., 0., 150., 150.);
}

te::layout::AbstractRulerModel::~AbstractRulerModel( void )
{

}

void te::layout::AbstractRulerModel::drawLineW( te::gm::Envelope box, Utils* utils )
{
  te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
  line = utils->createSimpleLine(box);
  utils->drawLineW(line);
  if(line) delete line;
}

void te::layout::AbstractRulerModel::drawRectW( te::gm::Envelope box, te::color::RGBAColor color, te::map::Canvas* canvas, Utils* utils )
{
  canvas->setPolygonFillColor(color);
  canvas->setPolygonContourColor(color);
  utils->drawRectW(box);
}

void te::layout::AbstractRulerModel::setVisible( bool visible )
{
  m_visible = visible;
}

bool te::layout::AbstractRulerModel::isVisible()
{
  return m_visible;
}

void te::layout::AbstractRulerModel::setPaperConfig( PaperConfig* pConfig )
{
  m_paperConfig = pConfig;
}

te::layout::PaperConfig* te::layout::AbstractRulerModel::getPaperConfig() const
{
  return m_paperConfig;
}
