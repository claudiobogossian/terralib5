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
  \file terralib/qt/plugins/vp/IntersectionAction.cpp

  \brief This file defines the Intersection class
*/

// Terralib
#include "../../../vp/qt/IntersectionDialog.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/Project.h"
#include "IntersectionAction.h"

// Qt
#include <QtCore/QObject>

// STL
#include <cassert>
#include <memory>

te::qt::plugins::vp::IntersectionAction::IntersectionAction(QMenu* menu)
  : te::qt::plugins::vp::AbstractAction(menu)
{
  createAction(tr("Intersection...").toStdString());
}

te::qt::plugins::vp::IntersectionAction::~IntersectionAction()
{
}

void te::qt::plugins::vp::IntersectionAction::onActionActivated(bool checked)
{
  te::vp::IntersectionDialog dlg(0);

  // get the list of layers from current project
  te::qt::af::Project* prj = te::qt::af::ApplicationController::getInstance().getProject();

  if(prj)
  {
    dlg.setLayers(prj->getLayers());
  }

  dlg.exec();

  te::map::AbstractLayerPtr layer = dlg.getLayer();

  assert(layer);

  if(prj)
  {
    prj->add(layer);

    te::qt::af::evt::LayerAdded evt(layer.get());

    te::qt::af::ApplicationController::getInstance().broadcast(&evt);
  }
}
