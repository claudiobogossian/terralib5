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
\file terralib/qt/plugins/mnt/SowValuesAction.cpp

\brief This file defines the Show MNT Values Proccess
*/

// Terralib
#include "../../af/ApplicationController.h"
#include "../../af/BaseApplication.h"
#include "../../af/events/MapEvents.h"
#include "../../af/events/ApplicationEvents.h"
#include "../../widgets/canvas/MapDisplay.h"
#include "../../widgets/Utils.h"

#include "ShowValuesAction.h"

// STL
#include <memory>


te::qt::plugins::mnt::ShowValuesAction::ShowValuesAction(QMenu* menu)
  : te::qt::plugins::mnt::AbstractAction(menu)
{
  createAction(tr("Show Values").toStdString(), "mnt-processing-show values");
  m_action->setObjectName("Processing.DTM Processing.SHOW VALUES");
  m_status = false;
  m_readpixel = 0;
}

te::qt::plugins::mnt::ShowValuesAction::~ShowValuesAction()
{
  if (m_readpixel)
    delete m_readpixel;
}

void te::qt::plugins::mnt::ShowValuesAction::onChecked(bool status)
{
  if (!status)
  {
    QFont f = m_action->font();
    f.setBold(false);
    m_action->setFont(f);
    return;
  }
  QFont f = m_action->font();
  f.setBold(true);
  m_action->setFont(f);

}

void te::qt::plugins::mnt::ShowValuesAction::onActionActivated(bool )
{
  m_status = !m_status;
  m_app = dynamic_cast<te::qt::af::BaseApplication*>(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());
  std::list<te::map::AbstractLayerPtr> layers = te::qt::widgets::GetSelectedLayersOnly(m_app->getLayerExplorer());
  if (!layers.size())
  {
    m_status = false;
  }

  if (!m_status)
  {
    m_action->setChecked(false);
    QFont f = m_action->font();
    f.setBold(false);
    m_action->setFont(f);
    if (m_readpixel)
    {
      m_app->getMapDisplay()->removeEventFilter(m_readpixel);
      delete m_readpixel;
      m_readpixel = 0;
    }
    return;
  }
  m_action->setChecked(true);
  QFont f = m_action->font();
  f.setBold(true);
  m_action->setFont(f);

  m_readpixel = new ShowValuesTool(m_app->getMapDisplay(), m_app);
  m_app->getMapDisplay()->installEventFilter(m_readpixel);

}

