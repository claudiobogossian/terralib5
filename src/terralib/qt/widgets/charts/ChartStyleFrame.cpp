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
  \file terralib/qt/af/settings/ChartFrame.cpp

  \brief A frame used to adjust a Chart's visual style.
*/

#include "ChartDisplay.h"
#include "ChartStyleFrame.h"
#include "ChartStyleWidget.h"
#include "ui_ChartStyleFrameWidgetForm.h"

te::qt::widgets::ChartStyleFrame::ChartStyleFrame(QWidget* parent)
  : ChartWidget(parent),
    m_ui(new Ui::ChartStyleFrameWidgetForm)
{
  m_ui->setupUi(this);

  m_styleWidget = new te::qt::widgets::ChartStyleWidget();
  m_label = "Chart";

  QGridLayout* layout = new QGridLayout(m_ui->m_StyleFrame);
  layout->addWidget(m_styleWidget);
}

te::qt::widgets::ChartStyleFrame::~ChartStyleFrame()
{

}

te::qt::widgets::ChartDisplay* te::qt::widgets::ChartStyleFrame::getDisplay()
{
  m_display->setStyle(m_styleWidget->getStyle());
  return m_display;
}

void te::qt::widgets::ChartStyleFrame::setDisplay(te::qt::widgets::ChartDisplay* newDisplay)
{
  m_display = newDisplay;
  m_styleWidget->setStyle(m_display->getStyle());
}