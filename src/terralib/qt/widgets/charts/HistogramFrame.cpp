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
  \file terralib/qt/af/settings/HistogramFrame.cpp

  \brief A frame used to adjust a histogram's chart parameters, weather it is about it's data or it's visual style.
*/

#include "../../../dataaccess.h"
#include "ui_HistogramFrameWidgetForm.h"
#include "HistogramFrame.h"
#include "HistogramDataWidget.h"
#include "HistogramStyleWidget.h"

te::qt::widgets::HistogramFrame::HistogramFrame(QWidget* parent)
  : ChartWidget(parent),
    m_ui(new Ui::HistogramFrameWidgetForm)
{
  m_ui->setupUi(this);
  m_label = "Histogram";


  te::qt::widgets::HistogramStyleWidget* styleWidget = new te::qt::widgets::HistogramStyleWidget();

  QGridLayout* styleLayout = new QGridLayout(m_ui->m_histogramStyleFrame);
  styleLayout->addWidget(styleWidget);
}

te::qt::widgets::HistogramFrame::~HistogramFrame()
{

}

void te::qt::widgets::HistogramFrame::setDataSet(te::da::DataSet* dataSet)
{
  te::qt::widgets::HistogramDataWidget* dataWidget = new te::qt::widgets::HistogramDataWidget(dataSet);
  QGridLayout* dataLayout = new QGridLayout(m_ui->m_histogramDataFrame);
  dataLayout->addWidget(dataWidget);
}