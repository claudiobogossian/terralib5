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
  \file terralib/plugins/exchanger/DataSetOptionsWizardPage.cpp

  \brief A Qt dialog for ....
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../dataaccess/dataset/Index.h"
#include "../../../dataaccess/dataset/PrimaryKey.h"
#include "../../../dataaccess/dataset/UniqueKey.h"
#include "../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../qt/widgets/utils/ScopedCursor.h"
#include "../../../qt/widgets/srs/SRSManagerDialog.h"
#include "../property/ConstraintsIndexesListWidget.h"
#include "../property/DataSetAdapterWidget.h"
#include "DataSetOptionsWizardPage.h"
#include "ui_DataSetOptionsWizardPageForm.h"

// STL
#include <algorithm>

// Boost
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>

// Qt
#include <QIcon>
#include <QMessageBox>

te::qt::widgets::DataSetOptionsWizardPage::DataSetOptionsWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::DataSetOptionsWizardPageForm)
{
// setup controls
  m_ui->setupUi(this);

//build form
  m_constraintWidget.reset(new te::qt::widgets::ConstraintsIndexesListWidget(m_ui->m_constraintWidget));
  QGridLayout* constraintLayout = new QGridLayout(m_ui->m_constraintWidget);
  constraintLayout->addWidget(m_constraintWidget.get());
  constraintLayout->setContentsMargins(0,0,0,0);

  m_dataSetAdapterWidget.reset(new te::qt::widgets::DataSetAdapterWidget(m_ui->m_dataSetWidget));
  QGridLayout* dataSetLayout = new QGridLayout(m_ui->m_dataSetWidget);
  dataSetLayout->addWidget(m_dataSetAdapterWidget.get());
  dataSetLayout->setContentsMargins(0,0,0,0);

// connect signals and slots
  connect(m_ui->m_sridSearchToolButton, SIGNAL(pressed()), this, SLOT(sridSearchToolButtonPressed()));

  connect(m_ui->m_selectedDatasetListWidget, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(datasetPressed(QListWidgetItem*)));

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

  clearForm();

  m_datasets.clear();

  m_datasource = datasource;
  
  m_targetDatasource = targetDatasource;

  te::da::DataSourcePtr targetDSPtr = te::da::DataSourceManager::getInstance().get(m_targetDatasource->getId(), m_targetDatasource->getType(), m_targetDatasource->getConnInfo()); 

  for(std::list<te::da::DataSetTypePtr>::const_iterator it = datasets.begin(); it != datasets.end(); ++it)
  {
    if(it->get() == 0)
      continue;

    if((*it)->size() == 0)
      te::da::LoadProperties((*it).get(), datasource->getId());

    //create dataset adapter
    te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter((*it).get(), targetDSPtr->getCapabilities(), targetDSPtr->getEncoding());

    //fix output dataset name
    std::string name = converter->getResult()->getName();

    std::size_t idx = name.find(".");
    if (idx != std::string::npos)
    {
        name = name.substr(idx + 1, name.size() - 1);
    }

    converter->getResult()->setName(name);

    //fix primary key name
    if(converter->getResult() && converter->getResult()->getPrimaryKey())
    {
      te::da::PrimaryKey* pk = converter->getResult()->getPrimaryKey();
      pk->setName(converter->getResult()->getName() + "_pk");
    }

    m_datasets.insert(std::map<te::da::DataSetTypePtr, te::da::DataSetTypeConverter*>::value_type((*it), converter));
  }

  for(std::list<te::da::DataSetTypePtr>::const_iterator it = datasets.begin(); it != datasets.end(); ++it)
  {
    QString title = QString::fromStdString((*it)->getTitle());

    QString name = QString::fromStdString((*it)->getName());

    if(title.isEmpty())
      title = name;

    QListWidgetItem* item = new QListWidgetItem(title);

    m_ui->m_selectedDatasetListWidget->addItem(item);

    if(datasets.size() == 1)
    {
      item->setSelected(true);
      datasetPressed(item);
    }
  }

  if(datasets.size() != 1)
    setControlsEnabled(false);
}

const std::map<te::da::DataSetTypePtr, te::da::DataSetTypeConverter*>& te::qt::widgets::DataSetOptionsWizardPage::getDatasets() const
{
  return m_datasets;
}

void te::qt::widgets::DataSetOptionsWizardPage::showSimpleMode(const bool& mode)
{
  m_ui->m_dataSetWidget->setVisible(!mode);
  m_ui->m_constraintWidget->setVisible(!mode);
}

