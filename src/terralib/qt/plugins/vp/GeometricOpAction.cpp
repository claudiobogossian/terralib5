/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/plugins/vp/GeometricOpAction.cpp

  \brief This file defines the GeometricOp class
*/

// Terralib
#include "../../../vp/qt/GeometricOpWizard.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/Project.h"
#include "GeometricOpAction.h"

// Qt
#include <QObject>
#include <QMessageBox>

// STL
#include <memory>

te::qt::plugins::vp::GeometricOpAction::GeometricOpAction(QMenu* menu)
  : te::qt::plugins::vp::AbstractAction(menu)
{
  createAction(tr("Geometric Operation...").toStdString(), "vector-processing-geometric-op");
}

te::qt::plugins::vp::GeometricOpAction::~GeometricOpAction()
{
}

void te::qt::plugins::vp::GeometricOpAction::onActionActivated(bool checked)
{
  te::vp::GeometricOpWizard dlg(te::qt::af::ApplicationController::getInstance().getMainWindow());

  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  dlg.setList( layersList );

  if(dlg.exec() != QDialog::Accepted)
    return;

  te::map::AbstractLayerPtr layer = dlg.getOutLayer();

  if(!layer)
    return;

  int reply = QMessageBox::question(0, tr("Geometric Operation"), tr("The operation was concluded successfully. Would you like to add the layer to the project?"), QMessageBox::No, QMessageBox::Yes);
  
  if(reply == QMessageBox::Yes)
  {
    te::qt::af::evt::LayerAdded evt(layer);
    te::qt::af::ApplicationController::getInstance().broadcast(&evt);
  }
}
