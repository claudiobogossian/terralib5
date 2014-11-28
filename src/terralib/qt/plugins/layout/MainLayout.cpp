/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.
 
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
#include "ProxyProject.h"
#include "../../../layout/core/pattern/singleton/Context.h"

// Qt
#include <QGraphicsScene>
#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QString>
#include <QVBoxLayout>

te::qt::plugins::layout::MainLayout::MainLayout() :
  m_view(0),
  m_dockLayoutDisplay(0),
  m_buildContext(0),
  m_outsideArea(0),
  m_buildEnums(0),
  m_proxyProject(0)
{
  m_buildContext = new te::layout::BuildContext;
  m_buildEnums = new te::layout::BuildEnums;
}

te::qt::plugins::layout::MainLayout::~MainLayout()
{
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
  
  finish();  

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

void te::qt::plugins::layout::MainLayout::init(QWidget* mainWindow, QMenu* mnuLayout)
{
  bool create = false;

  QSize size(800, 600);
  QRect screen = QApplication::desktop()->screen()->rect();

  if(mainWindow)
  {
    QMainWindow* mw = dynamic_cast<QMainWindow*>(mainWindow);
    if(mw)
    {
      size = mw->centralWidget()->size();
      screen = mw->centralWidget()->geometry();
    }
  }

  if(!m_view)
  {
    create = true;
    m_view = new te::layout::View;
    m_view->setScene(new te::layout::Scene());
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

  createDockLayoutDisplay(mainWindow, m_view);
    
  if(!m_outsideArea)
  {
    m_outsideArea = new OutsideArea(m_view, mainWindow, mnuLayout);
    m_outsideArea->connect(m_outsideArea, SIGNAL(exit()), m_dockLayoutDisplay, SLOT(onExit()));
  }

  m_view->show();
  m_outsideArea->openMainMenu();
  m_outsideArea->openAllDocks();
}

void te::qt::plugins::layout::MainLayout::createDockLayoutDisplay(QWidget* mainWindow, te::layout::View* view)
{
  if(mainWindow)
  {
    QMainWindow* mw = dynamic_cast<QMainWindow*>(mainWindow);
    if(!m_dockLayoutDisplay)
    {
      m_dockLayoutDisplay = new DisplayDock;    

      m_statusBar = new QStatusBar;
      m_statusBar->setMinimumSize(200, 10);
      m_statusBar->showMessage("TerraPrint. Terralib 5 - Ready...");

      QVBoxLayout* vLayout = new QVBoxLayout;
      vLayout->addWidget(view);
      vLayout->addWidget(m_statusBar);

      m_groupBox = new QGroupBox(m_dockLayoutDisplay);
      m_groupBox->setLayout(vLayout);
    }   

    m_dockLayoutDisplay->setWidget(m_groupBox);
    m_dockLayoutDisplay->setPreviousCentralWidget(mw->centralWidget());
    mw->removeDockWidget((QDockWidget*)mw->centralWidget());
    m_dockLayoutDisplay->setParent(mw); 

    mw->setCentralWidget(m_dockLayoutDisplay);
    m_dockLayoutDisplay->setVisible(true);      
  }
}

void te::qt::plugins::layout::MainLayout::createLayoutContext(int width, int height)
{
  if(!m_buildContext)
    return;

  m_buildContext->createLayoutContext(width, height, m_view);
  m_proxyProject = new ProxyProject;
  if(!te::layout::Context::getInstance().getProxyProject())
  {
    te::layout::Context::getInstance().setProxyProject(m_proxyProject);
  }
}

void te::qt::plugins::layout::MainLayout::finish()
{
  if(m_dockLayoutDisplay)
  {
    m_dockLayoutDisplay->close();
    delete m_dockLayoutDisplay;
    m_dockLayoutDisplay = 0;
  }
}

void te::qt::plugins::layout::MainLayout::createEnums()
{
  if(!m_buildEnums)
    return;

  m_buildEnums->build();
}
