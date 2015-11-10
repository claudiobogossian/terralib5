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
  \file terralib/qt/plugins/terralib4/TL4LayerSelectionWizardPage.cpp

  \brief A wizard page for selecting the TerraLib 4.x.
*/

// TerraLib
#include "ui_TL4LayerSelectionWizardPageForm.h"
#include "TL4LayerSelectionWizardPage.h"

// Qt
#include<QIcon>
#include<QListWidgetItem>

QIcon getImage(int type)
{
  switch(type)
  {
    case 0:
      return QIcon::fromTheme("tl4-layer").pixmap(16,16);

    case 1:
      return QIcon::fromTheme("tl4-theme").pixmap(16,16);

    case 2:
      return QIcon::fromTheme("tl4-table").pixmap(16,16);

    case 3:
      return QIcon::fromTheme("tl4-raster").pixmap(16,16);

    default:
      return QIcon::fromTheme("tl4-layer").pixmap(16,16);
  }
}

te::qt::plugins::terralib4::TL4LayerSelectionWizardPage::TL4LayerSelectionWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::TL4LayerSelectionWizardPageForm)
{
// setup controls
  m_ui->setupUi(this);

  connect(m_ui->m_selectAllPushButton, SIGNAL(clicked()), this, SLOT(onSelectAllPushButtonClicked()));
  connect(m_ui->m_deselectAllPushButton, SIGNAL(clicked()), this, SLOT(onDeselectAllPushButtonClicked()));

  // add icons
  m_ui->m_imgVectorLabel->setPixmap(QIcon::fromTheme("tl4-layer").pixmap(16,16));
  m_ui->m_imgTabularLabel->setPixmap(QIcon::fromTheme("tl4-table").pixmap(16,16));
  m_ui->m_imgRasterLabel->setPixmap(QIcon::fromTheme("tl4-raster").pixmap(16,16));
}

te::qt::plugins::terralib4::TL4LayerSelectionWizardPage::~TL4LayerSelectionWizardPage()
{
}

void te::qt::plugins::terralib4::TL4LayerSelectionWizardPage::setDatasets(std::vector<std::string> layers,
                                                                          std::vector<std::string> tables,
                                                                          std::vector<std::string> rasters,
                                                                          std::vector<std::pair<std::string, std::string> > rasterFiles)

{
  m_ui->m_layersListWidget->clear();

  setTL4Layers(layers);
  setTL4Tables(tables);
  setTL4Rasters(rasters);
  setTL4RasterFiles(rasterFiles);
}

void te::qt::plugins::terralib4::TL4LayerSelectionWizardPage::setTL4Layers(std::vector<std::string> layers)
{
  for(std::size_t i = 0; i < layers.size(); ++i)
  {
    QListWidgetItem* item = new QListWidgetItem(getImage(LAYER), QString::fromLatin1(layers[i].c_str()), m_ui->m_layersListWidget, LAYER);
    item->setCheckState(Qt::Checked);
    m_ui->m_layersListWidget->addItem(item);
  }
}

void te::qt::plugins::terralib4::TL4LayerSelectionWizardPage::setTL4RasterFiles(std::vector<std::pair<std::string, std::string> > rasterFiles)
{
  for (std::size_t i = 0; i < rasterFiles.size(); ++i)
  {
    QListWidgetItem* item = new QListWidgetItem(getImage(RASTER), QString::fromLatin1(rasterFiles[i].first.c_str()) + " (" + tr("Not supported") + "!)", m_ui->m_layersListWidget, RASTER);
    item->setCheckState(Qt::Unchecked);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setToolTip(QString::fromLatin1(rasterFiles[i].second.c_str()));
    m_ui->m_layersListWidget->addItem(item);
  }
}

void te::qt::plugins::terralib4::TL4LayerSelectionWizardPage::setTL4Tables(std::vector<std::string> tables)
{
  for(std::size_t i = 0; i < tables.size(); ++i)
  {
    QListWidgetItem* item = new QListWidgetItem(getImage(TABLE), QString::fromLatin1(tables[i].c_str()), m_ui->m_layersListWidget, TABLE);
    item->setCheckState(Qt::Checked);
    m_ui->m_layersListWidget->addItem(item);
  }
}

void te::qt::plugins::terralib4::TL4LayerSelectionWizardPage::setTL4Rasters(std::vector<std::string> rasters)
{
  for(std::size_t i = 0; i < rasters.size(); ++i)
  {
    QListWidgetItem* item = new QListWidgetItem(getImage(RASTER), QString::fromLatin1(rasters[i].c_str()), m_ui->m_layersListWidget, RASTER);
    item->setCheckState(Qt::Checked);
    m_ui->m_layersListWidget->addItem(item);
  }
}

std::vector<std::string> te::qt::plugins::terralib4::TL4LayerSelectionWizardPage::getChecked()
{
  std::vector<std::string> checked;
  for(int i = 0; i < m_ui->m_layersListWidget->count(); ++i)
  {
    if(m_ui->m_layersListWidget->item(i)->checkState() == Qt::Checked)
      checked.push_back(m_ui->m_layersListWidget->item(i)->text().toStdString());
  }

  return checked;
}

bool te::qt::plugins::terralib4::TL4LayerSelectionWizardPage::hasChecked() const
{
  for(int i = 0; i < m_ui->m_layersListWidget->count(); ++i)
  {
    if(m_ui->m_layersListWidget->item(i)->checkState() == Qt::Checked)
      return true;
  }

  return false;
}

void te::qt::plugins::terralib4::TL4LayerSelectionWizardPage::onSelectAllPushButtonClicked()
{
  for(int i = 0; i < m_ui->m_layersListWidget->count(); ++i)
  {
    Qt::CheckState state = m_ui->m_layersListWidget->item(i)->checkState();
    if(state != Qt::Checked)
      m_ui->m_layersListWidget->item(i)->setCheckState(Qt::Checked);
  }
}

void te::qt::plugins::terralib4::TL4LayerSelectionWizardPage::onDeselectAllPushButtonClicked()
{
  for(int i = 0; i < m_ui->m_layersListWidget->count(); ++i)
  {
    Qt::CheckState state = m_ui->m_layersListWidget->item(i)->checkState();
    if(state == Qt::Checked)
      m_ui->m_layersListWidget->item(i)->setCheckState(Qt::Unchecked);
  }
}

std::vector<QListWidgetItem*> te::qt::plugins::terralib4::TL4LayerSelectionWizardPage::getCheckedItems()
{
  std::vector<QListWidgetItem*> checked;
  for(int i = 0; i < m_ui->m_layersListWidget->count(); ++i)
  {
    if(m_ui->m_layersListWidget->item(i)->checkState() == Qt::Checked)
      checked.push_back(m_ui->m_layersListWidget->item(i));
  }

  return checked;
}