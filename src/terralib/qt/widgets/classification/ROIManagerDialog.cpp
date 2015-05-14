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
  \file terralib/qt/widgets/classification/ROIManagerDialog.cpp

  \brief This file has the ROIManagerDialog class.
*/

// TerraLib
#include "ROIManagerDialog.h"
#include "ROIManagerWidget.h"
#include "ui_ROIManagerDialogForm.h"
#include "ui_ROIManagerWidgetForm.h"

// Qt
#include <QGridLayout>

te::qt::widgets::ROIManagerDialog::ROIManagerDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ROIManagerDialogForm)
{
  m_ui->setupUi(this);

//build form
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  m_widgetROIWidget.reset( new te::qt::widgets::ROIManagerWidget(m_ui->m_widget));
  layout->addWidget(m_widgetROIWidget.get(), 0, 0);
  layout->setContentsMargins(0,0,0,0);
}

te::qt::widgets::ROIManagerDialog::~ROIManagerDialog()
{
}

te::qt::widgets::ROIManagerWidget* te::qt::widgets::ROIManagerDialog::getWidget()
{
  return m_widgetROIWidget.get();
}

void te::qt::widgets::ROIManagerDialog::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_widgetROIWidget->setList(layerList);
}

void te::qt::widgets::ROIManagerDialog::set(te::map::AbstractLayerPtr layer)
{
  m_widgetROIWidget->set(layer);
}

void te::qt::widgets::ROIManagerDialog::closeEvent(QCloseEvent* /*e*/)
{
  emit roiManagerClosed();
}
