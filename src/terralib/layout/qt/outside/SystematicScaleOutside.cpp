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
  \file PageSetupOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "SystematicScaleOutside.h"
#include "ui_SystematicScale.h"

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

te::layout::SystematicScaleOutside::SystematicScaleOutside(AbstractOutsideController* controller) :
  QDialog(0),
	AbstractOutsideView(controller),
  m_ui(new Ui::SystematicScale)
{
  m_ui->setupUi(this);

  setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint 
    | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );

}

te::layout::SystematicScaleOutside::~SystematicScaleOutside()
{

}

void te::layout::SystematicScaleOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::SystematicScaleOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::SystematicScaleOutside::on_btnOK_clicked()
{
  double scale = 0.;

  scale = m_ui->cmbScale->currentText().toDouble();
  SystematicScaleType type = getType(scale);
  emit systematicApply(scale, type);

  accept();
}

void te::layout::SystematicScaleOutside::on_btnCancel_clicked()
{
  reject();
}

te::layout::SystematicScaleType te::layout::SystematicScaleOutside::getType( double scale )
{
  te::layout::SystematicScaleType type = S0;  

  if(scale == 250.000)
  {
    type = S250000;
  }
  else if(scale == 100.000)
  {
    type = S100000;
  }
  else if(scale == 50.000)
  {
    type = S50000;
  }  
  else if(scale == 25.000)
  {
    type = S25000;
  }

  return type;
}
