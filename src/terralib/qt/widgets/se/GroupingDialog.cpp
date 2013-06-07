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
  \file terralib/qt/widgets/se/GroupingDialog.cpp

  \brief A dialog used to build a grouping.
*/

// TerraLib
#include "GroupingDialog.h"
#include "GroupingWidget.h"
#include "ui_GroupingDialogForm.h"

te::qt::widgets::GroupingDialog::GroupingDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::GroupingDialogForm)
{
  m_ui->setupUi(this);

  // Fill Widget
  m_groupingWidget = new te::qt::widgets::GroupingWidget(m_ui->m_widget);

  // Signals & slots
  connect(m_ui->m_applyPushButton, SIGNAL(clicked()), m_groupingWidget, SLOT(onApplyPushButtonClicked()));

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  layout->setContentsMargins(0,0,0,0);
  layout->addWidget(m_groupingWidget);
}

te::qt::widgets::GroupingDialog::~GroupingDialog()
{
}

void te::qt::widgets::GroupingDialog::setLayer(te::map::AbstractLayerPtr layer)
{
  m_groupingWidget->setLayer(layer);
}
