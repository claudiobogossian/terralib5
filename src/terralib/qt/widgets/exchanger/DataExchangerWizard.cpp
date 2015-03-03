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
  \file terralib/plugins/exchanger/DataExchangerWizard.cpp

  \brief A Qt dialog for ....
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetAdapter.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../qt/widgets/dataset/selector/DataSetSelectorWizardPage.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWidget.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWizardPage.h"
#include "../../../qt/widgets/help/HelpPushButton.h"
#include "../../../qt/widgets/utils/ScopedCursor.h"
#include "DataExchangerWizard.h"
#include "DataExchangeStatus.h"
#include "DataExchangeSummaryWizardPage.h"
#include "DataSetOptionsWizardPage.h"
#include "ui_DataExchangerWizardForm.h"
#include "ui_DataExchangeSummaryWizardPageForm.h"
#include "ui_DataSetOptionsWizardPageForm.h"
#include "ui_DataSourceSelectorWidgetForm.h"


// STL
#include <cassert>

// Boost
//#include <boost/chrono.hpp>

// Qt
#include <QAbstractButton>
#include <QMessageBox>
#include <QVBoxLayout>

te::qt::widgets::DataExchangerWizard::DataExchangerWizard(QWidget* parent, Qt::WindowFlags f)
  : QWizard(parent, f),
    m_ui(new Ui::DataExchangerWizardForm)
{
// setup controls
  m_ui->setupUi(this);

// add pages
  m_datasourceSelectorPage.reset(new DataSourceSelectorWizardPage(this));
  m_datasourceSelectorPage->setTitle(tr("Data Source Selection"));
  m_datasourceSelectorPage->setSubTitle(tr("Please, select the data source where the data is stored"));
  m_datasourceSelectorPage->getSelectorWidget()->setSelectionMode(QAbstractItemView::SingleSelection);
  //m_datasourceSelectorPage->getSelectorWidget()->setButtonsEnabled(false);
  m_datasourceSelectorPage->getSelectorWidget()->getForm()->m_mainLayout->setStretch(0, 49);
  m_datasourceSelectorPage->getSelectorWidget()->getForm()->m_mainLayout->setStretch(1, 100);

  m_datasourceSelectorPage->getSelectorWidget()->showDataSourceWithRasterSupport(false);

  setPage(PAGE_DATASOURCE_SELECTION, m_datasourceSelectorPage.get());

  m_datasetSelectorPage.reset(new DataSetSelectorWizardPage(this));
  m_datasetSelectorPage->setTitle(tr("Dataset Selection"));
  m_datasetSelectorPage->setSubTitle(tr("Please, select the datasets you want to transfer to another data source"));
  setPage(PAGE_DATASET_SELECTION, m_datasetSelectorPage.get());

  m_targetSelectorPage.reset(new DataSourceSelectorWizardPage(this));
  m_targetSelectorPage->setTitle(tr("Target Data Source"));
  m_targetSelectorPage->setSubTitle(tr("Please, select the target data source"));
  m_targetSelectorPage->getSelectorWidget()->setSelectionMode(QAbstractItemView::SingleSelection);
  //m_targetSelectorPage->getSelectorWidget()->setButtonsEnabled(false);
  m_targetSelectorPage->getSelectorWidget()->getForm()->m_mainLayout->setStretch(0, 49);
  m_targetSelectorPage->getSelectorWidget()->getForm()->m_mainLayout->setStretch(1, 100);
  setPage(PAGE_TARGET_DATASOURCE, m_targetSelectorPage.get());

  m_datasetOptionsPage.reset(new DataSetOptionsWizardPage(this));
  //m_datasetOptionsPage->showSimpleMode(true); // USED TO HIDE ADVANCED OPTIONS
  //m_datasetOptionsPage->setFinalPage(true);
  m_datasetOptionsPage->setCommitPage(true);
  //m_datasetOptionsPage->setTitle(tr("Transfer Options"));
  //m_datasetOptionsPage->setSubTitle(tr("You can provide more information on how the datasets will be transferred and mapped to the target data source"));
  setPage(PAGE_DATASET_OPTIONS, m_datasetOptionsPage.get());


  m_summaryPage.reset(new DataExchangeSummaryWizardPage(this));
  setPage(PAGE_SUMMARY, m_summaryPage.get());

// connect signals and slots
  connect(this->button(QWizard::NextButton), SIGNAL(pressed()), this, SLOT(next()));
  connect(this->button(QWizard::BackButton), SIGNAL(pressed()), this, SLOT(back()));
  connect(this->button(QWizard::CommitButton), SIGNAL(pressed()), this, SLOT(commit()));
  //connect(this->button(QWizard::FinishButton), SIGNAL(pressed()), this, SLOT(finish()));

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("widgets/exchanger_all/exchanger_all.html");
}

te::qt::widgets::DataExchangerWizard::~DataExchangerWizard()
{
}

