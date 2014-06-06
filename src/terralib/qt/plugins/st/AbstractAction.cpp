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
  \file terralib/qt/plugins/slider/AbstractAction.cpp

  \brief This file defines the abstract class AbstractAction
*/

#include "AbstractAction.h"

// STL
#include <cassert>

te::qt::plugins::st::AbstractAction::AbstractAction(QMenu* menu): m_menu(menu), m_action(0)
{
}

te::qt::plugins::st::AbstractAction::~AbstractAction()
{
}

void te::qt::plugins::st::AbstractAction::createAction(std::string name, std::string pixmap)
{
  assert(m_menu);

  m_action = new QAction(m_menu);

  m_action->setText(name.c_str());

  if(pixmap.empty() == false)
    m_action->setIcon(QIcon::fromTheme(pixmap.c_str()));

  connect(m_action, SIGNAL(triggered(bool)), this, SLOT(onActionActivated(bool)));

  m_menu->addAction(m_action);
}
