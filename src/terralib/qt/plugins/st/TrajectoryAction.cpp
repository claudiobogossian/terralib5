/*  Copyright (C) 2011-2014 National Institute For Space Research (INPE) - Brazil.

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
#include "../../../qt/widgets/layer/explorer/AbstractTreeItemFactory.h"
#include "../../../qt/widgets/st/TrajectoryDialog.h"
#include "../../af/ApplicationController.h"
#include "TrajectoryAction.h"
#include "TrajectoryLayerItem.h"

// Qt
#include <QMessageBox>

//STL
#include <memory>

// Boost
#include <boost/functional/factory.hpp>
#include <boost/bind.hpp>

te::qt::plugins::st::TrajectoryAction::TrajectoryAction(QMenu* menu)
: te::qt::plugins::st::AbstractAction(menu)
{
  createAction(tr("Trajectory...").toStdString());
  te::qt::widgets::AbstractTreeItemFactory::reg("TRAJECTORYDATASETLAYER", boost::bind(boost::factory<TrajectoryLayerItem*>(),_1, _2));
}

 te::qt::plugins::st::TrajectoryAction::~TrajectoryAction()
{
}

void te::qt::plugins::st::TrajectoryAction::onActionActivated(bool checked)
{
  QWidget* parent = te::qt::af::ApplicationController::getInstance().getMainWindow();
  te::qt::widgets::TrajectoryDialog dlg (0, parent);

  if(dlg.exec() != QDialog::Accepted)
  return;

}