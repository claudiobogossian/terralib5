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
  \file terralib/qt/plugins/rp/ContrastAction.cpp

  \brief This file defines the Contrast class
*/

// Terralib
#include "../../../qt/widgets/rp/ContrastWizard.h"
#include "../../af/ApplicationController.h"
#include "../../af/Project.h"
#include "ContrastAction.h"

// Qt
#include <QtCore/QObject>

// STL
#include <memory>

te::qt::plugins::rp::ContrastAction::ContrastAction(QMenu* menu):te::qt::plugins::rp::AbstractAction(menu)
{
  createAction(tr("Contrast...").toStdString());
}

te::qt::plugins::rp::ContrastAction::~ContrastAction()
{
}

void te::qt::plugins::rp::ContrastAction::onActionActivated(bool checked)
{
  te::qt::widgets::ContrastWizard dlg(0);

// get the list of layers from current project
  te::qt::af::Project* prj = te::qt::af::ApplicationController::getInstance().getProject();

  if(prj)
  {
    dlg.setList(prj->getLayers());
  }

  if(dlg.exec() == QDialog::Accepted)
  {
    //add new layer
    addNewLayer(dlg.getOutputLayer());
  }
}
