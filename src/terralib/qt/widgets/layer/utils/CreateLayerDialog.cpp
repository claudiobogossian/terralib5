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
\file terralib/qt/widgets/layer/utils/CreateLayerDialog.cpp

\brief This interface is used to create new layer operation.
*/

// TerraLib


#include "CreateLayerDialog.h"
#include "CreateLayerWidget.h"
#include "ui_CreateLayerDialogForm.h"

// Qt
#include <QMessageBox>


te::qt::widgets::CreateLayerDialog::CreateLayerDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::CreateLayerDialogForm)
{
  // add controls
  m_ui->setupUi(this);
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("layer-new").pixmap(50, 50));

  //add new property widget
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  layout->setContentsMargins(0, 0, 0, 0);

  m_createLayerWidget = new te::qt::widgets::CreateLayerWidget(m_ui->m_widget);

  layout->addWidget(m_createLayerWidget);

  //connects
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
}

te::qt::widgets::CreateLayerDialog::~CreateLayerDialog()
{

}

te::map::AbstractLayerPtr te::qt::widgets::CreateLayerDialog::getLayer()
{
  return m_createLayerWidget->getLayer();
}

void te::qt::widgets::CreateLayerDialog::onOkPushButtonClicked()
{
  std::string errorMessage = "";

  bool res = m_createLayerWidget->createLayer(errorMessage);

  if (!res)
  {
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  }

  QMessageBox::information(this, tr("Warning"), tr("Layer Created"));

  accept();
}

