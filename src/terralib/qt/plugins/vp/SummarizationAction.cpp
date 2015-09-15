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
  \file terralib/qt/plugins/vp/SummarizationAction.cpp

  \brief This file defines the Summarization class
*/

// Terralib
#include "../../../vp/qt/SummarizationDialog.h"
#include "../../af/ApplicationController.h"
#include "SummarizationAction.h"

// Qt
#include <QtCore/QObject>

// STL
#include <memory>

te::qt::plugins::vp::SummarizationAction::SummarizationAction(QMenu* menu)
  : te::qt::plugins::vp::AbstractAction(menu)
{
  createAction(tr("Summarization...").toStdString());
  m_action->setObjectName("Processing.Vector Processing.Sumarization");
}

te::qt::plugins::vp::SummarizationAction::~SummarizationAction()
{
}

void te::qt::plugins::vp::SummarizationAction::onActionActivated(bool)
{
  te::vp::SummarizationDialog dlg(0);

  dlg.setLayers(getLayers());

  dlg.exec();
}
