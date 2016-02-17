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
\file terralib/qt/widgets/layer/utils/SaveSelectedObjectsDialog.cpp

\brief This interface is used to create a new layer based on a layer with selected objects.
*/

// TerraLib
#include "SaveSelectedObjectsDialog.h"
#include "SaveSelectedObjectsWidget.h"
#include "ui_SaveSelectedObjectsDialogForm.h"

// Qt
#include <QGridLayout>
#include <QMessageBox>


te::qt::widgets::SaveSelectedObjectsDialog::SaveSelectedObjectsDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::SaveSelectedObjectsDialogForm),
  m_saveSelectedObjectsWidget(0)
{
  // add controls
  m_ui->setupUi(this);
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("layer-save-selected-obj").pixmap(50, 50));

  //add new property widget
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  layout->setContentsMargins(0, 0, 0, 0);

  //Fill Widget
  m_saveSelectedObjectsWidget = new te::qt::widgets::SaveSelectedObjectsWidget(m_ui->m_widget);

  layout->addWidget(m_saveSelectedObjectsWidget);

  //connects
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

}

te::qt::widgets::SaveSelectedObjectsDialog::~SaveSelectedObjectsDialog()
{

}

te::map::AbstractLayerPtr te::qt::widgets::SaveSelectedObjectsDialog::getLayer()
{
  return m_layerResult;
}

void te::qt::widgets::SaveSelectedObjectsDialog::setParameters(te::map::AbstractLayerPtr layer)
{
  std::auto_ptr<te::map::LayerSchema> dsType(layer->getSchema());

  m_saveSelectedObjectsWidget->setParameters(layer);
  m_saveSelectedObjectsWidget->updateWindowComponents();
}

void te::qt::widgets::SaveSelectedObjectsDialog::onOkPushButtonClicked()
{
  std::string errorMessage = "";
  
  this->setCursor(Qt::WaitCursor);
  
  bool res = m_saveSelectedObjectsWidget->execute(errorMessage);
  
  this->setCursor(Qt::ArrowCursor);

  if (!res)
  {
    QMessageBox::warning(this, tr("Warning"), errorMessage.c_str());
    return;
  }

  m_layerResult = m_saveSelectedObjectsWidget->getLayer();

  QMessageBox::information(this, tr("Warning"), tr("Layer was saved."));

  accept();
}

void te::qt::widgets::SaveSelectedObjectsDialog::onCancelPushButtonClicked()
{
  reject();
}