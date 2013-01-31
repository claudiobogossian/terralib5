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
  \file terralib/plugins/exchanger/DataSetOptionsWizardPage.cpp

  \brief A Qt dialog for ....
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/Index.h"
#include "../../../dataaccess/dataset/PrimaryKey.h"
#include "../../../dataaccess/dataset/UniqueKey.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../qt/widgets/utils/ScopedCursor.h"
#include "ui_DataSetOptionsWizardPageForm.h"
#include "DataSetOptionsWizardPage.h"

// STL
#include <algorithm>

// Boost
#include <boost/lexical_cast.hpp>

// Qt
#include <QtGui/QIcon>
#include <QtGui/QMessageBox>

te::qt::widgets::DataSetOptionsWizardPage::DataSetOptionsWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::DataSetOptionsWizardPageForm)
{
// setup controls
  m_ui->setupUi(this);

// setup icons
  m_ui->m_addUkToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeUkToolButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_editUkToolButton->setIcon(QIcon::fromTheme("preferences-system"));

  m_ui->m_addIndexToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeIndexToolButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_editIndexToolButton->setIcon(QIcon::fromTheme("preferences-system"));

// setup control options
  m_ui->m_propertiesTableWidget->resizeColumnsToContents();

// connect signals and slots
  connect(m_ui->m_addUkToolButton, SIGNAL(pressed()), this, SLOT(addUkToolButtonPressed()));
  connect(m_ui->m_removeUkToolButton, SIGNAL(pressed()), this, SLOT(removeUkToolButtonPressed()));
  connect(m_ui->m_editUkToolButton, SIGNAL(pressed()), this, SLOT(editUkToolButtonPressed()));

  connect(m_ui->m_addIndexToolButton, SIGNAL(pressed()), this, SLOT(addIndexToolButtonPressed()));
  connect(m_ui->m_removeIndexToolButton, SIGNAL(pressed()), this, SLOT(removeIndexToolButtonPressed()));
  connect(m_ui->m_editIndexToolButton, SIGNAL(pressed()), this, SLOT(editIndexToolButtonPressed()));

  connect(m_ui->m_sridSearchToolButton, SIGNAL(pressed()), this, SLOT(sridSearchToolButtonPressed()));

  connect(m_ui->m_applyChangesPushButton, SIGNAL(pressed()), this, SLOT(applyChangesPushButtonPressed()));

  connect(m_ui->m_selectedDatasetListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(datasetPressed(QListWidgetItem*)));

  connect(m_ui->m_datasetNameLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(enableApplyButton()));
  connect(m_ui->m_datasetTitleLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(enableApplyButton()));

  //QCoreApplication* app = QCoreApplication::instance();
  //connect(app, SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(checkModificationsOnFocusChange(QWidget*, QWidget*)));
}

te::qt::widgets::DataSetOptionsWizardPage::~DataSetOptionsWizardPage()
{
}

void te::qt::widgets::DataSetOptionsWizardPage::set(const std::list<te::da::DataSetTypePtr>& datasets,
                                                    const te::da::DataSourceInfoPtr& datasource,
                                                    const te::da::DataSourceInfoPtr& targetDatasource)
{
  ScopedCursor wcursor(Qt::WaitCursor);

  m_ui->m_selectedDatasetListWidget->clear();
  m_ui->m_applyChangesPushButton->setEnabled(false);

  clearForm();

  m_datasets.clear();

  m_datasource = datasource;
  
  m_targetDatasource = targetDatasource;

  for(std::list<te::da::DataSetTypePtr>::const_iterator it = datasets.begin(); it != datasets.end(); ++it)
  {
    if(it->get() == 0)
      continue;

    te::da::DataSetTypePtr myclone(static_cast<te::da::DataSetType*>((*it)->clone()));

    if(!myclone->isFullLoaded())
      te::da::LoadFull(myclone.get(), datasource->getId());

    m_datasets.push_back(myclone);
  }

  for(std::list<te::da::DataSetTypePtr>::const_iterator it = datasets.begin(); it != datasets.end(); ++it)
  {
    QString title = QString::fromStdString((*it)->getTitle());

    QString name = QString::fromStdString((*it)->getName());

    if(title.isEmpty())
      title = name;

    QListWidgetItem* item = new QListWidgetItem(title);

    //item->setData(Qt::UserRole, QVariant(name));

    m_ui->m_selectedDatasetListWidget->addItem(item);
  }

  setControlsEnabled(false);
}

const std::list<te::da::DataSetTypePtr>& te::qt::widgets::DataSetOptionsWizardPage::getDatasets() const
{
  return m_datasets;
}

void te::qt::widgets::DataSetOptionsWizardPage::addUkToolButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("This option is not implemented yet!\nWe will provide it soon!"));
  //m_ui->m_applyChangesPushButton->setEnabled(true);
}

void te::qt::widgets::DataSetOptionsWizardPage::removeUkToolButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("This option is not implemented yet!\nWe will provide it soon!"));
  //m_ui->m_applyChangesPushButton->setEnabled(true);
}

