
/*  Copyright (C) 2013-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file LayoutView.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LayoutView.h"
#include "OutsideArea.h"
#include "PropertiesDock.h"
#include "ObjectInspectorDock.h"
#include "ToolbarDock.h"
#include "../../../layout/qt/outside/PropertiesOutside.h"
#include "../../../layout/qt/outside/ObjectInspectorOutside.h"
#include "../../../layout/qt/outside/ToolbarOutside.h"

te::qt::plugins::layout2::LayoutView::LayoutView( QWidget* widget) : 
  te::layout::View(widget),
  m_outsideArea(0)
{

}

te::qt::plugins::layout2::LayoutView::~LayoutView()
{

}

void te::qt::plugins::layout2::LayoutView::config()
{	
  te::layout::View::config();
  connect(m_outsideArea->getToolbarDock()->getToolbarOutside(), SIGNAL(changeContext(bool)), this, SLOT(onToolbarChangeContext(bool)));
  connect(m_outsideArea, SIGNAL(changeMenuContext(bool)), this, SLOT(onMainMenuChangeContext(bool)));
}

void te::qt::plugins::layout2::LayoutView::hideEvent( QHideEvent * event )
{
  QGraphicsView::hideEvent(event);

  if(m_outsideArea)
  {
    m_outsideArea->closeAllDocks();
    m_outsideArea->closeMainMenu();
  }
}

void te::qt::plugins::layout2::LayoutView::closeEvent( QCloseEvent * event )
{
  QGraphicsView::closeEvent(event);

  if(m_outsideArea)
  {
    m_outsideArea->closeAllDocks();
    m_outsideArea->closeMainMenu();
  }
}

void te::qt::plugins::layout2::LayoutView::showEvent( QShowEvent * event )
{
  QGraphicsView::showEvent(event);

  if(m_outsideArea)
  {
    m_outsideArea->openAllDocks();
    m_outsideArea->openMainMenu();
  }
}

void te::qt::plugins::layout2::LayoutView::onSelectionChanged()
{
  QList<QGraphicsItem*> graphicsItems = scene()->selectedItems();
  QList<QGraphicsItem*> allItems = scene()->items();
  //Refresh Property window   
  if(m_outsideArea->getPropertiesDock())
    m_outsideArea->getPropertiesDock()->getPropertiesOutside()->itemsSelected(graphicsItems, allItems);
}

void te::qt::plugins::layout2::LayoutView::onAddItemFinalized()
{
  QList<QGraphicsItem*> graphicsItems = scene()->selectedItems();
  //Refresh Inspector Object window
  if(m_outsideArea->getObjectInspectorDock())
    m_outsideArea->getObjectInspectorDock()->getObjectInspectorOutside()->itemsInspector(graphicsItems);
}

void te::qt::plugins::layout2::LayoutView::setOutsideArea( OutsideArea* outsideArea )
{
  m_outsideArea = outsideArea;
}