void te::qt::widgets::DataSetOptionsWizardPage::applyChanges()
{
  QListWidgetItem* item = m_ui->m_selectedDatasetListWidget->currentItem();

  if(item == 0)
    return;

  std::string dataSetAdapterName = item->text().toStdString();

  std::map<te::da::DataSetTypePtr, te::da::DataSetTypeConverter*>::iterator it = m_datasets.begin();

  while(it != m_datasets.end())
  {
    if(it->second->getConvertee()->getName() == dataSetAdapterName)
    {
      it->second->getResult()->setName(m_ui->m_datasetNameLineEdit->text().trimmed().toStdString());
      it->second->getResult()->setTitle(m_ui->m_datasetTitleLineEdit->text().trimmed().toStdString());

      if(it->second->getResult()->hasGeom())
      {
        te::gm::GeometryProperty* geomProp = dynamic_cast<te::gm::GeometryProperty*>(it->second->getResult()->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE));

        if(geomProp)
          geomProp->setSRID(boost::lexical_cast<int>(m_ui->m_sridLineEdit->text().trimmed().toStdString()));
      }

      if(it->second->getResult()->getPrimaryKey())
      {
        te::da::PrimaryKey* pk = it->second->getResult()->getPrimaryKey();
        pk->setName(it->second->getResult()->getName() + "_pk");

        // fill constraints
        m_constraintWidget->setDataSetType(it->second->getResult());
      }

      break;
    }
    ++it;
  }
}

void te::qt::widgets::DataSetOptionsWizardPage::sridSearchToolButtonPressed()
{
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the SRS"));

  if(srsDialog.exec() != QDialog::Rejected)
  {
    std::pair<int, std::string> srid = srsDialog.getSelectedSRS();
    m_ui->m_sridLineEdit->setText(QString::number(srid.first));
  }
}

void te::qt::widgets::DataSetOptionsWizardPage::datasetPressed(QListWidgetItem* item)
{
  if(item == 0)
    return;

  std::string dataSetAdapterName = item->text().toStdString();

  std::map<te::da::DataSetTypePtr, te::da::DataSetTypeConverter*>::iterator it = m_datasets.begin();

  while(it != m_datasets.end())
  {
    if(it->second->getConvertee()->getName() == dataSetAdapterName)
    {
      te::da::DataSetType* dataset = it->second->getResult();

      // fill line edits
      m_ui->m_datasetNameLineEdit->setEnabled(true);
      m_ui->m_datasetNameLineEdit->setText(QString::fromStdString(dataset->getName()));

      m_ui->m_datasetTitleLineEdit->setEnabled(true);
      m_ui->m_datasetTitleLineEdit->setText(QString::fromStdString(dataset->getTitle()));

      if(dataset->hasGeom())
      {
        m_ui->m_sridSearchToolButton->setEnabled(true);
        m_ui->m_sridLineEdit->setEnabled(true);

        te::gm::GeometryProperty* geomProp = dynamic_cast<te::gm::GeometryProperty*>(dataset->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE));

        if(geomProp)
          m_ui->m_sridLineEdit->setText(QString::fromStdString(boost::lexical_cast<std::string>(geomProp->getSRID())));
      }
      else
      {
        m_ui->m_sridSearchToolButton->setEnabled(false);
        m_ui->m_sridLineEdit->setEnabled(false);
      }

      // fill property table
      m_dataSetAdapterWidget->setAdapterParameters(it->second->getConvertee(), it->second, m_targetDatasource);

      // fill constraints
      m_constraintWidget->setDataSetType(dataset);
      break;
    }
    ++it;
  }
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
   QListWidgetItem* item = m_ui->m_selectedDatasetListWidget->currentItem();

  if(item == 0)
    return te::da::DataSetTypePtr();

  std::string dataSetAdapterName = item->text().toStdString();

  std::map<te::da::DataSetTypePtr, te::da::DataSetTypeConverter*>::const_iterator it = m_datasets.begin();

  while(it != m_datasets.end())
  {
    if(it->second->getConvertee()->getName() == dataSetAdapterName)
    {
      return it->first;
    }
    ++it;
  }
 
  return te::da::DataSetTypePtr();
}

void te::qt::widgets::DataSetOptionsWizardPage::clearForm()
{
  m_ui->m_datasetNameLineEdit->clear();
  m_ui->m_datasetTitleLineEdit->clear();
  m_ui->m_sridLineEdit->clear();
}

void te::qt::widgets::DataSetOptionsWizardPage::setControlsEnabled(bool enabled)
{
  m_ui->m_datasetNameLineEdit->setEnabled(enabled);
  m_ui->m_datasetTitleLineEdit->setEnabled(enabled);
  m_ui->m_sridLineEdit->setEnabled(enabled);
  m_ui->m_sridSearchToolButton->setEnabled(enabled);
}

te::qt::widgets::DataSetOptionsWizardPage::FindByName::FindByName(const QString& name)
{
  m_name = name.toStdString();
}

bool te::qt::widgets::DataSetOptionsWizardPage::FindByName::operator()(const te::da::DataSetTypePtr& dataset) const
{
  return dataset->getName() == m_name;
}

