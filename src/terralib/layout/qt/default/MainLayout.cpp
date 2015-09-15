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
 \file terralib/layout/MainLayout.cpp
 
 \brief A singleton  
*/

// TerraLib
#include "MainLayout.h"
#include "../../../layout/qt/core/Scene.h"
#include "../../../color/RGBAColor.h"
#include "../../../geometry/Envelope.h"
#include "OutsideArea.h"
#include "DisplayDock.h"
#include "../../../layout/core/pattern/singleton/Context.h"

// Qt
#include <QGraphicsScene>
#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QString>
#include <QVBoxLayout>

#include "../../../qt/plugins/layout/ProxyProject.h"

te::layout::MainLayout::MainLayout(AbstractProxyProject* proxyProject) :
  m_view(0),
  m_statusBar(0),
  m_buildContext(0),
  m_outsideArea(0),
  m_buildEnums(0),
  m_proxyProject(proxyProject)
{
  m_buildContext = new te::layout::BuildContext;
  m_buildEnums = new te::layout::BuildEnums;
}

te::layout::MainLayout::~MainLayout()
{

  onExit();

  if(m_outsideArea)
  {
    delete m_outsideArea;
    m_outsideArea = 0;
  }

  if(m_view)
  {
    delete m_view;
    m_view = 0;
  }
  
  if(m_statusBar)
  {
    delete m_statusBar;
    m_statusBar = 0;
  }

  if(m_buildContext)
  {
    delete m_buildContext;
    m_buildContext = 0;
  }

  if(m_buildEnums)
  {
    delete m_buildEnums;
    m_buildEnums = 0;
  }

  if(m_proxyProject)
  {
    delete m_proxyProject;
    m_proxyProject = 0;
  }
}

void te::layout::MainLayout::init(const QSize& size, const QRect& screen)
{

  bool create = false;

  if(!m_view)
  {
    create = true;
    m_view = new te::layout::View;
    m_view->setScene(new te::layout::Scene(m_view));
  }
      
  //Resize the dialog and put it in the screen center  
  m_view->move( screen.center() - m_view->rect().center() );

  createEnums();

  createLayoutContext(size.width(), size.height());

  if(create)
  {
    //Calculate matrix and centralizes the scene
    m_view->config();
  }

  m_statusBar = new QStatusBar;
  m_statusBar->setMinimumSize(200, 10);
  m_statusBar->showMessage("Map Layout - TerraLib 5");
}

void te::layout::MainLayout::postInit()
{
  if(!m_outsideArea)
   {
     m_outsideArea = new OutsideArea(m_view, m_statusBar);
 /* TODO: DONE Evento de exit sendo tratado do lado de fora para matar o dock quando mainlayout morrer*/
     m_outsideArea->connect(m_outsideArea, SIGNAL(exit()), this, SLOT(onExit()));
   }

   m_view->show();
   m_outsideArea->openMainMenu();
}

void te::layout::MainLayout::createLayoutContext(int width, int height)
{
  if(!m_buildContext)
    return;

  m_buildContext->createLayoutContext(width, height, m_view);
  //m_proxyProject = proxyProject;
  if(!te::layout::Context::getInstance().getProxyProject())
  {
    te::layout::Context::getInstance().setProxyProject(m_proxyProject);
  }
}

void te::layout::MainLayout::onExit()
{
  emit exit();
}

void te::layout::MainLayout::createEnums()
{
  if(!m_buildEnums)
    return;

  m_buildEnums->build();
}

QStatusBar* te::layout::MainLayout::getStatusBar()
{
  return m_statusBar;
}

te::layout::View* te::layout::MainLayout::getView()
{
  return m_view;
}

te::layout::PropertiesDock* te::layout::MainLayout::getProperties()
{
  return m_outsideArea->getPropertiesDock();
}

te::layout::ObjectInspectorDock* te::layout::MainLayout::getObjectInspector()
{
  return m_outsideArea->getObjectInspectorDock();
}

te::layout::ToolbarOutside* te::layout::MainLayout::getToolbar()
{
  return m_outsideArea->getToolbar();
}

te::layout::EditTemplateDock* te::layout::MainLayout::getEditTemplate()
{
  return m_outsideArea->getEditTemplate();
}

QMenu* te::layout::MainLayout::getMenu(QMenu* parentMenu)
{
  return m_outsideArea->getMenu(parentMenu);
}
