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
  \file terralib/qt/plugins/st/TrajectoryAction.cpp

  \brief This file defines the Trajectory Action class
*/


//Terralib
#include "../../../qt/af/events/LayerEvents.h"
#include "../../../qt/widgets/dataset/selector/DataSetSelectorWizardPage.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWizardPage.h"
//#include "../../../qt/widgets/layer/explorer/AbstractTreeItemFactory.h"
#include "../../../qt/widgets/st/TrajectoryWizard.h"
#include "../../../st/loader/STDataLoader.h"
#include "../../af/ApplicationController.h"
#include "TrajectoryAction.h"
#include "TrajectoryLayerItem.h"

// Qt
#include <QMessageBox>
#include <QWizard>
#include <QWizardPage>

//STL
#include <memory>

// Boost
#include <boost/bind.hpp>
#include <boost/functional/factory.hpp>

te::qt::plugins::st::TrajectoryAction::TrajectoryAction(QMenu* menu)
: te::qt::plugins::st::AbstractAction(menu)
{
  createAction(tr("Trajectory...").toStdString(), "trajectory-layer");
  //te::qt::widgets::AbstractTreeItemFactory::reg("TRAJECTORYDATASETLAYER", boost::bind(boost::factory<TrajectoryLayerItem*>(),_1, _2));
}

 te::qt::plugins::st::TrajectoryAction::~TrajectoryAction()
{
}

void te::qt::plugins::st::TrajectoryAction::onActionActivated(bool checked)
{
  QWidget* parent = te::qt::af::AppCtrlSingleton::getInstance().getMainWindow();

  std::auto_ptr<te::qt::widgets::TrajectoryWizard> trajWiz;
  trajWiz.reset( new te::qt::widgets::TrajectoryWizard(parent));

  int res = trajWiz->exec();
  if (res == QDialog::Accepted)
  {
    //Initialize STDataLoader support
    te::st::STDataLoader::initialize();

    std::list<te::st::TrajectoryDataSetLayerPtr> layers = trajWiz->getTrajectoryLayers();
    std::list<te::st::TrajectoryDataSetLayerPtr>::const_iterator layerItB = layers.begin();
    std::list<te::st::TrajectoryDataSetLayerPtr>::const_iterator layerItE = layers.end();

    while(layerItB != layerItE)
    {
      te::qt::af::evt::LayerAdded evt(*layerItB, 0);
      emit triggered(&evt);

      layerItB++;
    }
  }
}
