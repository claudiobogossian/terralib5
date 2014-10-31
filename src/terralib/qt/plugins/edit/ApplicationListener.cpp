/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*! 
  \file terralib/qt/plugins/ApplicationListener.cpp

  \brief A listener of application framework. It is used to listen events sent by the application framework.
*/

// TerraLib
#include "../../../edit/SnapManager.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/events/MapEvents.h"
#include "ApplicationListener.h"

te::qt::plugins::edit::ApplicationListener::ApplicationListener()
{
}

te::qt::plugins::edit::ApplicationListener::~ApplicationListener()
{
}

void te::qt::plugins::edit::ApplicationListener::onApplicationTriggered(te::qt::af::evt::Event* e)
{
  switch(e->m_id)
  {
    case te::qt::af::evt::DRAWING_FINISHED:
    {
      te::qt::af::evt::DrawingFinished* df = static_cast<te::qt::af::evt::DrawingFinished*>(e);

      te::qt::widgets::MapDisplay* display = df->m_display->getDisplay();

      const te::gm::Envelope& env = display->getExtent();

      te::edit::SnapManager::getInstance().setWorld(env.m_llx, env.m_lly, env.m_urx, env.m_ury, display->getWidth(), display->getHeight());
    }
    break;

    default:
      return;
  }
}
