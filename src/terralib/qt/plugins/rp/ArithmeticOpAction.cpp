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
  \file terralib/qt/plugins/rp/ArithmeticOpAction.cpp

  \brief This file defines the ArithmeticOp class
*/

// Terralib
#include "../../../qt/widgets/rp/ArithmeticOpDialogForm.h"
#include "../../af/ApplicationController.h"
#include "ArithmeticOpAction.h"

// Qt
#include <QtCore/QObject>

// STL
#include <memory>

te::qt::plugins::rp::ArithmeticOpAction::ArithmeticOpAction(QMenu* menu, QMenu* popupMenu):te::qt::plugins::rp::AbstractAction(menu, popupMenu)
{
  m_arithOpDlg = 0;

  createAction(tr("Arithmetic Operations...").toUtf8().data(), "arithmeticOp");
  m_action->setObjectName("Processing.Raster Processing.Arithmetic Operations");
}

te::qt::plugins::rp::ArithmeticOpAction::~ArithmeticOpAction()
{
  if (m_arithOpDlg)
  {
    delete m_arithOpDlg;
    m_arithOpDlg = 0;
  }
}

void te::qt::plugins::rp::ArithmeticOpAction::onActionActivated(bool checked)
{
  if (m_arithOpDlg)
  {
    delete m_arithOpDlg;
    m_arithOpDlg = 0;
  }

  m_arithOpDlg = new te::qt::widgets::ArithmeticOpDialogForm (te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

  connect(m_arithOpDlg, SIGNAL(addLayer(te::map::AbstractLayerPtr)), this, SLOT(addLayer(te::map::AbstractLayerPtr)));
  connect(m_arithOpDlg, SIGNAL(closeTool()), this, SLOT(closeTool()));

  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  m_arithOpDlg->setModal(false);
  m_arithOpDlg->setList(layersList);
  m_arithOpDlg->show();
}

void te::qt::plugins::rp::ArithmeticOpAction::addLayer(te::map::AbstractLayerPtr outputLayer)
{
  //add new layer
  addNewLayer(outputLayer);
}

void te::qt::plugins::rp::ArithmeticOpAction::closeTool()
{
  if (m_arithOpDlg)
  {
    delete m_arithOpDlg;
    m_arithOpDlg = 0;
  }
}

