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
  \file terralib/qt/widgets/st/ObservationWizard.cpp

  \brief  A wizard used to generate a new Observationlayer.
*/

//Terralib
#include "../../../geometry/Envelope.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../qt/widgets/dataset/selector/DataSetSelectorWizardPage.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWidget.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWizardPage.h"
#include "../../../qt/widgets/help/HelpPushButton.h"
#include "../../../se/Utils.h"
#include "../../../st/core/observation/ObservationDataSetInfo.h"
#include "ObservationPropertiesWizardPage.h"
#include "ObservationWizard.h"
#include "ui_ObservationWizardForm.h"

//Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::st::ObservationDataSetLayerPtr generateLayer(te::da::DataSetTypePtr dataType, te::st::ObservationDataSetInfo* obsInfo, te::da::DataSourceInfoPtr dataInfo, te::map::AbstractLayer* parent = 0)
{
  static boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);
  std::string title = dataType->getTitle().empty() ? dataType->getName() : dataType->getTitle();

  te::st::ObservationDataSetLayerPtr observationLayer = new te::st::ObservationDataSetLayer(id, title, parent, obsInfo);
  observationLayer->setVisibility(te::map::NOT_VISIBLE);
  observationLayer->setRendererType("ABSTRACT_LAYER_RENDERER");

  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(dataType.get());
  std::auto_ptr<te::gm::Envelope> mbr(te::da::GetExtent(dataType->getName(), gp->getName(), dataInfo->getId()));
  observationLayer->setSRID(gp->getSRID());
  observationLayer->setExtent(*mbr);
  observationLayer->setStyle(te::se::CreateFeatureTypeStyle(gp->getGeometryType()));
  return observationLayer;
}

te::qt::widgets::ObservationWizard::ObservationWizard(QWidget* parent, Qt::WindowFlags f)
  : QWizard(parent, f),
    m_ui(new Ui::ObservationWizardForm)
{
  m_ui->setupUi(this);

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

  //Observation Properties
  m_PropWidgetPage.reset(new ObservationPropertiesWizardPage(this));
  m_PropWidgetPage->setTitle(tr("Observation Properties"));
  m_PropWidgetPage->setSubTitle(tr("Please, adjust the temporal properties of the new Observation Layer"));
  setPage(PAGE_OBSERVATION_PROPERTIES_SELECTION, m_PropWidgetPage.get());

  // connect signals and slots
  connect(this->button(QWizard::NextButton), SIGNAL(pressed()), this, SLOT(next()));
  connect(this->button(QWizard::BackButton), SIGNAL(pressed()), this, SLOT(back()));
  connect(this->button(QWizard::FinishButton), SIGNAL(pressed()), this, SLOT(finish()));

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);
  this->setButton(QWizard::HelpButton, helpButton);
  //helpButton->setPageReference("widgets/exchanger_all/exchanger_all.html");
}

te::qt::widgets::ObservationWizard::~ObservationWizard()
{
}

te::da::DataSourceInfoPtr te::qt::widgets::ObservationWizard::getDataSource() const
{
  std::list<te::da::DataSourceInfoPtr> datasources = m_datasourceSelectorPage->getSelectorWidget()->getSelecteds();

  if(datasources.empty())
    return te::da::DataSourceInfoPtr();
  else
    return datasources.front();
}

std::list<te::st::ObservationDataSetLayerPtr> te::qt::widgets::ObservationWizard::getObservationLayers()
{
  return m_observationLayers;
}

void te::qt::widgets::ObservationWizard::back()
{
  QWizard::back();
}

void te::qt::widgets::ObservationWizard::next()
{
  if(currentId() == PAGE_DATASOURCE_SELECTION)
  {
    m_datasetSelectorPage->set(getDataSource(), true);
  }
  else if (currentId() == PAGE_DATASET_SELECTION)
  {
    m_PropWidgetPage->set(m_datasetSelectorPage->getCheckedDataSets());
  }
  QWizard::next();
}

void te::qt::widgets::ObservationWizard::finish()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  te::da::DataSourceInfoPtr dataInfo = getDataSource();
  std::list<te::da::DataSetTypePtr> dataTypes = m_datasetSelectorPage->getCheckedDataSets();

  try
  {
    //Generates a random id to the data source
    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id = boost::uuids::to_string(u);
    dataInfo->setId(id);

    //Create the data source and put it into the manager
    te::da::DataSourceManager::getInstance().open(dataInfo->getId(), dataInfo->getType(), dataInfo->getConnInfo());
  }
  catch(const te::common::Exception& e)
  {
    std::cout << std::endl << "Failed to create a new data source and put it into the manager: " << e.what() << std::endl;
    QWizard::finished(1);
  }
  catch(...)
  {
    std::cout << std::endl << "Failed to create a new data source and put it into the manager: unknown exception!" << std::endl;
    QWizard::finished(1);
  }

  try
  {
    std::list<te::st::ObservationDataSetInfo*> infos = m_PropWidgetPage->getInfo(dataInfo);
    std::list<te::st::ObservationDataSetInfo*>::const_iterator infosBegin = infos.begin();
    std::list<te::st::ObservationDataSetInfo*>::const_iterator infosEnd = infos.end();
    std::list<te::da::DataSetTypePtr>::const_iterator typesItBegin = dataTypes.begin();

    if (infos.size() == 1)
    {
      m_observationLayers.push_back(generateLayer(*typesItBegin, *infosBegin, dataInfo));
    }
    else
    {
      static boost::uuids::basic_random_generator<boost::mt19937> gen;
      boost::uuids::uuid u = gen();
      std::string id = boost::uuids::to_string(u);

      while(infosBegin != infosEnd)
      {
        m_observationLayers.push_back(generateLayer(*typesItBegin, *infosBegin, dataInfo));
        infosBegin++;
        typesItBegin++;
      }
    }
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