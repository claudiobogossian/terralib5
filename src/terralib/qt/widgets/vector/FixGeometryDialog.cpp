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
  \file terralib/qt/widgets/raster/FixGeometryDialog.cpp

  \brief A dialog to fix geometry.
*/

// TerraLib
#include "../../../common/Translator.h"
#include "../Exception.h"
#include "FixGeometryDialog.h"
#include "FixGeometryWidget.h"
#include "ui_FixGeometryDialogForm.h"
#include "ui_FixGeometryWidgetForm.h"

// Qt
#include <QMessageBox>
#include <QGridLayout>

te::qt::widgets::FixGeometryDialog::FixGeometryDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::FixGeometryDialogForm)
{
// add controls
  m_ui->setupUi(this);

  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("fixgeom-hint").pixmap(112,48));

//add widget
  m_widget.reset(new FixGeometryWidget(m_ui->m_widget, f));

  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  layout->addWidget(m_widget.get());
  layout->setContentsMargins(0,0,0,0);
  m_ui->m_widget->setLayout(layout);

// connect signal and slots
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
}

te::qt::widgets::FixGeometryDialog::~FixGeometryDialog()
{
}

te::qt::widgets::FixGeometryWidget* te::qt::widgets::FixGeometryDialog::getWidget() const
{
  return m_widget.get();
}

void te::qt::widgets::FixGeometryDialog::setLayerList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_widget->setLayerList(layerList);
}

void te::qt::widgets::FixGeometryDialog::onOkPushButtonClicked()
{
  bool res = m_widget->fixGeometry();

  if(!res)
    return;

  accept();
}

void te::qt::widgets::FixGeometryDialog::onCancelPushButtonClicked()
{
  reject();
}