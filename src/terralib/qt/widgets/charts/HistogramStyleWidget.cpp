/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/charts/histogramStyleWidget.cpp

  \brief A widget used to adjust a histogram's style.
*/


//Terralib
#include "../../../se/Fill.h"
#include "../../../se/Stroke.h"
#include "../se/BasicFillDialog.h"
#include "../se/BasicStrokeDialog.h"
#include "HistogramStyleWidget.h"
#include "HistogramStyle.h"
#include "ui_HistogramStyleWidgetForm.h"

//QT
#include <QWidget>

#include <memory>

te::qt::widgets::HistogramStyleWidget::HistogramStyleWidget(te::qt::widgets::HistogramStyle* initial, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::HistogramStyleWidgetForm),
    m_histogramStyle(initial)
{
  m_ui->setupUi(this);

  if(!m_histogramStyle)
    m_histogramStyle = new te::qt::widgets::HistogramStyle(); 

// connect signal and slots
  connect(m_ui->m_fillPushButton, SIGNAL(clicked()), this, SLOT(onFillPushButtonClicked()));
  connect(m_ui->m_strokePushButton, SIGNAL(clicked()), this, SLOT(onStrokePushButtonClicked()));
}

te::qt::widgets::HistogramStyleWidget::~HistogramStyleWidget()
{
  delete m_histogramStyle;
}

te::qt::widgets::HistogramStyle* te::qt::widgets::HistogramStyleWidget::getHistogramStyle()
{
  return m_histogramStyle->clone();
}

void te::qt::widgets::HistogramStyleWidget::onFillPushButtonClicked()
{
  std::auto_ptr<te::se::Fill> fill (te::qt::widgets::BasicFillDialog::getFill(m_histogramStyle->getFill(), 0, "Bar Fill"));
  if (fill.get())
    m_histogramStyle->setFill(fill->clone());
}

void te::qt::widgets::HistogramStyleWidget::onStrokePushButtonClicked()
{
  std::auto_ptr<te::se::Stroke> stroke (te::qt::widgets::BasicStrokeDialog::getStroke(m_histogramStyle->getStroke(), 0, "Bar Stroke"));
  if(stroke.get())
    m_histogramStyle->setStroke(stroke->clone());
}
