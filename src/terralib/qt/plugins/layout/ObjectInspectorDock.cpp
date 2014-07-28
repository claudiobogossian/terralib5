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
  \file ObjectInspectorOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ObjectInspectorDock.h"
#include "../../../layout/outside/ObjectInspectorModel.h"
#include "../../../layout/outside/ObjectInspectorController.h"
#include "../../../layout/core/pattern/mvc/OutsideObserver.h"
#include "../../../layout/qt/outside/ObjectInspectorOutside.h"


te::qt::plugins::layout::ObjectInspectorDock::ObjectInspectorDock( QWidget * parent /*= 0*/, Qt::WindowFlags flags /*= 0*/ ) :
  QDockWidget(parent, flags)
{
  setWindowTitle("Layout - Object Inspector");
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  create();

  setWidget(m_inspector);
}

te::qt::plugins::layout::ObjectInspectorDock::~ObjectInspectorDock()
{

}

void te::qt::plugins::layout::ObjectInspectorDock::create()
{
  te::layout::ObjectInspectorModel* dockInspectorModel = new te::layout::ObjectInspectorModel();		 
  te::layout::ObjectInspectorController* dockInspectorController = new te::layout::ObjectInspectorController(dockInspectorModel);
  te::layout::OutsideObserver* itemDockInspector = (te::layout::OutsideObserver*)dockInspectorController->getView();
  m_inspector = dynamic_cast<te::layout::ObjectInspectorOutside*>(itemDockInspector);  
}

te::layout::ObjectInspectorOutside* te::qt::plugins::layout::ObjectInspectorDock::getObjectInspectorOutside()
{
  return m_inspector;
}
