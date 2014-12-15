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
  \file terralib/edit/qt/SnapOptionsWidget.cpp

  \brief A dialog used to configure geometry snap options.
*/

// TerraLib
#include "../../dataaccess/dataset/DataSet.h"
#include "../Snap.h"
#include "../SnapManager.h"
#include "SnapOptionsDialog.h"
#include "ui_SnapOptionsDialogForm.h"

// Qt
#include <QCheckBox>
#include <QComboBox>
#include <QTableWidgetItem>

// STL
#include <cassert>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::edit::SnapOptionsDialog::SnapOptionsDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::SnapOptionsDialogForm)
{
  m_ui->setupUi(this);

  // Signals & slots
  connect(m_ui->m_okPushButton, SIGNAL(pressed()), this, SLOT(onOkPushButtonPressed()));
}

te::edit::SnapOptionsDialog::~SnapOptionsDialog()
{
}

void te::edit::SnapOptionsDialog::setLayers(const std::list<te::map::AbstractLayerPtr>& layers)
{
  m_layers = layers;

  buildOptions();
}

void te::edit::SnapOptionsDialog::buildOptions()
{
  for(std::list<te::map::AbstractLayerPtr>::const_iterator it = m_layers.begin(); it != m_layers.end(); ++it)
    buildOptions(*it);
}

void te::edit::SnapOptionsDialog::buildOptions(const te::map::AbstractLayerPtr& layer)
{
  for(std::size_t i = 0; i < layer->getChildrenCount(); ++i)
  {
    te::map::AbstractLayerPtr child(boost::dynamic_pointer_cast<te::map::AbstractLayer>(layer->getChild(i)));
    buildOptions(child);
  }

  if(layer->getType() == "FOLDER_LAYER")
    return;

  int row = m_ui->m_tableOptionsWidget->rowCount() + 1;
  m_ui->m_tableOptionsWidget->setRowCount(row);

  int currentRow = row - 1;
  int currentCollumn = 0;

  // Enable
  QCheckBox* enableCheckBox = new QCheckBox(m_ui->m_tableOptionsWidget);

  if(SnapManager::getInstance().hasSnap(layer->getId()))
    enableCheckBox->setChecked(true);

  m_ui->m_tableOptionsWidget->setCellWidget(currentRow, currentCollumn++, enableCheckBox);

  // Layer
  QTableWidgetItem* layerItem = new QTableWidgetItem(layer->getTitle().c_str());
  layerItem->setTextAlignment(Qt::AlignCenter);
  layerItem->setFlags(Qt::ItemIsEnabled);
  layerItem->setData(Qt::UserRole, QVariant::fromValue(layer));
  m_ui->m_tableOptionsWidget->setItem(currentRow, currentCollumn++, layerItem);

  // Snap Mode
  QComboBox* modeComboBox = new QComboBox(m_ui->m_tableOptionsWidget);
  modeComboBox->addItem(tr("vertex"));
  m_ui->m_tableOptionsWidget->setCellWidget(currentRow, currentCollumn++, modeComboBox);

  // Tolerance
  QTableWidgetItem* toleranceItem = new QTableWidgetItem("16");
  toleranceItem->setTextAlignment( Qt::AlignCenter);

  if(SnapManager::getInstance().hasSnap(layer->getId()))
  {
    Snap* snap = SnapManager::getInstance().getSnap(layer->getId());
    toleranceItem->setText(QString::number(snap->getTolerance()));
  }

  m_ui->m_tableOptionsWidget->setItem(currentRow, currentCollumn++, toleranceItem);

  // Units
  QComboBox* unitsComboBox = new QComboBox(m_ui->m_tableOptionsWidget);
  unitsComboBox->addItem(tr("pixels"));
  m_ui->m_tableOptionsWidget->setCellWidget(currentRow, currentCollumn++, unitsComboBox);
}

void te::edit::SnapOptionsDialog::onOkPushButtonPressed()
{
  for(int i = 0; i < m_ui->m_tableOptionsWidget->rowCount(); ++i) // for each option
  {
    QCheckBox* enableCheckBox = dynamic_cast<QCheckBox*>(m_ui->m_tableOptionsWidget->cellWidget(i, 0));

    // Get the layer
    te::map::AbstractLayerPtr layer = m_ui->m_tableOptionsWidget->item(i, 1)->data(Qt::UserRole).value<te::map::AbstractLayerPtr>();

    // TODO: mode, tolerance and units

    if(enableCheckBox->isChecked())
    {
      if(SnapManager::getInstance().hasSnap(layer->getId()) == false)
      {
        // Build the snap
        std::auto_ptr<te::da::DataSet> dataset(layer->getData());
        SnapManager::getInstance().buildSnap(layer->getId(), layer->getSRID(), dataset.get());
      }
    }
    else
    {
      // Remove the snap
      SnapManager::getInstance().removeSnap(layer->getId());
    }
  }

  close();
}
