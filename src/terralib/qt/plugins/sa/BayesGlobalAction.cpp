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
  \file terralib/qt/plugins/sa/BayesGlobalAction.cpp

  \brief This file defines the concrete class for BayesGlobal Action
*/

// Terralib
#include "../../../qt/af/ApplicationController.h"
#include "../../../sa/qt/BayesGlobalDialog.h"
#include "BayesGlobalAction.h"

te::qt::plugins::sa::BayesGlobalAction::BayesGlobalAction(QMenu *menu) :
  AbstractAction(menu)
{
  createAction(tr("Global Empirical Bayes...").toStdString(), "sa-bayesglobal-icon");
}

te::qt::plugins::sa::BayesGlobalAction::~BayesGlobalAction()
{
}

void te::qt::plugins::sa::BayesGlobalAction::onActionActivated(bool checked)
{
  te::sa::BayesGlobalDialog dlg(te::qt::af::ApplicationController::getInstance().getMainWindow());

  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  dlg.setLayers(layersList);

  dlg.exec();
}
