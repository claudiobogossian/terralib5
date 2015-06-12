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
#include "MixtureModelAction.h"

// Qt
#include <QObject>
#include <QMessageBox>

// STL
#include <memory>

te::qt::plugins::rp::MixtureModelAction::MixtureModelAction(QMenu* menu, QMenu* popupMenu):te::qt::plugins::rp::AbstractAction(menu, popupMenu)
{
  createAction(tr("Mixture Model...").toStdString(), "mixmodel");
  m_action->setObjectName("Processing.Raster Processing.Mixture Model");

  //createPopUpAction(tr("Mixture Model...").toStdString(), "mixmodel");
}

te::qt::plugins::rp::MixtureModelAction::~MixtureModelAction()
{
}

void te::qt::plugins::rp::MixtureModelAction::onActionActivated(bool checked)
{
  te::qt::widgets::MixtureModelWizard dlg(te::qt::af::ApplicationController::getInstance().getMainWindow());

  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  dlg.setList( layersList );

  if(dlg.exec() == QDialog::Accepted)
  {
    //add new layer
    addNewLayer(dlg.getOutputLayer());
  }
}

void te::qt::plugins::rp::MixtureModelAction::onPopUpActionActivated(bool checked)
{
  te::map::AbstractLayerPtr layer = getCurrentLayer();

  if(layer.get())
  {
    te::qt::widgets::MixtureModelWizard dlg(te::qt::af::ApplicationController::getInstance().getMainWindow());

    dlg.setLayer(layer);

    if(dlg.exec() == QDialog::Accepted)
    {
      //add new layer
      addNewLayer(dlg.getOutputLayer());
    }
  }
  else
  {
    QMessageBox::warning(te::qt::af::ApplicationController::getInstance().getMainWindow(), tr("Warning"), tr("The layer selected is invalid or does not have an raster representation."));
  }
}
