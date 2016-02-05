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
\file terralib/qt/widgets/layer/utils/SaveLayerAsDialog.cpp

\brief This interface is used to create a new layer based on a layer with selected objects.
*/

// TerraLib
#include "SaveLayerAsDialog.h"
#include "SaveLayerAsWidget.h"
#include "ui_SaveLayerAsDialogForm.h"

// Qt
#include <QGridLayout.h>
#include <QMessageBox>


te::qt::widgets::SaveLayerAsDialog::SaveLayerAsDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::SaveLayerAsDialogForm),
  m_saveLayerAsWidget(0)
{
  // add controls
  m_ui->setupUi(this);
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("layer-save-as").pixmap(50, 50));

  //add new property widget
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  layout->setContentsMargins(0, 0, 0, 0);

  //Fill Widget
  m_saveLayerAsWidget = new te::qt::widgets::SaveLayerAsWidget(m_ui->m_widget);

  layout->addWidget(m_saveLayerAsWidget);

  //connects
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

}

te::qt::widgets::SaveLayerAsDialog::~SaveLayerAsDialog()
{

}

void te::qt::widgets::SaveLayerAsDialog::setParameters(te::map::AbstractLayerPtr layer)
{
  std::auto_ptr<te::map::LayerSchema> dsType(layer->getSchema());

  m_saveLayerAsWidget->setParameters(layer);
  m_saveLayerAsWidget->updateWindowComponents();
}

void te::qt::widgets::SaveLayerAsDialog::onOkPushButtonClicked()
{
  std::string errorMessage = "";

  bool res = m_saveLayerAsWidget->execute(errorMessage);

  if (!res)
  {
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  }

  QMessageBox::information(this, tr("Warning"), tr("Layer was saved."));

  accept();
}

void te::qt::widgets::SaveLayerAsDialog::onCancelPushButtonClicked()
{
  reject();
}