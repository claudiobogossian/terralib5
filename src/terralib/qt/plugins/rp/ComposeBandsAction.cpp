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
  \file terralib/qt/plugins/rp/ComposeBandsAction.cpp

  \brief This file defines the Compose Bands class
*/

// Terralib
#include "../../../qt/widgets/rp/ComposeBandsWizard.h"
#include "../../af/ApplicationController.h"
#include "ComposeBandsAction.h"


// Qt
#include <QtCore/QObject>

// STL
#include <memory>

te::qt::plugins::rp::ComposeBandsAction::ComposeBandsAction(QMenu* menu, QMenu* popupMenu):te::qt::plugins::rp::AbstractAction(menu, popupMenu)
{
  createAction(tr("Compose / Decompose Bands...").toStdString(), "compose-bands");
}

te::qt::plugins::rp::ComposeBandsAction::~ComposeBandsAction()
{
}

void te::qt::plugins::rp::ComposeBandsAction::onActionActivated(bool checked)
{
  te::qt::widgets::ComposeBandsWizard dlg(te::qt::af::ApplicationController::getInstance().getMainWindow());

  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  dlg.setList( layersList );

  if(dlg.exec() == QDialog::Accepted)
  {
    //add new layer
    std::list<te::map::AbstractLayerPtr> layers = dlg.getOutputLayers();
    std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

    while(it != layers.end())
    {
      addNewLayer(*it);
      ++it;
    }
  }
}
