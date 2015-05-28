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
  \file terralib/qt/plugins/sa/BayesLocalAction.cpp

  \brief This file defines the concrete class for BayesLocal Action
*/

// Terralib
#include "../../../qt/af/ApplicationController.h"
#include "../../../sa/qt/BayesLocalDialog.h"
#include "BayesLocalAction.h"

te::qt::plugins::sa::BayesLocalAction::BayesLocalAction(QMenu *menu) :
  AbstractAction(menu)
{
  createAction(tr("Local Empirical Bayes...").toStdString(), "sa-bayeslocal-icon");
  m_action->setObjectName("Processing.Spatial Analysis.Local Empirical Bayes");
}

te::qt::plugins::sa::BayesLocalAction::~BayesLocalAction()
{
}

void te::qt::plugins::sa::BayesLocalAction::onActionActivated(bool checked)
{
  te::sa::BayesLocalDialog dlg(te::qt::af::ApplicationController::getInstance().getMainWindow());

  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  dlg.setLayers(layersList);

  if(dlg.exec() == QDialog::Accepted)
  {
    //add new layer
    addNewLayer(dlg.getOutputLayer());
  }
}
