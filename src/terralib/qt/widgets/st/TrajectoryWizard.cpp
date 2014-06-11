/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
#include "../../../qt/widgets/dataset/selector/DataSetSelectorWizardPage.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWidget.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWizardPage.h"
#include "../../../qt/widgets/help/HelpPushButton.h"
#include "TemporalPropertiesWidget.h"
#include "TrajectoryPropertiesWidget.h"
#include "TrajectoryWizard.h"
#include "ui_TrajectoryWizardForm.h"


te::qt::widgets::TrajectoryWizard::TrajectoryWizard(QWidget* parent, Qt::WindowFlags f)
  : QWizard(parent, f),
    m_ui(new Ui::TrajectoryWizardForm)
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

  //Temporal Properties
  setPage(PAGE_TEMPORAL_PROPERTIES_SELECTION, new QWizardPage(m_propertiesWidget.get()));

  //Trajectory Properties
  setPage(PAGE_TRAJECTORY_PROPERTIES_SELECTION, new QWizardPage(m_uniquePropWidget.get()));

  // connect signals and slots
  connect(this->button(QWizard::NextButton), SIGNAL(pressed()), this, SLOT(next()));
  connect(this->button(QWizard::BackButton), SIGNAL(pressed()), this, SLOT(back()));

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
    m_propertiesWidget.reset(new te::qt::widgets::TemporalPropertiesWidget(m_datasetSelectorPage->getCheckedDataSets().front().get()));
  }
  QWizard::next();
}