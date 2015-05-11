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
  \file terralib/qt/widgets/canvas/OverlayDialog.cpp

  \brief This file defines a class for a Overlay Dialog.
*/

// TerraLib
#include "OverlayDialog.h"
#include "OverlayWidget.h"
#include "ui_OverlayWidgetForm.h"
#include "ui_OverlayDialogForm.h"

// Qt
#include <QGridLayout>

te::qt::widgets::OverlayDialog::OverlayDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::OverlayDialogForm)
{
  m_ui->setupUi(this);

//build form
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  m_widgetOverlay.reset( new te::qt::widgets::OverlayWidget(m_ui->m_widget));
  layout->addWidget(m_widgetOverlay.get(), 0, 0);
  layout->setContentsMargins(0,0,0,0);

  this->setWindowTitle(tr("Overlay"));

  this->resize(640, 480);
}

te::qt::widgets::OverlayDialog::~OverlayDialog()
{
}

te::qt::widgets::OverlayWidget* te::qt::widgets::OverlayDialog::getWidget()
{
  return m_widgetOverlay.get();
}

void te::qt::widgets::OverlayDialog::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_widgetOverlay->setList(layerList);
}
