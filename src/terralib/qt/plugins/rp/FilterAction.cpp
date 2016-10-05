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
  \file terralib/qt/plugins/rp/FilterAction.cpp

  \brief This file defines the Filter class
*/

// Terralib
#include "../../../qt/widgets/rp/FilterDialogForm.h"
#include "../../af/ApplicationController.h"
#include "../../af/BaseApplication.h"
#include "FilterAction.h"

// Qt
#include <QtCore/QObject>

// STL
#include <memory>

te::qt::plugins::rp::FilterAction::FilterAction(QMenu* menu, QMenu* popupMenu):te::qt::plugins::rp::AbstractAction(menu, popupMenu)
{
  m_filterDlg = 0;

  createAction(tr("Filter...").toUtf8().data(), "mask");
  m_action->setObjectName("Processing.Raster Processing.Filter");
}

te::qt::plugins::rp::FilterAction::~FilterAction()
{
  if (m_filterDlg)
  {
    delete m_filterDlg;
    m_filterDlg = 0;
  }
}

void te::qt::plugins::rp::FilterAction::onActionActivated(bool checked)
{
  if (m_filterDlg)
  {
    delete m_filterDlg;
    m_filterDlg = 0;
  }

  m_filterDlg = new te::qt::widgets::FilterDialogForm(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

  connect(m_filterDlg, SIGNAL(addLayer(te::map::AbstractLayerPtr)), this, SLOT(addLayer(te::map::AbstractLayerPtr)));
  connect(m_filterDlg, SIGNAL(closeTool()), this, SLOT(closeTool()));

  te::qt::af::BaseApplication* ba = dynamic_cast<te::qt::af::BaseApplication*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  m_filterDlg->setModal(false);
  m_filterDlg->setMapDisplay(ba->getMapDisplay());
  m_filterDlg->setList(layersList);

  m_filterDlg->show();
}

void te::qt::plugins::rp::FilterAction::addLayer(te::map::AbstractLayerPtr outputLayer)
{
  //add new layer
  addNewLayer(outputLayer);
}

void te::qt::plugins::rp::FilterAction::closeTool()
{
  if (m_filterDlg)
  {
    delete m_filterDlg;
    m_filterDlg = 0;
  }
}
