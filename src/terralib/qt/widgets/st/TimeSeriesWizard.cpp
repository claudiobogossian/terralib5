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
  \file terralib/qt/widgets/st/ TimeSeriesWizard.cpp

  \brief  A wizard used to generate a new  TimeSerieslayer.
*/

//Terralib
#include "../../../geometry/GeometryProperty.h"
#include "../../../qt/widgets/dataset/selector/DataSetSelectorWizardPage.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWidget.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWizardPage.h"
#include "../../../qt/widgets/help/HelpPushButton.h"
#include "../../../se/Utils.h"
#include "../../../st/core/timeseries/TimeSeriesDataSetInfo.h"
#include "TimeSeriesPropertiesWizardPage.h"
#include "TimeSeriesWizard.h"
#include "ui_TimeSeriesWizardForm.h"

//Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::st::TimeSeriesDataSetLayerPtr generateLayer(te::da::DataSetTypePtr dataType, te::st::TimeSeriesDataSetInfo* timeSeriesInfo, te::da::DataSourceInfoPtr dataInfo, te::map::AbstractLayer* parent = 0)
{
  static boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);
  std::string title = dataType->getTitle().empty() ? dataType->getName() : dataType->getTitle();

  te::st::TimeSeriesDataSetLayerPtr timeSeriesLayer = new te::st::TimeSeriesDataSetLayer(id, title, parent, timeSeriesInfo);
  timeSeriesLayer->setVisibility(te::map::NOT_VISIBLE);
  timeSeriesLayer->setRendererType("ABSTRACT_LAYER_RENDERER");

  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(dataType.get());
  std::auto_ptr<te::gm::Envelope> mbr(te::da::GetExtent(dataType->getName(), gp->getName(), dataInfo->getId()));
  timeSeriesLayer->setSRID(gp->getSRID());
  timeSeriesLayer->setExtent(*mbr);
  timeSeriesLayer->setStyle(te::se::CreateFeatureTypeStyle(gp->getGeometryType()));
  return timeSeriesLayer;
}

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
  connect(this->button(QWizard::FinishButton), SIGNAL(pressed()), this, SLOT(finish()));

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

te::st::TimeSeriesDataSetLayerPtr te::qt::widgets::TimeSeriesWizard::getTimeSeriesLayer()
{
  return m_timeSeriesLayer;
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
    te::st::TimeSeriesDataSetInfo* info = m_PropWidgetPage->getInfo(dataInfo);
    m_timeSeriesLayer = generateLayer(dataTypes.front(), info, dataInfo);

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