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
\file terralib/qt/plugins/mnt/VolumeAction.cpp

\brief This file defines the Calculate Volume Proccess
*/

// Terralib
#include "../../../mnt/qt/VolumeDialog.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/ApplicationController.h"
#include "VolumeAction.h"

// Qt
#include <QObject>
#include <QMessageBox>

// STL
#include <memory>

te::qt::plugins::mnt::VolumeAction::VolumeAction(QMenu* menu)
  : te::qt::plugins::mnt::AbstractAction(menu)
{
  createAction(tr("Volume...").toStdString(), "mnt-processing-volume");
  m_action->setObjectName("Processing.DTM Processing.CALCULATE VOLUME");
}

te::qt::plugins::mnt::VolumeAction::~VolumeAction()
{
}

void te::qt::plugins::mnt::VolumeAction::onActionActivated(bool)
{
  QWidget* parent = te::qt::af::AppCtrlSingleton::getInstance().getMainWindow();
  te::mnt::VolumeDialog dlg(parent);

  // get the list of layers from current project
  dlg.setLayers(getLayers());

  if (dlg.exec() != QDialog::Accepted)
    return;

}

