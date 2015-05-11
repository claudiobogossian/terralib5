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
  \file terralib/qt/plugins/cellspace/CreateCellLayerAction.cpp

  \brief This file defines the CreateCellLayer class
*/

// Terralib
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/Project.h"
#include "CreateCellLayerAction.h"
#include "CreateCellularSpaceDialog.h"

// Qt
#include <QObject>
#include <QMessageBox>

// STL
#include <memory>

te::qt::plugins::cellspace::CreateCellLayerAction::CreateCellLayerAction(QMenu* menu)
  : te::qt::plugins::cellspace::AbstractAction(menu)
{
  createAction(tr("Create Cell Layer...").toStdString(), "cellspace-vector-hint");
}

te::qt::plugins::cellspace::CreateCellLayerAction::~CreateCellLayerAction()
{
}

void te::qt::plugins::cellspace::CreateCellLayerAction::onActionActivated(bool checked)
{
  QWidget* parent = te::qt::af::ApplicationController::getInstance().getMainWindow();
  te::qt::plugins::cellspace::CreateCellularSpaceDialog dlg(parent);

  // get the list of layers from current project
  te::qt::af::Project* prj = te::qt::af::ApplicationController::getInstance().getProject();

  if(prj)
  {
    dlg.setLayers(prj->getSingleLayers(false));
  }

  if(dlg.exec() != QDialog::Accepted)
    return;

  if(QMessageBox::question(parent, tr("Cellular Spaces"), tr("Would you like to add the data in the tree?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
  {
    te::map::AbstractLayerPtr layer = dlg.getLayer();

    if(!layer)
      return;

    if(prj)
    {
      te::qt::af::evt::LayerAdded evt(layer);

      te::qt::af::ApplicationController::getInstance().broadcast(&evt);
    }
  }
}
