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
  \file terralib/qt/plugins/st/TimeSeriesAction.cpp

  \brief This file defines the TimeSeriers Action class
*/

//Terralib
#include "../../../qt/af/events/LayerEvents.h"
#include "../../../qt/widgets/dataset/selector/DataSetSelectorWizardPage.h"
#include "../../../qt/widgets/datasource/selector/DataSourceSelectorWizardPage.h"
//#include "../../../qt/widgets/layer/explorer/AbstractTreeItemFactory.h"
#include "../../../qt/widgets/st/TimeSeriesWizard.h"
#include "../../../st/loader/STDataLoader.h"
#include "../../af/ApplicationController.h"
#include "TimeSeriesAction.h"
#include "TimeSeriesLayerItem.h"

// Qt
#include <QMessageBox>

//STL
#include <memory>

// Boost
#include <boost/functional/factory.hpp>
#include <boost/bind.hpp>

te::qt::plugins::st::TimeSeriesAction::TimeSeriesAction(QMenu* menu)
: te::qt::plugins::st::AbstractAction(menu)
{
  createAction(tr("Time Series...").toStdString(), "timeseries-layer");
//  te::qt::widgets::AbstractTreeItemFactory::reg("TIMESERIESDATASETLAYER", boost::bind(boost::factory<TimeSeriesLayerItem*>(),_1, _2));
}

 te::qt::plugins::st::TimeSeriesAction::~TimeSeriesAction()
{
}

void te::qt::plugins::st::TimeSeriesAction::onActionActivated(bool checked)
{
  QWidget* parent = te::qt::af::AppCtrlSingleton::getInstance().getMainWindow();

  std::auto_ptr<te::qt::widgets::TimeSeriesWizard> timeWiz;
  timeWiz.reset( new te::qt::widgets::TimeSeriesWizard(parent));

  int res = timeWiz->exec();
  if (res == QDialog::Accepted)
  {
    //Initialize STDataLoader support
    te::st::STDataLoader::initialize();

    te::qt::af::evt::LayerAdded evt(timeWiz->getTimeSeriesLayer(), 0);
    emit triggered(&evt);
  }
}