int te::qt::widgets::DataExchangerWizard::nextId() const
{
  return QWizard::nextId();
}

te::da::DataSourceInfoPtr te::qt::widgets::DataExchangerWizard::getDataSource() const
{
  std::list<te::da::DataSourceInfoPtr> datasources = m_datasourceSelectorPage->getSelectorWidget()->getSelecteds();

  if(datasources.empty())
    return te::da::DataSourceInfoPtr();
  else
    return datasources.front();
}

te::da::DataSourceInfoPtr te::qt::widgets::DataExchangerWizard::getTargetDataSource() const
{
  std::list<te::da::DataSourceInfoPtr> datasources = m_targetSelectorPage->getSelectorWidget()->getSelecteds();

  if(datasources.empty())
    return te::da::DataSourceInfoPtr();
  else
    return datasources.front();
}

void te::qt::widgets::DataExchangerWizard::back()
{
  this->setOption(QWizard::HaveCustomButton1, false);

  if(currentId() == PAGE_SUMMARY)
  {
    this->setButtonText(QWizard::CustomButton1, tr("Apply"));
    this->setOption(QWizard::HaveCustomButton1, true);

    connect(this->button(QWizard::CustomButton1), SIGNAL(clicked()), m_datasetOptionsPage.get(), SLOT(applyChanges()));
  }

  QWizard::back();
}

void te::qt::widgets::DataExchangerWizard::next()
{
  this->setOption(QWizard::HaveCustomButton1, false);

  if(currentId() == PAGE_DATASOURCE_SELECTION)
  {
    m_datasetSelectorPage->set(getDataSource(), true);
  }
  else if(currentId() == PAGE_TARGET_DATASOURCE)
  {
    std::list<te::da::DataSetTypePtr> datasets = m_datasetSelectorPage->getCheckedDataSets();
    m_datasetOptionsPage->set(datasets, getDataSource(), getTargetDataSource());

    this->setButtonText(QWizard::CustomButton1, tr("Apply"));
    this->setOption(QWizard::HaveCustomButton1, true);

    connect(this->button(QWizard::CustomButton1), SIGNAL(clicked()), m_datasetOptionsPage.get(), SLOT(applyChanges()));
  }

  QWizard::next();
}

void te::qt::widgets::DataExchangerWizard::commit()
{
  ScopedCursor wcursor(Qt::WaitCursor);

// get input data source
  te::da::DataSourceInfoPtr ids = getDataSource();

  if(ids.get() == 0)
    return;

  te::da::DataSourcePtr idatasource = te::da::DataSourceManager::getInstance().get(ids->getId(), ids->getAccessDriver(), ids->getConnInfo());

  if(idatasource.get() == 0)
    return;

// get output data source
  te::da::DataSourceInfoPtr ods = getTargetDataSource();

  if(ods.get() == 0)
    return;

  te::da::DataSourcePtr odatasource = te::da::DataSourceManager::getInstance().get(ods->getId(), ods->getAccessDriver(), ods->getConnInfo());

  if(odatasource.get() == 0)
    return;

// get selected datasets and modified datasets
  std::list<DataExchangeStatus> result;

  std::map<te::da::DataSetTypePtr, te::da::DataSetTypeConverter*> odatasets = m_datasetOptionsPage->getDatasets();

  std::map<te::da::DataSetTypePtr, te::da::DataSetTypeConverter*>::iterator it = odatasets.begin();

  while(it != odatasets.end())
  {
    te::da::DataSetTypePtr idset = it->first;
    te::da::DataSetType* odset = it->second->getResult();
    te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(odset);

    std::auto_ptr<te::da::DataSourceTransactor> t;

    try
    {
      std::map<std::string,std::string> nopt;
      
      //boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();

      std::auto_ptr<te::da::DataSet> dataset(idatasource->getDataSet(idset->getName()));

      t = odatasource->getTransactor();

      t->begin();

      // stay tunned: create can change idset!
      t->createDataSet(odset, nopt);

      std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(dataset.get(), it->second));
      if(geomProp)
        dsAdapter->setSRID(geomProp->getSRID());

      if(dataset->moveBeforeFirst())
        t->add(odset->getName(), dsAdapter.get(), ods->getConnInfo());

      // boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;

      DataExchangeStatus status;
      status.m_dataset = odset;
      status.m_successful = true;
      //status.m_time = sec;

      result.push_back(status);

      t->commit();
    }
    catch(const std::exception& e)
    {
      DataExchangeStatus status;
      status.m_dataset = odset;
      status.m_successful = false;
      status.m_exceptionMsg = e.what();

      result.push_back(status);

      t->rollBack();
    }
    catch(...)
    {
      DataExchangeStatus status;
      status.m_dataset = odset;
      status.m_successful = false;
      status.m_exceptionMsg = tr("Unknown error!").toStdString();

      result.push_back(status);

      t->rollBack();
    }

    ++it;
  }

  m_summaryPage->set(result);

  next();
}


