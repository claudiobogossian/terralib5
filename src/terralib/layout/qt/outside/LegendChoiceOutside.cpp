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
  \file terralib/layout/qt/outside/MapLayerChoiceOutside.cpp

  \brief A dialog configure the input layer to address geocoding operation
*/

// TerraLib
#include "LegendChoiceOutside.h"
#include "ui_LegendChoice.h"

// STL
#include <string>

// Qt
#include <QGridLayout>

te::layout::LegendChoiceOutside::LegendChoiceOutside(OutsideController* controller, Observable* o)
  : QDialog(0),
    OutsideObserver(controller, o),
    m_ui(new Ui::LegendChoice)
{
  // add controls
  m_ui->setupUi(this);
  
  m_widget.reset(new te::qt::widgets::DoubleListWidget(m_ui->m_widget));
  
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_widget);
  displayLayout->addWidget(m_widget.get());

  m_widget->setLeftLabel("Available Attributes");
  m_widget->setRightLabel("Selected Attributes");

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

}

te::layout::LegendChoiceOutside::~LegendChoiceOutside()
{
}

void te::layout::LegendChoiceOutside::onOkPushButtonClicked()
{
  this->accept();
}

void te::layout::LegendChoiceOutside::onCancelPushButtonClicked()
{
  reject();
}

void te::layout::LegendChoiceOutside::updateObserver( ContextItem context )
{
  setVisible(context.isShow());
  if(context.isShow() == true)
    show();
  else
    hide();
}

void te::layout::LegendChoiceOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::LegendChoiceOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

