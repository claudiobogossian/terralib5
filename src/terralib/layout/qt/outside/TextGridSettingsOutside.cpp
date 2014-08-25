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
  \file TextGridSettingsOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TextGridSettingsOutside.h"
#include "ui_TextGridSettings.h"
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../core/pattern/mvc/OutsideController.h"
#include "../../core/pattern/singleton/Context.h"

// STL
#include <string>
#include <sstream> 

te::layout::TextGridSettingsOutside::TextGridSettingsOutside( OutsideController* controller, Observable* o ) :
  QWidget(0),
  OutsideObserver(controller, o),
  m_ui(new Ui::TextGridSettings)
{
  m_ui->setupUi(this);
  
  init();
}

te::layout::TextGridSettingsOutside::~TextGridSettingsOutside()
{

}

void te::layout::TextGridSettingsOutside::init()
{

}

void te::layout::TextGridSettingsOutside::updateObserver( ContextItem context )
{
  setVisible(context.getShow());
  if(context.getShow() == true)
    show();
  else
    hide();
}

void te::layout::TextGridSettingsOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::TextGridSettingsOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::TextGridSettingsOutside::load()
{

}

void te::layout::TextGridSettingsOutside::on_pbApply_clicked()
{
  
}