void te::qt::widgets::DataSetOptionsWizardPage::editUkToolButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("This option is not implemented yet!\nWe will provide it soon!"));
  //m_ui->m_applyChangesPushButton->setEnabled(true);
}

void te::qt::widgets::DataSetOptionsWizardPage::addIndexToolButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("This option is not implemented yet!\nWe will provide it soon!"));
  //m_ui->m_applyChangesPushButton->setEnabled(true);
}

void te::qt::widgets::DataSetOptionsWizardPage::removeIndexToolButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("This option is not implemented yet!\nWe will provide it soon!"));
  //m_ui->m_applyChangesPushButton->setEnabled(true);
}

void te::qt::widgets::DataSetOptionsWizardPage::editIndexToolButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("This option is not implemented yet!\nWe will provide it soon!"));
  //m_ui->m_applyChangesPushButton->setEnabled(true);
}

void te::qt::widgets::DataSetOptionsWizardPage::sridSearchToolButtonPressed()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("This option is not implemented yet!\nWe will provide it soon!"));
  //m_ui->m_applyChangesPushButton->setEnabled(true);
}

void te::qt::widgets::DataSetOptionsWizardPage::applyChangesPushButtonPressed()
{
  QList<QListWidgetItem*> items = m_ui->m_selectedDatasetListWidget->selectedItems();

  if(items.empty())
    return;

  QListWidgetItem* item = items.first();

  if(item == 0)
    return;

  int row = m_ui->m_selectedDatasetListWidget->row(item);

  std::list<te::da::DataSetTypePtr>::iterator it = m_datasets.begin();

  std::advance(it, row);

  if(it == m_datasets.end())
    return;

  (*it)->setName(m_ui->m_datasetNameLineEdit->text().trimmed().toStdString());
  (*it)->setTitle(m_ui->m_datasetTitleLineEdit->text().trimmed().toStdString());

  if((*it)->hasGeom())
    (*it)->getDefaultGeomProperty()->setSRID(boost::lexical_cast<int>(m_ui->m_sridLineEdit->text().trimmed().toStdString()));

  QString title = QString::fromStdString((*it)->getTitle());

  QString name = QString::fromStdString((*it)->getName());

  if(title.isEmpty())
    title = name;

  item->setText(title);

  m_ui->m_selectedDatasetListWidget->update();

  m_ui->m_applyChangesPushButton->setEnabled(false);
}

void te::qt::widgets::DataSetOptionsWizardPage::datasetPressed(QListWidgetItem* item)
{
  if(item == 0)
    return;

  int row = m_ui->m_selectedDatasetListWidget->row(item);

  std::list<te::da::DataSetTypePtr>::const_iterator it = m_datasets.begin();

  std::advance(it, row);

  if(it == m_datasets.end())
    return;

  te::da::DataSetTypePtr dataset = *it;

// fill line edits
  m_ui->m_datasetNameLineEdit->setEnabled(true);
  m_ui->m_datasetNameLineEdit->setText(QString::fromStdString(dataset->getName()));

  m_ui->m_datasetTitleLineEdit->setEnabled(true);
  m_ui->m_datasetTitleLineEdit->setText(QString::fromStdString(dataset->getTitle()));

  if(dataset->hasGeom())
  {
    m_ui->m_sridSearchToolButton->setEnabled(true);
    m_ui->m_sridLineEdit->setEnabled(true);
    m_ui->m_sridLineEdit->setText(QString::fromStdString(boost::lexical_cast<std::string>(dataset->getDefaultGeomProperty()->getSRID())));
  }
  else
  {
    m_ui->m_sridSearchToolButton->setEnabled(false);
    m_ui->m_sridLineEdit->setEnabled(false);
  }

// fill property table
  QTableWidget* table = m_ui->m_propertiesTableWidget;

  const int nproperties = static_cast<int>(dataset->size());

  if(nproperties != 0)
    table->setEnabled(true);

  table->setRowCount(nproperties);

  for(int i = 0; i < nproperties; ++i)
  {
    const te::dt::Property* p = dataset->getProperty(i);

    QTableWidgetItem* item = new QTableWidgetItem("");
    item->setCheckState(Qt::Checked);
    table->setItem(i, 0, item);

    item = new QTableWidgetItem(QString::fromStdString(p->getName()));
    table->setItem(i, 1, item);

    item = new QTableWidgetItem(QString::fromStdString(boost::lexical_cast<std::string>(p->getType())));
    table->setItem(i, 2, item);

    const te::da::PrimaryKey* pk = dataset->getPrimaryKey();

    item = new QTableWidgetItem("");
    item->setCheckState((pk !=0 ) && (pk->has(p)) ? Qt::Checked : Qt::Unchecked);
    table->setItem(i, 3, item);
  }

  table->resizeColumnsToContents();

// fill uks
  m_ui->m_addUkToolButton->setEnabled(true);

  const std::size_t nuks = dataset->getNumberOfUniqueKeys();

  for(std::size_t i = 0; i < nuks; ++i)
  {
    const te::da::UniqueKey* uk = dataset->getUniqueKey(i);

    QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(uk->getName()));

    m_ui->m_uksListWidget->addItem(item);
  }

