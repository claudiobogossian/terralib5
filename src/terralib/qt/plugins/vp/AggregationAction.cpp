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
  \file terralib/qt/plugins/vp/AggregationAction.cpp

  \brief This file defines the Aggregation class
*/

// Terralib
#include "../../../vp/qt/AggregationDialog.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/Project.h"
#include "AggregationAction.h"

// Qt
#include <QObject>
#include <QMessageBox>

// STL
#include <memory>

te::qt::plugins::vp::AggregationAction::AggregationAction(QMenu* menu)
  : te::qt::plugins::vp::AbstractAction(menu)
{
  createAction(tr("Aggregation...").toStdString(), "vector-processing-aggregation");
  m_action->setObjectName("Processing.Vector Processing.Aggregation");
}

te::qt::plugins::vp::AggregationAction::~AggregationAction()
{
}

void te::qt::plugins::vp::AggregationAction::onActionActivated(bool checked)
{
  QWidget* parent = te::qt::af::ApplicationController::getInstance().getMainWindow();
  te::vp::AggregationDialog dlg(parent);

  // get the list of layers from current project
  te::qt::af::Project* prj = te::qt::af::ApplicationController::getInstance().getProject();

  if(prj)
  {
    std::list<te::map::AbstractLayerPtr> layers = prj->getSelectedLayers(false);

    if (layers.size() > 0)
      dlg.setLayers(prj->getSingleLayers(false), *layers.begin());
    else
      dlg.setLayers(prj->getSingleLayers(false));
  }

  if(dlg.exec() != QDialog::Accepted)
    return;

  te::map::AbstractLayerPtr layer = dlg.getLayer();

  if(!layer)
    return;

  int reply = QMessageBox::question(0, tr("Aggregation Result"), tr("The operation was concluded successfully. Would you like to add the layer to the project?"), QMessageBox::No, QMessageBox::Yes);

  if(prj && reply == QMessageBox::Yes)
  {
    te::qt::af::evt::LayerAdded evt(layer);

    te::qt::af::ApplicationController::getInstance().broadcast(&evt);
  }
}
