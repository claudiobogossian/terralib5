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
\file terralib/qt/plugins/mnt/CreateIsolinesAction.cpp

\brief This file defines the Isolines Generation class
*/

// Terralib
#include "../../../mnt/qt/ProfileDialog.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/ApplicationController.h"
#include "ProfileAction.h"

// Qt
#include <QObject>
#include <QMessageBox>

// STL
#include <memory>

te::qt::plugins::mnt::ProfileAction::ProfileAction(QMenu* menu)
  : te::qt::plugins::mnt::AbstractAction(menu)
{
  createAction(tr("Profile ...").toStdString(), "mnt-processing-profile");
  m_action->setObjectName("Processing.PROFILE Processing.PROFILE");

  QWidget* parent = te::qt::af::AppCtrlSingleton::getInstance().getMainWindow();

  m_profiledlg = new te::mnt::ProfileDialog(parent);

}

te::qt::plugins::mnt::ProfileAction::~ProfileAction()
{
  if (m_profiledlg)
    delete m_profiledlg;
}

void te::qt::plugins::mnt::ProfileAction::onActionActivated(bool actived)
{
 // get the list of layers from current project
  m_profiledlg->setLayers(getLayers());
 // if (actived)
    m_profiledlg->show();

    if (m_profiledlg->exec() != QDialog::Accepted)
    {
    }

    m_profiledlg->release();
    return;
}

