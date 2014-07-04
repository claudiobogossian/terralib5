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
  \file PageSetupOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PageSetupOutside.h"
#include "ui_PageSetup.h"
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../core/pattern/mvc/OutsideController.h"

// STL
#include <string>
#include <sstream> 

// Qt
#include <QColorDialog> 
#include <QFrame>
#include <QLineEdit>
#include <QValidator>
#include <QComboBox>
#include <QCheckBox> 
#include <QMessageBox>
#include <QObjectList>

te::layout::PageSetupOutside::PageSetupOutside( OutsideController* controller, Observable* o ) :
  QDialog(0),
  OutsideObserver(controller, o),
  m_ui(new Ui::PageSetup)
{
  m_ui->setupUi(this);

  setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint 
    | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );

  init();
}

te::layout::PageSetupOutside::~PageSetupOutside()
{

}

void te::layout::PageSetupOutside::init()
{
  m_ui->lneCustomWidth->setValidator(new  QDoubleValidator(this));
  m_ui->lneCustomHeight->setValidator(new  QDoubleValidator(this));
}

void te::layout::PageSetupOutside::updateObserver( ContextItem context )
{
  setVisible(context.getShow());
  if(context.getShow() == true)
    show();
  else
    hide();
}

void te::layout::PageSetupOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::PageSetupOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::PageSetupOutside::load()
{

}