// fill idxs
  m_ui->m_addIndexToolButton->setEnabled(true);

  const std::size_t nidxs = dataset->getNumberOfIndexes();

  for(std::size_t i = 0; i < nidxs; ++i)
  {
    const te::da::Index* idx = dataset->getIndex(i);

    QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(idx->getName()));

    m_ui->m_indexesListWidget->addItem(item);
  }

// disable buttons
  m_ui->m_applyChangesPushButton->setEnabled(false);
}
void te::qt::widgets::DataSetOptionsWizardPage::enableApplyButton()
{
  m_ui->m_applyChangesPushButton->setEnabled(true);
}

//void te::qt::widgets::DataSetOptionsWizardPage::checkModificationsOnFocusChange(QWidget* old, QWidget* now)
//{
//  if(old == m_ui->m_datasetNameLineEdit)
//  {
//    te::da::DataSetTypePtr dataset = getSelectedDataSet();
//
//    if(dataset.get() == 0)
//      return;
//
//    std::string name = m_ui->m_datasetNameLineEdit->text().trimmed().toStdString();
//
//    if(name != dataset->getName())
//    {
//      dataset->setName(name);
//      m_ui->m_applyChangesPushButton->setEnabled(true);
//    }
//  }
//  else if(old == m_ui->m_datasetTitleLineEdit)
//  {
//    te::da::DataSetTypePtr dataset = getSelectedDataSet();
//
//    if(dataset.get() == 0)
//      return;
//
//    std::string title = m_ui->m_datasetTitleLineEdit->text().trimmed().toStdString();
//
//    if(title != dataset->getTitle())
//    {
//      dataset->setTitle(title);
//      m_ui->m_applyChangesPushButton->setEnabled(true);
//    }
//  }
//  else if(old == m_ui->m_sridLineEdit)
//  {
//    te::da::DataSetTypePtr dataset = getSelectedDataSet();
//
//    if(dataset.get() == 0)
//      return;
//
//    if(!dataset->hasGeom())
//      return;
//
//    te::gm::GeometryProperty* gp = dataset->getDefaultGeomProperty();
//
//    int srid = boost::lexical_cast<int>(m_ui->m_sridLineEdit->text().trimmed().toStdString());
//
//    if(srid != gp->getSRID())
//    {
//      gp->setSRID(srid);
//      m_ui->m_applyChangesPushButton->setEnabled(true);
//    }
//  }
//}

te::da::DataSetTypePtr te::qt::widgets::DataSetOptionsWizardPage::getSelectedDataSet() const
{
  QList<QListWidgetItem*> items = m_ui->m_selectedDatasetListWidget->selectedItems();

  if(items.empty())
    return te::da::DataSetTypePtr();

  QListWidgetItem* item = items.first();

  if(item == 0)
    return te::da::DataSetTypePtr();

  int row = m_ui->m_selectedDatasetListWidget->row(item);

  std::list<te::da::DataSetTypePtr>::const_iterator it = m_datasets.begin();

  std::advance(it, row);

  if(it == m_datasets.end())
    return te::da::DataSetTypePtr();

  return *it;
}

void te::qt::widgets::DataSetOptionsWizardPage::clearForm()
{
  m_ui->m_datasetNameLineEdit->clear();
  m_ui->m_datasetTitleLineEdit->clear();
  m_ui->m_sridLineEdit->clear();
  m_ui->m_propertiesTableWidget->clearContents();
  m_ui->m_uksListWidget->clear();
  m_ui->m_indexesListWidget->clear();
}

void te::qt::widgets::DataSetOptionsWizardPage::setControlsEnabled(bool enabled)
{
  m_ui->m_datasetNameLineEdit->setEnabled(enabled);
  m_ui->m_datasetTitleLineEdit->setEnabled(enabled);
  m_ui->m_sridLineEdit->setEnabled(enabled);
  m_ui->m_sridSearchToolButton->setEnabled(enabled);
  m_ui->m_propertiesTableWidget->setEnabled(enabled);
  m_ui->m_addUkToolButton->setEnabled(enabled);
  m_ui->m_removeUkToolButton->setEnabled(enabled);
  m_ui->m_editUkToolButton->setEnabled(enabled);
  m_ui->m_addIndexToolButton->setEnabled(enabled);
  m_ui->m_removeIndexToolButton->setEnabled(enabled);
  m_ui->m_editIndexToolButton->setEnabled(enabled);
  m_ui->m_applyChangesPushButton->setEnabled(enabled);
}

te::qt::widgets::DataSetOptionsWizardPage::FindByName::FindByName(const QString& name)
{
  m_name = name.toStdString();
}

bool te::qt::widgets::DataSetOptionsWizardPage::FindByName::operator()(const te::da::DataSetTypePtr& dataset) const
{
  return dataset->getName() == m_name;
}

