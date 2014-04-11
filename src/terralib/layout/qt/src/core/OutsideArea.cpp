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
  \file OutsideArea.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "OutsideArea.h"
#include "PropertiesOutside.h"
#include "PropertiesModel.h"
#include "PropertiesController.h"
#include "ObjectInspectorModel.h"
#include "ObjectInspectorController.h"
#include "ObjectInspectorOutside.h"
#include "ToolbarModel.h"
#include "ToolbarController.h"
#include "ToolbarOutside.h"

// Qt
#include <QMainWindow>

te::layout::OutsideArea::OutsideArea( QWidget* dockParent ) :
  m_dockParent(dockParent)
{
  init();
}

te::layout::OutsideArea::~OutsideArea()
{
  QMainWindow* win = (QMainWindow*)m_dockParent;

  if(!win)
    return;

  if(m_dockProperties)
  {
    win->removeDockWidget(m_dockProperties);
    m_dockProperties->close();
    m_dockProperties->setParent(0);
    delete m_dockProperties;
  }

  if(m_dockInspector)
  {
    win->removeDockWidget(m_dockInspector);
    m_dockInspector->close();
    m_dockInspector->setParent(0);
    delete m_dockInspector;
  }

  if(m_dockToolbar)
  {
    win->removeDockWidget(m_dockToolbar);
    m_dockToolbar->close();
    m_dockToolbar->setParent(0);
    delete m_dockToolbar;
  }
}

void te::layout::OutsideArea::init()
{
  //Use the Property Browser Framework for create Property Window
  PropertiesModel* dockPropertyModel = new PropertiesModel();		 
  PropertiesController* dockPropertyController = new PropertiesController(dockPropertyModel);
  OutsideObserver* itemDockProperty = (OutsideObserver*)dockPropertyController->getView();
  m_dockProperties = dynamic_cast<PropertiesOutside*>(itemDockProperty);  
  m_dockProperties->setVisible(true);  

  //Use the Property Browser Framework for create Object Inspector Window
  ObjectInspectorModel* dockInspectorModel = new ObjectInspectorModel();		 
  ObjectInspectorController* dockInspectorController = new ObjectInspectorController(dockInspectorModel);
  OutsideObserver* itemDockInspector = (OutsideObserver*)dockInspectorController->getView();
  m_dockInspector = dynamic_cast<ObjectInspectorOutside*>(itemDockInspector);  
  m_dockInspector->setVisible(true);
  
  //Use the Property Browser Framework for create Object Inspector Window
  ToolbarModel* dockToolbarModel = new ToolbarModel();		 
  ToolbarController* dockToolbarController = new ToolbarController(dockToolbarModel);
  OutsideObserver* itemDockToolbar = (OutsideObserver*)dockToolbarController->getView();
  m_dockToolbar = dynamic_cast<ToolbarOutside*>(itemDockToolbar); 
  m_dockToolbar->setVisible(true);
  
  openAllDocks();
}

te::layout::PropertiesOutside* te::layout::OutsideArea::getPropertiesOutside()
{
  return m_dockProperties;
}

te::layout::ObjectInspectorOutside* te::layout::OutsideArea::getObjectInspectorOutside()
{
  return m_dockInspector;
}

te::layout::ToolbarOutside* te::layout::OutsideArea::getToolbarOutside()
{
  return m_dockToolbar;
}

void te::layout::OutsideArea::openAllDocks()
{
  QMainWindow* win = (QMainWindow*)m_dockParent;

  if(!win)
    return;

  if(m_dockProperties)
  {
    m_dockProperties->setParent(m_dockParent);
    win->addDockWidget(Qt::LeftDockWidgetArea, m_dockProperties);
    m_dockProperties->setVisible(true);
  }
  if(m_dockInspector)
  {
    m_dockInspector->setParent(m_dockParent);
    win->addDockWidget(Qt::LeftDockWidgetArea, m_dockInspector);
    m_dockInspector->setVisible(true);
  }
  if(m_dockToolbar)
  {
    m_dockToolbar->setParent(m_dockParent); 
    win->addDockWidget(Qt::TopDockWidgetArea, m_dockToolbar);
    m_dockToolbar->setVisible(true);
  }
}

void te::layout::OutsideArea::closeAllDocks()
{
  QMainWindow* win = (QMainWindow*)m_dockParent;

  if(!win)
    return;

  if(m_dockProperties)
  {
    win->removeDockWidget(m_dockProperties);
    m_dockProperties->close();
  }
  if(m_dockInspector)
  {
    win->removeDockWidget(m_dockInspector);
    m_dockInspector->close();
  }
  if(m_dockToolbar)
  {
    win->removeDockWidget(m_dockToolbar);
    m_dockToolbar->close();
  }
}
