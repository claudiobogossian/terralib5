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
  \file terralib/qt/plugins/terralib4/TL4ConverterAction.cpp

  \brief This file defines the Aggregation class
*/

// Terralib
#include "../../../af/ApplicationController.h"
#include "../../../af/events/LayerEvents.h"
#include "../../../af/Project.h"
#include "TL4ConverterAction.h"
#include "TL4ConverterWizard.h"

// Qt
#include <QObject>
#include <QMessageBox>

// STL
#include <cassert>
#include <memory>

te::qt::plugins::terralib4::TL4ConverterAction::TL4ConverterAction(QMenu* menu)
  : m_menu(menu), m_action(0)
{
  createAction(tr("TerraLib4 Converter...").toStdString());
}

te::qt::plugins::terralib4::TL4ConverterAction::~TL4ConverterAction()
{
}

void te::qt::plugins::terralib4::TL4ConverterAction::createAction(const std::string& name,
                                                                  const std::string& pixmap)
{
  assert(m_menu);

  m_action = new QAction(m_menu);

  m_action->setText(name.c_str());

  if(!pixmap.empty())
    m_action->setIcon(QIcon::fromTheme(pixmap.c_str()));

  connect(m_action, SIGNAL(triggered(bool)), this, SLOT(onActionActivated(bool)));

  m_menu->addAction(m_action);
}

void te::qt::plugins::terralib4::TL4ConverterAction::onActionActivated(bool checked)
{
  QWidget* parent = te::qt::af::ApplicationController::getInstance().getMainWindow();
  te::qt::plugins::terralib4::TL4ConverterWizard dlg(parent);

  if(dlg.exec() != QDialog::Accepted)
    return;
}
