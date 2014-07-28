/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file ToolbarDock.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ToolbarDock.h"
#include "../../../layout/qt/outside/ToolbarOutside.h"
#include "../../../layout/outside/ToolbarModel.h"
#include "../../../layout/outside/ToolbarController.h"
#include "../../../layout/core/pattern/mvc/OutsideObserver.h"

// Qt
#include <QWidget>

te::qt::plugins::layout::ToolbarDock::ToolbarDock( QWidget * parent, Qt::WindowFlags flags ) :
  QDockWidget(parent, flags)
{
  create();

  setWidget(m_toolbar);
}

te::qt::plugins::layout::ToolbarDock::~ToolbarDock()
{

}

void te::qt::plugins::layout::ToolbarDock::create()
{
  //Use the Property Browser Framework for create Object Inspector Window
  te::layout::ToolbarModel* dockToolbarModel = new te::layout::ToolbarModel();		 
  te::layout::ToolbarController* dockToolbarController = new te::layout::ToolbarController(dockToolbarModel);
  te::layout::OutsideObserver* itemDockToolbar = (te::layout::OutsideObserver*)dockToolbarController->getView();
  m_toolbar = dynamic_cast<te::layout::ToolbarOutside*>(itemDockToolbar); 
}

te::layout::ToolbarOutside* te::qt::plugins::layout::ToolbarDock::getToolbarOutside()
{
  return m_toolbar;
}
