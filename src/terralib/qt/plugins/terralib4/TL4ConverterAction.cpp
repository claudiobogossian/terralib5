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
  \file terralib/qt/plugins/terralib4/TL4ConverterAction.cpp

  \brief This file defines the Aggregation class
*/

// Terralib
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/Project.h"
#include "TL4ConverterAction.h"
#include "TL4ConverterWizard.h"

// Qt
#include <QtCore/QObject>
#include <QtGui/QMessageBox>

// STL
#include <memory>

te::qt::plugins::terralib4::TL4ConverterAction::TL4ConverterAction(QMenu* menu)
  : te::qt::plugins::terralib4::AbstractAction(menu)
{
  createAction(tr("TerraLib4 Converter...").toStdString());
}

te::qt::plugins::terralib4::TL4ConverterAction::~TL4ConverterAction()
{
}

void te::qt::plugins::terralib4::TL4ConverterAction::onActionActivated(bool checked)
{
  QWidget* parent = te::qt::af::ApplicationController::getInstance().getMainWindow();
  te::qt::plugins::terralib4::TL4ConverterWizard dlg(parent);

  if(dlg.exec() != QDialog::Accepted)
    return;
}
