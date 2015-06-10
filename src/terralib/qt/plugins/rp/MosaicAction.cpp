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
  \file terralib/qt/plugins/rp/MosaicAction.cpp

  \brief This file defines the Mosaic class
*/

// Terralib
#include "../../../qt/widgets/rp/MosaicWizard.h"
#include "../../af/ApplicationController.h"
#include "MosaicAction.h"


// Qt
#include <QtCore/QObject>

// STL
#include <memory>

te::qt::plugins::rp::MosaicAction::MosaicAction(QMenu* menu, QMenu* popupMenu):te::qt::plugins::rp::AbstractAction(menu, popupMenu)
{
  createAction(tr("Mosaic...").toStdString(), "mosaic");
  m_action->setObjectName("Processing.Raster Processing.Mosaic");
}

te::qt::plugins::rp::MosaicAction::~MosaicAction()
{
}

void te::qt::plugins::rp::MosaicAction::onActionActivated(bool checked)
{
  te::qt::widgets::MosaicWizard dlg(te::qt::af::ApplicationController::getInstance().getMainWindow());

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
