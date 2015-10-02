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
\file src/terraMobilePlugin/CreateLayerAction.cpp

\brief This file defines the Create Layer Action class
*/

// Terralib
#include "../../../qt/af/ApplicationController.h"
#include "../../../qt/af/BaseApplication.h"
#include "../../../qt/widgets/layer/utils/CreateLayerDialog.h"
#include "CreateLayerAction.h"

// Qt
#include <QtCore/QObject>

// STL
#include <memory>

te::qt::plugins::terramobile::CreateLayerAction::CreateLayerAction(QMenu* menu) :te::qt::plugins::terramobile::AbstractAction(menu)
{
  createAction(tr("Create Layer...").toStdString(), "terraMobile");
}

te::qt::plugins::terramobile::CreateLayerAction::~CreateLayerAction()
{
}

void te::qt::plugins::terramobile::CreateLayerAction::onActionActivated(bool checked)
{
  //get display extent
  te::qt::af::BaseApplication* ba = dynamic_cast<te::qt::af::BaseApplication*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

  //show interface
  te::qt::widgets::CreateLayerDialog dlg(ba);

  if(dlg.exec() == QDialog::Accepted)
  {

  }
}
