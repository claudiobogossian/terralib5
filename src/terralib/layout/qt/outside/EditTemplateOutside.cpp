/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file EditTemplateOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EditTemplateOutside.h"
#include "ui_EditTemplate.h"
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../core/pattern/mvc/OutsideController.h"
#include "../../core/pattern/singleton/Context.h"

// STL
#include <string>
#include <sstream> 

te::layout::EditTemplateOutside::EditTemplateOutside( OutsideController* controller, Observable* o ) :
  QWidget(0),
  OutsideObserver(controller, o),
  m_ui(new Ui::EditTemplate)
{
  m_ui->setupUi(this);
  
  init();
}

te::layout::EditTemplateOutside::~EditTemplateOutside()
{

}

void te::layout::EditTemplateOutside::init()
{

}

void te::layout::EditTemplateOutside::updateObserver( ContextItem context )
{
  setVisible(context.isShow());
  if(context.isShow() == true)
    show();
  else
    hide();
}

void te::layout::EditTemplateOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::EditTemplateOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::EditTemplateOutside::load()
{

}

void te::layout::EditTemplateOutside::on_pbApply_clicked()
{
  
}
