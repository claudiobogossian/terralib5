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
  \file terralib/qt/plugins/rp/ClassifierAction.cpp

  \brief This file defines the Classifier class
*/

// Terralib
#include "../../../qt/widgets/rp/ClassifierWizard.h"
#include "../../af/ApplicationController.h"
#include "../../af/BaseApplication.h"
#include "ClassifierAction.h"


// Qt
#include <QtCore/QObject>

// STL
#include <memory>

te::qt::plugins::rp::ClassifierAction::ClassifierAction(QMenu* menu, QMenu* popupMenu):te::qt::plugins::rp::AbstractAction(menu, popupMenu)
{
  createAction(tr("Classifier...").toUtf8().data(), "classifier");
  m_action->setObjectName("Processing.Raster Processing.Classifier");
}

te::qt::plugins::rp::ClassifierAction::~ClassifierAction()
{
  if(m_classifierWizard)
     m_classifierWizard->setParent(0);

  delete m_classifierWizard;
}

void te::qt::plugins::rp::ClassifierAction::onActionActivated(bool checked)
{
  m_classifierWizard = new te::qt::widgets::ClassifierWizard(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

  te::qt::af::BaseApplication* ba = dynamic_cast<te::qt::af::BaseApplication*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());
  m_classifierWizard->setMapDisplay(ba->getMapDisplay());

  connect(m_classifierWizard, SIGNAL(addLayer(te::map::AbstractLayerPtr)), this, SLOT(addLayerSlot(te::map::AbstractLayerPtr)));

  QActionGroup* m_mapEditionTools = te::qt::af::AppCtrlSingleton::getInstance().findActionGroup("Map.ToolsGroup");
  assert(m_mapEditionTools);

  m_classifierWizard->setActionGroup(m_mapEditionTools);

  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  m_classifierWizard->setAttribute(Qt::WA_DeleteOnClose);

  m_classifierWizard->setList(layersList);

  m_classifierWizard->setModal(false);

  m_classifierWizard->show();
}

void te::qt::plugins::rp::ClassifierAction::addLayerSlot(te::map::AbstractLayerPtr layer)
{
  addNewLayer(layer);
}
