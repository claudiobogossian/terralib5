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
  \file terralib/qt/plugins/rp/RasterSlicingAction.cpp

  \brief This file defines the RasterSlicing class
*/

// Terralib
#include "../../../qt/widgets/rp/RasterSlicingWizard.h"
#include "../../../raster/Raster.h"
#include "../../af/ApplicationController.h"
#include "../../af/BaseApplication.h"
#include "../../widgets/canvas/MapDisplay.h"
#include "RasterSlicingAction.h"

// Qt
#include <QObject>
#include <QMessageBox>

// STL
#include <memory>

te::qt::plugins::rp::RasterSlicingAction::RasterSlicingAction(QMenu* menu, QMenu* popupMenu):te::qt::plugins::rp::AbstractAction(menu, popupMenu)
{
  createAction(tr("Raster Slicing...").toStdString(), "rasterslicing");
  m_action->setObjectName("Processing.Raster Processing.RasterSlicing");
}

te::qt::plugins::rp::RasterSlicingAction::~RasterSlicingAction()
{
}

void te::qt::plugins::rp::RasterSlicingAction::onActionActivated(bool checked)
{
  te::qt::widgets::RasterSlicingWizard dlg(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  dlg.setList( layersList );

  //get display extent
  te::qt::af::BaseApplication* ba = dynamic_cast<te::qt::af::BaseApplication*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());
  const te::gm::Envelope extent = ba->getMapDisplay()->getExtent();
  int srid = ba->getMapDisplay()->getSRID();

  dlg.setExtent(extent);
  dlg.setSRID(srid);
  
  if(dlg.exec() == QDialog::Accepted)
  {
    //add new layer
    addNewLayer(dlg.getOutputLayer());
  }
}

void te::qt::plugins::rp::RasterSlicingAction::onPopUpActionActivated(bool checked)
{
  te::map::AbstractLayerPtr layer = getCurrentLayer();

  if(layer.get())
  {
    te::qt::widgets::RasterSlicingWizard dlg(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

    if(dlg.exec() == QDialog::Accepted)
    {
      //add new layer
      addNewLayer(dlg.getOutputLayer());
    }
  }
  else
  {
    QMessageBox::warning(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow(), tr("Warning"), tr("The layer selected is invalid or does not have an raster representation."));
  }
}
