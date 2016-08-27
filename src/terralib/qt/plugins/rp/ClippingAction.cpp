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
  \file terralib/qt/plugins/rp/ClippingAction.cpp

  \brief This file defines the Clipping class
*/

// Terralib
#include "../../../qt/widgets/rp/ClippingWizard.h"
#include "../../../raster/Raster.h"
#include "../../af/ApplicationController.h"
#include "../../af/BaseApplication.h"
#include "ClippingAction.h"

// Qt
#include <QObject>
#include <QMessageBox>

// STL
#include <memory>

te::qt::plugins::rp::ClippingAction::ClippingAction(QMenu* menu, QMenu* popupMenu):te::qt::plugins::rp::AbstractAction(menu, popupMenu)
{
  createAction(tr("Clipping...").toUtf8().data(), "edit-cut");
  m_action->setObjectName("Processing.Raster Processing.Clipping");

  createPopUpAction(tr("Clipping...").toUtf8().data(), "clipping");
}

te::qt::plugins::rp::ClippingAction::~ClippingAction()
{
}

void te::qt::plugins::rp::ClippingAction::onActionActivated(bool checked)
{
  m_clippingWizard = new te::qt::widgets::ClippingWizard(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

  te::qt::af::BaseApplication* ba = dynamic_cast<te::qt::af::BaseApplication*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());
  m_clippingWizard->setMapDisplay(ba->getMapDisplay());

  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  m_clippingWizard->setList(layersList);

  m_clippingWizard->setModal(false);

  m_clippingWizard->show();

  connect(m_clippingWizard, SIGNAL(addLayer(te::map::AbstractLayerPtr)), this, SLOT(addLayerSlot(te::map::AbstractLayerPtr)));
  connect(m_clippingWizard, SIGNAL(closeTool()), this, SLOT(closeTool()));
}

void te::qt::plugins::rp::ClippingAction::addLayerSlot(te::map::AbstractLayerPtr layer)
{
  addNewLayer(layer);
}

void te::qt::plugins::rp::ClippingAction::onPopUpActionActivated(bool checked)
{
  te::map::AbstractLayerPtr layer = getCurrentLayer();

  if(layer.get())
  {
    te::qt::widgets::ClippingWizard dlg(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

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
    QMessageBox::warning(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow(), tr("Warning"), tr("The layer selected is invalid or does not have an raster representation."));
  }
}

void te::qt::plugins::rp::ClippingAction::closeTool()
{
  if (m_clippingWizard)
  {
    delete m_clippingWizard;
    m_clippingWizard = 0;
  }
}
