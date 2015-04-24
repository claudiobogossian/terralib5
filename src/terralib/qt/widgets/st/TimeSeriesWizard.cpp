/*  Copyright (C) 2010-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/st/ TimeSeriesWizard.cpp

  \brief  A wizard used to generate a new  TimeSerieslayer.
*/

//Terralib
#include "../../../qt/widgets/dataset/selector/DataSetSelectorWizardPage.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWidget.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWizardPage.h"
#include "../../../qt/widgets/help/HelpPushButton.h"
#include "../../../st/core/timeseries/TimeSeriesDataSetInfo.h"
#include "TimeSeriesPropertiesWizardPage.h"
#include "TimeSeriesWizard.h"
#include "ui_TimeSeriesWizardForm.h"


te::qt::widgets::TimeSeriesWizard::TimeSeriesWizard(QWidget* parent, Qt::WindowFlags f)
  : QWizard(parent, f),
    m_ui(new Ui::TimeSeriesWizardForm)
{
// setup controls
  m_ui->setupUi(this);

// add pages

  //DataSource
  m_datasourceSelectorPage.reset(new DataSourceSelectorWizardPage(this));
  m_datasourceSelectorPage->setTitle(tr("Data Source Selection"));
  m_datasourceSelectorPage->setSubTitle(tr("Please, select the data source where the data is stored"));
  m_datasourceSelectorPage->getSelectorWidget()->setSelectionMode(QAbstractItemView::SingleSelection);
  m_datasourceSelectorPage->getSelectorWidget()->showDataSourceWithRasterSupport(false);
  setPage(PAGE_DATASOURCE_SELECTION, m_datasourceSelectorPage.get());

  //DataSet
  m_datasetSelectorPage.reset(new DataSetSelectorWizardPage(this));
  m_datasetSelectorPage->setTitle(tr("Dataset Selection"));
  m_datasetSelectorPage->setSubTitle(tr("Please, select the datasets you want to transfer to another data source"));
  setPage(PAGE_DATASET_SELECTION, m_datasetSelectorPage.get());

  // TimeSeries Properties
  m_PropWidgetPage.reset(new  TimeSeriesPropertiesWizardPage(this));
  setPage(PAGE_TIMESERIES_PROPERTIES_SELECTION, m_PropWidgetPage.get());

  // connect signals and slots
  connect(this->button(QWizard::NextButton), SIGNAL(pressed()), this, SLOT(next()));
  connect(this->button(QWizard::BackButton), SIGNAL(pressed()), this, SLOT(back()));

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);
  this->setButton(QWizard::HelpButton, helpButton);
  //helpButton->setPageReference("widgets/exchanger_all/exchanger_all.html");
}

te::qt::widgets::TimeSeriesWizard::~TimeSeriesWizard()
{
}

te::da::DataSourceInfoPtr te::qt::widgets::TimeSeriesWizard::getDataSource() const
{
  std::list<te::da::DataSourceInfoPtr> datasources = m_datasourceSelectorPage->getSelectorWidget()->getSelecteds();

  if(datasources.empty())
    return te::da::DataSourceInfoPtr();
  else
    return datasources.front();
}

te::da::DataSetTypePtr te::qt::widgets::TimeSeriesWizard::getDatasetType() const
{
  std::list<te::da::DataSetTypePtr> dataTypes = m_datasetSelectorPage->getCheckedDataSets();

  if(dataTypes.empty())
    return te::da::DataSetTypePtr();
  else
    return dataTypes.front();
}

void te::qt::widgets::TimeSeriesWizard::back()
{
  QWizard::back();
}

void te::qt::widgets::TimeSeriesWizard::next()
{
  if(currentId() == PAGE_DATASOURCE_SELECTION)
  {
    m_datasetSelectorPage->set(getDataSource(), true);
  }
  else if (currentId() == PAGE_DATASET_SELECTION)
  {
    m_PropWidgetPage->set(getDatasetType());
  }
  QWizard::next();
}

void te::qt::widgets::TimeSeriesWizard::finish()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  te::da::DataSourceInfoPtr dataInfo = getDataSource();
  std::list<te::da::DataSetTypePtr> dataTypes = m_datasetSelectorPage->getCheckedDataSets();

  try
  {

    //std::list<te::st::TimeSeriesDataSetInfo*> infos = m_PropWidgetPage->getInfo(dataInfo);
    //std::list<te::st::TimeSeriesDataSetInfo*>::const_iterator infosBegin = infos.begin();
    //std::list<te::st::TimeSeriesDataSetInfo*>::const_iterator infosEnd = infos.end();
    //std::list<te::da::DataSetTypePtr>::const_iterator typesItBegin = dataTypes.begin();

    //if (infos.size() == 1)
    //{
    //  m_trajectoryLayers.push_back(generateLayer(*typesItBegin, *infosBegin, dataInfo));
    //}
    //else
    //{

    //  static boost::uuids::basic_random_generator<boost::mt19937> gen;
    //  boost::uuids::uuid u = gen();
    //  std::string id = boost::uuids::to_string(u);

    //  te::st::TrajectoryDataSetLayerPtr testParent(new te::st::TrajectoryDataSetLayer());
    //  testParent->setId(id);

    //  m_trajectoryLayers.push_back(testParent);
    //  while(infosBegin != infosEnd)
    //  {
    //    m_trajectoryLayers.push_back(generateLayer(*typesItBegin, *infosBegin, dataInfo, testParent.get()));
    //    infosBegin++;
    //    typesItBegin++;
    //  }
    //}
  }
  catch(const te::common::Exception& e)
  {
    std::cout << std::endl << "Failed to create a new layer and insert it into the application: " << e.what() << std::endl;
    QWizard::finished(1);
  }
  catch(...)
  {
    std::cout << std::endl << "Failed to create a new layer and insert it into the application: unknown exception!" << std::endl;
    QWizard::finished(1);
  }

  QApplication::restoreOverrideCursor();
  QWizard::finished(0);
}