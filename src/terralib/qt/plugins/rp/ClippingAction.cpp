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
  \file terralib/qt/plugins/rp/ClippingAction.cpp

  \brief This file defines the Clipping class
*/

// Terralib
#include "../../../qt/widgets/rp/ClippingWizard.h"
#include "../../../raster/Raster.h"
#include "../../af/ApplicationController.h"
#include "ClippingAction.h"

// Qt
#include <QObject>
#include <QMessageBox>

// STL
#include <memory>

te::qt::plugins::rp::ClippingAction::ClippingAction(QMenu* menu, QMenu* popupMenu):te::qt::plugins::rp::AbstractAction(menu, popupMenu)
{
  createAction(tr("Clipping...").toStdString(), "edit-cut");

  createPopUpAction(tr("Clipping...").toStdString(), "clipping");
}

te::qt::plugins::rp::ClippingAction::~ClippingAction()
{
}

void te::qt::plugins::rp::ClippingAction::onActionActivated(bool checked)
{
  te::qt::widgets::ClippingWizard dlg(te::qt::af::ApplicationController::getInstance().getMainWindow());

  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  dlg.setList( layersList );

  if(dlg.exec() == QDialog::Accepted)
  {
    //add layers
    std::vector<te::map::AbstractLayerPtr> layersVec = dlg.getOutputLayers();
    for(std::size_t i = 0; i < layersVec.size(); ++i)
      addNewLayer(layersVec[i]);
  }
}

void te::qt::plugins::rp::ClippingAction::onPopUpActionActivated(bool checked)
{
  te::map::AbstractLayerPtr layer = getCurrentLayer();

  if(layer.get())
  {
    te::qt::widgets::ClippingWizard dlg(te::qt::af::ApplicationController::getInstance().getMainWindow());

    dlg.setLayer(layer);

    if(dlg.exec() == QDialog::Accepted)
    {
      //add layers
      std::vector<te::map::AbstractLayerPtr> layersVec = dlg.getOutputLayers();
      for(std::size_t i = 0; i < layersVec.size(); ++i)
        addNewLayer(layersVec[i]);
    }
  }
  else
  {
    QMessageBox::warning(te::qt::af::ApplicationController::getInstance().getMainWindow(), tr("Warning"), tr("The layer selected is invalid or does not have an raster representation."));
  }
}
