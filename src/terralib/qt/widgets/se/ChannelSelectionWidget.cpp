/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/ChannelSelectionWidget.cpp

  \brief A dialog used to build a ChannelSelection element.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "ChannelSelectionWidget.h"
#include "SelectedChannelWidget.h"
#include "ui_ChannelSelectionWidgetForm.h"

// Qt


// STL


te::qt::widgets::ChannelSelectionWidget::ChannelSelectionWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ChannelSelectionWidgetForm)
{
  m_ui->setupUi(this);

  //Selected Channel Widget
  m_redSCWidget = new te::qt::widgets::SelectedChannelWidget(this);
  m_greenSCWidget = new te::qt::widgets::SelectedChannelWidget(this);
  m_blueSCWidget = new te::qt::widgets::SelectedChannelWidget(this);
  m_monoSCWidget = new te::qt::widgets::SelectedChannelWidget(this);

  // Adjusting...
  QGridLayout* layoutRed = new QGridLayout(m_ui->m_rFrame);
  layoutRed->addWidget(m_redSCWidget);

  QGridLayout* layoutGreen = new QGridLayout(m_ui->m_gFrame);
  layoutGreen->addWidget(m_greenSCWidget);

  QGridLayout* layoutBlue = new QGridLayout(m_ui->m_bFrame);
  layoutBlue->addWidget(m_blueSCWidget);

  QGridLayout* layoutMono = new QGridLayout(m_ui->m_mFrame);
  layoutMono->addWidget(m_monoSCWidget);
}

te::qt::widgets::ChannelSelectionWidget::~ChannelSelectionWidget()
{

}

void te::qt::widgets::ChannelSelectionWidget::updateUi()
{
  
}
