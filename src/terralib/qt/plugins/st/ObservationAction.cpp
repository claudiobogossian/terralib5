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
  \file terralib/qt/plugins/st/ObservationAction.cpp

  \brief This file defines the Observation Action class
*/

//Terralib
#include "../../../qt/af/events/LayerEvents.h"
#include "../../../qt/widgets/dataset/selector/DataSetSelectorWizardPage.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWizardPage.h"
//#include "../../../qt/widgets/layer/explorer/AbstractTreeItemFactory.h"
#include "../../../qt/widgets/st/ObservationWizard.h"
#include "../../../st/loader/STDataLoader.h"
#include "../../af/ApplicationController.h"
#include "ObservationAction.h"

// Qt
#include <QMessageBox>

//STL
#include <memory>

// Boost
#include <boost/functional/factory.hpp>
#include <boost/bind.hpp>

te::qt::plugins::st::ObservationAction::ObservationAction(QMenu* menu)
: te::qt::plugins::st::AbstractAction(menu)
{
  createAction(tr("Observation...").toStdString(), "observation-layer");
  //te::qt::widgets::AbstractTreeItemFactory::reg("OBSERVATIONDATASETLAYER", boost::bind(boost::factory<ObservationLayerItem*>(),_1, _2));
}

 te::qt::plugins::st::ObservationAction::~ObservationAction()
{
}

void te::qt::plugins::st::ObservationAction::onActionActivated(bool checked)
{
  QWidget* parent = te::qt::af::AppCtrlSingleton::getInstance().getMainWindow();

  std::auto_ptr<te::qt::widgets::ObservationWizard> obsWiz;
  obsWiz.reset( new te::qt::widgets::ObservationWizard(parent));

  int res = obsWiz->exec();
  if (res == QDialog::Accepted)
  {
    //Initialize STDataLoader support
    te::st::STDataLoader::initialize();

    std::list<te::st::ObservationDataSetLayerPtr> layers = obsWiz->getObservationLayers();
    std::list<te::st::ObservationDataSetLayerPtr>::const_iterator layerItB = layers.begin();
    std::list<te::st::ObservationDataSetLayerPtr>::const_iterator layerItE = layers.end();

    while(layerItB != layerItE)
    {
      te::qt::af::evt::LayerAdded evt(*layerItB, 0);
      emit triggered(&evt);

      layerItB++;
    }
  }
}
