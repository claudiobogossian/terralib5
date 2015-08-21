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
  \file PropertiesDock.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PropertiesDock.h"
#include "../../../layout/core/pattern/singleton/Context.h"
#include "../../../layout/qt/core/BuildGraphicsOutside.h"
#include "../../../layout/core/enum/Enums.h"
#include "../../../layout/qt/outside/PropertiesOutside.h"

te::qt::plugins::layout::PropertiesDock::PropertiesDock( QWidget * parent, Qt::WindowFlags flags ) :
  QDockWidget(parent, flags)
{
  setVisible(false);
  setWindowTitle("Properties");
  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

  create();

  setWidget(m_properties);
}

te::qt::plugins::layout::PropertiesDock::~PropertiesDock()
{
  
}

void te::qt::plugins::layout::PropertiesDock::create()
{
  te::layout::AbstractBuildGraphicsOutside* abstractBuildOutside = te::layout::Context::getInstance().getAbstractBuildGraphicsOutside();
  if(!abstractBuildOutside)
  {
    return;
  }

  te::layout::BuildGraphicsOutside* buildOutside = dynamic_cast<te::layout::BuildGraphicsOutside*>(abstractBuildOutside);
  if(!buildOutside)
  {
    return;
  }

  te::layout::EnumObjectType* objectType = te::layout::Enums::getInstance().getEnumObjectType();
  if(!objectType)
  {
    return;
  }

  QWidget* widget = buildOutside->createOuside(objectType->getPropertiesWindow());
  if(!widget)
  {
    return;
  }
  m_properties = dynamic_cast<te::layout::PropertiesOutside*>(widget);   
}

te::layout::PropertiesOutside* te::qt::plugins::layout::PropertiesDock::getPropertiesOutside()
{
  return m_properties;
}

void te::qt::plugins::layout::PropertiesDock::closeEvent( QCloseEvent * event )
{
  // Closing the PropertiesDock, all open windows from a property will be closed.
  m_properties->close();
}






