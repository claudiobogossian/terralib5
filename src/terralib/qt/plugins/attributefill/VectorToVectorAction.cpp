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
  \file terralib/qt/plugins/attributefill/VectorToVectorAction.cpp

  \brief This file defines the RasterToVector class
*/

// Terralib
#include "../../../attributefill/qt/VectorToVectorDialog.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/Project.h"
#include "VectorToVectorAction.h"

// Qt
#include <QObject>
#include <QMessageBox>

// STL
#include <memory>

te::qt::plugins::attributefill::VectorToVectorAction::VectorToVectorAction(QMenu* menu)
  : te::qt::plugins::attributefill::AbstractAction(menu)
{
  createAction(tr("Vector to Vector...").toStdString(), "attributefill-vector2vector-icon");
  m_action->setObjectName("Processing.Attribute Fill.Vector to Vector");
}

te::qt::plugins::attributefill::VectorToVectorAction::~VectorToVectorAction()
{
}

void te::qt::plugins::attributefill::VectorToVectorAction::onActionActivated(bool checked)
{
  QWidget* parent = te::qt::af::AppCtrlSingleton::getInstance().getMainWindow();
  te::attributefill::VectorToVectorDialog dlg(parent);

  // get the list of layers from current project
  te::qt::af::Project* prj = te::qt::af::AppCtrlSingleton::getInstance().getProject();

  if(prj)
  {
    std::list<te::map::AbstractLayerPtr> layers = prj->getSingleLayers(false);

    if(layers.size() < 2)
    {
      QMessageBox::warning(0, tr("Vector To Vector"), tr("It is necessary at least two layers to perform the operations!"));
      return;
    }
    dlg.setLayers(prj->getSingleLayers(false));
  }

  QString logPath = te::qt::af::AppCtrlSingleton::getInstance().getUserDataDir();
  logPath += "/log/terralib_attributefill.log";

  dlg.setLogPath(logPath.toStdString());

  if(dlg.exec() != QDialog::Accepted)
    return;

  te::map::AbstractLayerPtr layer = dlg.getLayer();

  if(!layer)
    return;

  int reply = QMessageBox::question(0, tr("Attribute Fill Result"), tr("The operation was concluded successfully. Would you like to add the layer to the project?"), QMessageBox::No, QMessageBox::Yes);

  if(prj && reply == QMessageBox::Yes)
  {
    te::qt::af::evt::LayerAdded evt(layer);

    te::qt::af::AppCtrlSingleton::getInstance().broadcast(&evt);
  }
}
