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
  \file FontDialogOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "FontDialogOutside.h"

// Qt

te::layout::FontDialogOutside::FontDialogOutside( OutsideController* controller, Observable* o ) :
	QFontDialog(0),
	OutsideObserver(controller, o)
{
  
}

te::layout::FontDialogOutside::~FontDialogOutside()
{

}

void te::layout::FontDialogOutside::updateObserver( ContextItem context )
{
  
}

void te::layout::FontDialogOutside::createLayout()
{
  //Layout
}

void te::layout::FontDialogOutside::setPosition( const double& x, const double& y )
{
  
}

te::gm::Coord2D te::layout::FontDialogOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

