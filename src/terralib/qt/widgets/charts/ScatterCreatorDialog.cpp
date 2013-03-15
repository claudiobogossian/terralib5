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
  \file terralib/qt/widgets/charts/ScatterCreatorDialog.cpp

  \brief AA widget used to create a nem Scatter chart.
*/

#include "ui_scatterCreatorDialog.h"
#include "ScatterCreatorDialog.h"
#include "ChartStyleDialog.h"

te::qt::widgets::ScatterCreatorDialog::ScatterCreatorDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ScatterDialog)
{
    m_ui->setupUi(this);

// connect signal and slots
  connect(m_ui->m_stylePushButton, SIGNAL(clicked()), this, SLOT(onStylePushButtonClicked()));
  connect(m_ui->m_plotStylePushButton, SIGNAL(clicked()), this, SLOT(onPlotStylePushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
}

te::qt::widgets::ScatterCreatorDialog::~ScatterCreatorDialog(){}

void te::qt::widgets::ScatterCreatorDialog::onStylePushButtonClicked()
{
    te::qt::widgets::ChartStyleDialog dlg(this);
    dlg.exec();
}
void te::qt::widgets::ScatterCreatorDialog::onPlotStylePushButtonClicked(){}
void te::qt::widgets::ScatterCreatorDialog::onOkPushButtonClicked(){}
void te::qt::widgets::ScatterCreatorDialog::onCancelPushButtonClicked(){}
void te::qt::widgets::ScatterCreatorDialog::onHelpPushButtonClicked(){}