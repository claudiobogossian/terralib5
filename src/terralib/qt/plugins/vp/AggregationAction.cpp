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
  \file terralib/qt/plugins/vp/AggregationAction.cpp

  \brief This file defines the Aggregation class
*/

// Terralib
#include "../../../vp/qt/AggregationDialog.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "AggregationAction.h"

// Qt
#include <QObject>
#include <QMessageBox>

// STL
#include <memory>

te::qt::plugins::vp::AggregationAction::AggregationAction(QMenu* menu)
  : te::qt::plugins::vp::AbstractAction(menu)
{
  createAction(tr("Aggregation...").toStdString(), "vector-processing-aggregation");
  m_action->setObjectName("Processing.Vector Processing.Aggregation");
}

te::qt::plugins::vp::AggregationAction::~AggregationAction()
{
}

void te::qt::plugins::vp::AggregationAction::onActionActivated(bool)
{
  QWidget* parent = te::qt::af::AppCtrlSingleton::getInstance().getMainWindow();
  te::vp::AggregationDialog dlg(parent);

  dlg.setLayers(getLayers());

  if(dlg.exec() != QDialog::Accepted)
    return;

  te::map::AbstractLayerPtr layer = dlg.getLayer();

  if(!layer)
    return;


  int reply;

  std::vector<std::string> warnings = dlg.getWarnings();

  if (!warnings.empty())
  {
    std::string details;
    for (std::size_t w = 0; w < warnings.size(); ++w)
      details += warnings[w] + "\n";

    QMessageBox question(QMessageBox::Warning, tr("Aggregation Result"), tr("The operation was concluded successfully. But it has warning(s). Would you like to add the layer to the project?"));
    question.addButton(QMessageBox::No);
    question.addButton(QMessageBox::Yes);
    question.setDetailedText(QString(details.c_str()));

    reply = question.exec();
  }
  else
  {
    reply = QMessageBox::question(0, tr("Aggregation Result"), tr("The operation was concluded successfully. Would you like to add the layer to the project?"), QMessageBox::No, QMessageBox::Yes);
  }

  if(reply == QMessageBox::Yes)
    addNewLayer(layer);
}
