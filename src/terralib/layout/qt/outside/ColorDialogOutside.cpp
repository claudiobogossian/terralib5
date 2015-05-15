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
  \file ColorDialogOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ColorDialogOutside.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../item/MapItem.h"
#include "../../core/pattern/mvc/ItemModelObservable.h"
#include "../../item/MapModel.h"
#include "../core/ItemUtils.h"

// Qt


te::layout::ColorDialogOutside::ColorDialogOutside( OutsideController* controller, Observable* o ) :
  QColorDialog(0),
	OutsideObserver(controller, o)
{
	
}

te::layout::ColorDialogOutside::~ColorDialogOutside()
{
  
}

void te::layout::ColorDialogOutside::createLayout()
{
  //Layout
    
}

void te::layout::ColorDialogOutside::updateObserver( ContextItem context )
{
	
}

void te::layout::ColorDialogOutside::setPosition( const double& x, const double& y )
{
  
}

te::gm::Coord2D te::layout::ColorDialogOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}
 
void te::layout::ColorDialogOutside::refreshOutside()
{
  
}
