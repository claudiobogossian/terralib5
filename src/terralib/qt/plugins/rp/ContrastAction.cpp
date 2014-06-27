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
  \file terralib/qt/plugins/rp/ContrastAction.cpp

  \brief This file defines the Contrast class
*/

// Terralib
#include "../../../qt/widgets/rp/ContrastWizard.h"
#include "../../af/ApplicationController.h"
#include "ContrastAction.h"

// Qt
#include <QObject>
#include <QMessageBox>

// STL
#include <memory>

te::qt::plugins::rp::ContrastAction::ContrastAction(QMenu* menu, QMenu* popupMenu):te::qt::plugins::rp::AbstractAction(menu, popupMenu)
{
  createAction(tr("Contrast...").toStdString(), "histogram");

  createPopUpAction(tr("Contrast...").toStdString(), "histogram");
}

te::qt::plugins::rp::ContrastAction::~ContrastAction()
{
}

void te::qt::plugins::rp::ContrastAction::onActionActivated(bool checked)
{
  te::qt::widgets::ContrastWizard dlg(te::qt::af::ApplicationController::getInstance().getMainWindow());
  
  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  dlg.setList( layersList );

  if(dlg.exec() == QDialog::Accepted)
  {
    //add new layer
    addNewLayer(dlg.getOutputLayer());
  }
}

void te::qt::plugins::rp::ContrastAction::onPopUpActionActivated(bool checked)
{
  te::map::AbstractLayerPtr layer = getCurrentLayer();

  if(layer.get())
  {
    te::qt::widgets::ContrastWizard dlg(te::qt::af::ApplicationController::getInstance().getMainWindow());

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
