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
  \file terralib/qt/plugins/vp/BasicGeographicOpAction.cpp

  \brief This file defines the BasicGeographicOp class
*/

// Terralib
#include "../../../vp/qt/BasicGeographicOpWizard.h"
#include "../../af/ApplicationController.h"
#include "../../af/Project.h"
#include "BasicGeographicOpAction.h"

// Qt
#include <QtCore/QObject>

// STL
#include <memory>

te::qt::plugins::vp::BasicGeographicOpAction::BasicGeographicOpAction(QMenu* menu)
  : te::qt::plugins::vp::AbstractAction(menu)
{
  createAction(tr("Basic Geographic Operation...").toStdString());
}

te::qt::plugins::vp::BasicGeographicOpAction::~BasicGeographicOpAction()
{
}

void te::qt::plugins::vp::BasicGeographicOpAction::onActionActivated(bool checked)
{
  te::vp::BasicGeographicOpWizard dlg(te::qt::af::ApplicationController::getInstance().getMainWindow());

  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  dlg.setList( layersList );

  if(dlg.exec() == QDialog::Accepted)
  {
    //add new layer
    addNewLayer(dlg.getInLayer());
  }
}
