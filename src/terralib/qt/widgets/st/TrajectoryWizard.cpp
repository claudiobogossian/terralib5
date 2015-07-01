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
  \file terralib/qt/widgets/st/TrajectoryWizard.cpp

  \brief  A wizard used to generate a new Trajectorylayer.
*/

//Terralib
#include "../../../geometry/GeometryProperty.h"
#include "../../../qt/widgets/dataset/selector/DataSetSelectorWizardPage.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWidget.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWizardPage.h"
#include "../../../qt/widgets/help/HelpPushButton.h"
#include "../../../se/Utils.h"
#include "../../../st/core/trajectory/TrajectoryDataSetInfo.h"
#include "TrajectoryPropertiesWizardPage.h"
#include "TrajectoryWizard.h"
#include "ui_TrajectoryWizardForm.h"

//Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::st::TrajectoryDataSetLayerPtr generateLayer(te::da::DataSetTypePtr dataType, te::st::TrajectoryDataSetInfo* trajInfo, te::da::DataSourceInfoPtr dataInfo, te::map::AbstractLayer* parent = 0)
{
  static boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);
  std::string title = dataType->getTitle().empty() ? dataType->getName() : dataType->getTitle();

  te::st::TrajectoryDataSetLayerPtr trajectoryLayer = new te::st::TrajectoryDataSetLayer(id, title, parent, trajInfo);
  trajectoryLayer->setVisibility(te::map::NOT_VISIBLE);
  trajectoryLayer->setRendererType("ABSTRACT_LAYER_RENDERER");

  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(dataType.get());
  std::auto_ptr<te::gm::Envelope> mbr(te::da::GetExtent(dataType->getName(), gp->getName(), dataInfo->getId()));
  trajectoryLayer->setSRID(gp->getSRID());
  trajectoryLayer->setExtent(*mbr);
  trajectoryLayer->setStyle(te::se::CreateFeatureTypeStyle(gp->getGeometryType()));
  return trajectoryLayer;
}

te::qt::widgets::TrajectoryWizard::TrajectoryWizard(QWidget* parent, Qt::WindowFlags f)
  : QWizard(parent, f),
    m_ui(new Ui::TrajectoryWizardForm)
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

  //Trajectory Properties
  m_PropWidgetPage.reset(new TrajectoryPropertiesWizardPage(this));
  m_PropWidgetPage->setTitle(tr("Trajectory Properties"));
  m_PropWidgetPage->setSubTitle(tr("Please, adjust the temporal properties of the new Trajectory Layer"));
  setPage(PAGE_TRAJECTORY_PROPERTIES_SELECTION, m_PropWidgetPage.get());

  // connect signals and slots
  connect(this->button(QWizard::NextButton), SIGNAL(pressed()), this, SLOT(next()));
  connect(this->button(QWizard::BackButton), SIGNAL(pressed()), this, SLOT(back()));
  connect(this->button(QWizard::FinishButton), SIGNAL(pressed()), this, SLOT(finish()));

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);
  this->setButton(QWizard::HelpButton, helpButton);
  //helpButton->setPageReference("widgets/exchanger_all/exchanger_all.html");
}

te::qt::widgets::TrajectoryWizard::~TrajectoryWizard()
{
}

te::da::DataSourceInfoPtr te::qt::widgets::TrajectoryWizard::getDataSource() const
{
  std::list<te::da::DataSourceInfoPtr> datasources = m_datasourceSelectorPage->getSelectorWidget()->getSelecteds();

  if(datasources.empty())
    return te::da::DataSourceInfoPtr();
  else
    return datasources.front();
}

std::list<te::st::TrajectoryDataSetLayerPtr> te::qt::widgets::TrajectoryWizard::getTrajectoryLayers()
{
  return m_trajectoryLayers;
}

void te::qt::widgets::TrajectoryWizard::back()
{
  QWizard::back();
}

void te::qt::widgets::TrajectoryWizard::next()
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

void te::qt::widgets::TrajectoryWizard::finish()
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  te::da::DataSourceInfoPtr dataInfo = getDataSource();
  std::list<te::da::DataSetTypePtr> dataTypes = m_datasetSelectorPage->getCheckedDataSets();

  try
  {
    std::list<te::st::TrajectoryDataSetInfo*> infos = m_PropWidgetPage->getInfo(dataInfo);
    std::list<te::st::TrajectoryDataSetInfo*>::const_iterator infosBegin = infos.begin();
    std::list<te::st::TrajectoryDataSetInfo*>::const_iterator infosEnd = infos.end();
    std::list<te::da::DataSetTypePtr>::const_iterator typesItBegin = dataTypes.begin();

    if (infos.size() == 1)
    {
      m_trajectoryLayers.push_back(generateLayer(*typesItBegin, *infosBegin, dataInfo));
    }
    else
    {
      while(infosBegin != infosEnd)
      {
        m_trajectoryLayers.push_back(generateLayer(*typesItBegin, *infosBegin, dataInfo));
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