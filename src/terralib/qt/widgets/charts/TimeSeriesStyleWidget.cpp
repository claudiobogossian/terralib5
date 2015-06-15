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
  \file terralib/qt/widgets/charts/timeSeriesStyleWidget.cpp

  \brief A widget used to adjust a timeSeries's style.
*/

//Terralib
#include "../../../se/Stroke.h"
#include "../se/BasicStrokeDialog.h"
#include "TimeSeriesStyle.h"
#include "TimeSeriesStyleWidget.h"
#include "ui_TimeSeriesStyleWidgetForm.h"

// STL
#include <memory>

//QT
#include <QDialog>

te::qt::widgets::TimeSeriesStyleWidget::TimeSeriesStyleWidget(te::qt::widgets::TimeSeriesStyle* initial, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::TimeSeriesStyleWidgetForm),
    m_timeSeriesStyle(initial)
{
    m_ui->setupUi(this);

  if(!m_timeSeriesStyle)
    m_timeSeriesStyle = new te::qt::widgets::TimeSeriesStyle(); 

// connect signal and slots
  connect(m_ui->m_strokePushButton, SIGNAL(clicked()), this, SLOT(onStrokePushButtonClicked()));
}

te::qt::widgets::TimeSeriesStyleWidget::~TimeSeriesStyleWidget()
{
  delete m_timeSeriesStyle;
}

te::qt::widgets::TimeSeriesStyle* te::qt::widgets::TimeSeriesStyleWidget::getTimeSeriesStyle()
{
  return m_timeSeriesStyle->clone();
}

void te::qt::widgets::TimeSeriesStyleWidget::onStrokePushButtonClicked()
{
  std::auto_ptr<te::se::Stroke> stroke (te::qt::widgets::BasicStrokeDialog::getStroke(m_timeSeriesStyle->getStroke(), 0, "Stroke"));
  if(stroke.get())
    m_timeSeriesStyle->setStroke(stroke->clone());
}
