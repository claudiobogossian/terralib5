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
  \file terralib/qt/plugins/rp/MixtureModelAction.cpp

  \brief This file defines the mixture model class
*/

// Terralib
#include "../../../qt/widgets/rp/MixtureModelWizard.h"
#include "../../af/ApplicationController.h"
#include "../../af/BaseApplication.h"
#include "MixtureModelAction.h"

// Qt
#include <QObject>
#include <QMessageBox>

// STL
#include <memory>

te::qt::plugins::rp::MixtureModelAction::MixtureModelAction(QMenu* menu, QMenu* popupMenu):te::qt::plugins::rp::AbstractAction(menu, popupMenu)
{
  createAction(tr("Mixture Model...").toUtf8().data(), "mixmodel");
  m_action->setObjectName("Processing.Raster Processing.Mixture Model");

  //createPopUpAction(tr("Mixture Model...").toUtf8().data(), "mixmodel");
}

te::qt::plugins::rp::MixtureModelAction::~MixtureModelAction()
{
  if(m_mixtureModelWizard)
     m_mixtureModelWizard->setParent(0);

  delete m_mixtureModelWizard;
}

void te::qt::plugins::rp::MixtureModelAction::onActionActivated(bool checked)
{
  m_mixtureModelWizard = new te::qt::widgets::MixtureModelWizard(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

  te::qt::af::BaseApplication* ba = dynamic_cast<te::qt::af::BaseApplication*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());
  m_mixtureModelWizard->setMapDisplay(ba->getMapDisplay());

  connect(m_mixtureModelWizard, SIGNAL(addLayer(te::map::AbstractLayerPtr)), this, SLOT(addLayerSlot(te::map::AbstractLayerPtr)));

  QActionGroup* m_mapEditionTools = te::qt::af::AppCtrlSingleton::getInstance().findActionGroup("Map.ToolsGroup");
  assert(m_mapEditionTools);
  
  m_mixtureModelWizard->setActionGroup(m_mapEditionTools);

  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  m_mixtureModelWizard->setAttribute(Qt::WA_DeleteOnClose);

  m_mixtureModelWizard->setList(layersList);

  m_mixtureModelWizard->setModal(false);

  m_mixtureModelWizard->show();
}

void te::qt::plugins::rp::MixtureModelAction::addLayerSlot(te::map::AbstractLayerPtr layer)
{
  addNewLayer(layer);
}

void te::qt::plugins::rp::MixtureModelAction::onPopUpActionActivated(bool checked)
{
  std::list<te::map::AbstractLayerPtr> layer;
  layer.push_back(getCurrentLayer());

  if (layer.size())
  {
    te::qt::widgets::MixtureModelWizard dlg(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

    dlg.setLayer(layer);

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
