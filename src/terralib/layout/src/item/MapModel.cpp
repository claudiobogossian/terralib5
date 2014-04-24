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
  \file MapModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapModel.h"
#include "ContextItem.h"
#include "Context.h"
#include "../../../maptools/Canvas.h"

te::layout::MapModel::MapModel()
{
  m_backgroundColor = te::color::RGBAColor(0, 0, 255, 255);
  m_box = te::gm::Envelope(0., 0., 100., 70.);
}

te::layout::MapModel::~MapModel()
{

}

void te::layout::MapModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  ContextItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

void te::layout::MapModel::setPosition( const double& x, const double& y )
{
  double x1 = x; 
  double y1 = y - m_box.getHeight();

  double x2 = x + m_box.getWidth();
  double y2 = y;

  m_box = te::gm::Envelope(x1, y1, x2, y2);
}

