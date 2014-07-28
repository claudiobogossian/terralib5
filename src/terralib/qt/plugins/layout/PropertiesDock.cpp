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
  \file PropertiesDock.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PropertiesDock.h"
#include "../../../layout/outside/PropertiesModel.h"
#include "../../../layout/outside/PropertiesController.h"
#include "../../../layout/core/pattern/mvc/OutsideObserver.h"
#include "../../../layout/qt/outside/PropertiesOutside.h"

te::qt::plugins::layout::PropertiesDock::PropertiesDock( QWidget * parent, Qt::WindowFlags flags ) :
	QDockWidget(parent, flags)
{
  setVisible(false);
  setWindowTitle("Layout - Propriedades");
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	create();

  setWidget(m_properties);
}

te::qt::plugins::layout::PropertiesDock::~PropertiesDock()
{
  
}

void te::qt::plugins::layout::PropertiesDock::create()
{
  //Use the Property Browser Framework for create Property Window
  te::layout::PropertiesModel* dockPropertyModel = new te::layout::PropertiesModel();		 
  te::layout::PropertiesController* dockPropertyController = new te::layout::PropertiesController(dockPropertyModel);
  te::layout::OutsideObserver* itemDockProperty = (te::layout::OutsideObserver*)dockPropertyController->getView();
  m_properties = dynamic_cast<te::layout::PropertiesOutside*>(itemDockProperty);   
}

te::layout::PropertiesOutside* te::qt::plugins::layout::PropertiesDock::getPropertiesOutside()
{
  return m_properties;
}
