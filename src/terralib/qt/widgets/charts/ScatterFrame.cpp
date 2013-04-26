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
  \file terralib/qt/af/settings/ScatterFrame.cpp

  \brief A frame used to adjust a scater's chart parameters, weather it is about it's data or it's visual style.
*/

#include "ui_ScatterFrameWidgetForm.h"
#include "ScatterFrame.h"
#include "ScatterDataWidget.h"
#include "ScatterStyleWidget.h"

te::qt::widgets::ScatterFrame::ScatterFrame(QWidget* parent)
  : ChartWidget(parent),
    m_ui(new Ui::ScatterFrameWidgetForm)
{
  m_ui->setupUi(this);
  m_label = "Scatter";

//  te::qt::widgets::ScatterDataWidget* dataWidget = new te::qt::widgets::ScatterDataWidget();
  te::qt::widgets::ScatterStyleWidget* styleWidget = new te::qt::widgets::ScatterStyleWidget();

  QGridLayout* dataLayout = new QGridLayout(m_ui->m_scatterDataFrame);
  //dataLayout->addWidget(dataWidget);

  QGridLayout* styleLayout = new QGridLayout(m_ui->m_scatterStyleFrame);
  styleLayout->addWidget(styleWidget);

}

te::qt::widgets::ScatterFrame::~ScatterFrame()
{

}

te::da::DataSet* te::qt::widgets::ScatterFrame::getDataSet()
{
  return m_dataSet;
}

void te::qt::widgets::ScatterFrame::setDataSet(te::da::DataSet* newDataSet)
{
 m_dataSet = newDataSet;
}