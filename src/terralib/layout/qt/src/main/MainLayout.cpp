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
#include "Scene.h"
#include "View.h"
#include "../../../../color/RGBAColor.h"
#include "../../../../geometry/Envelope.h"
#include "DisplayModel.h"
#include "DisplayController.h"
#include "OutsideObserver.h"
#include "DisplayOutside.h"
#include "Scene.h"
#include "Observer.h"
#include "BuildContext.h"
#include "OutsideArea.h"

// Qt
#include <QGraphicsScene>
#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QString>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QGroupBox>

te::layout::MainLayout::MainLayout() :
  m_view(0),
  m_dockLayoutDisplay(0),
  m_buildContext(0),
  m_outsideArea(0)
{
  m_buildContext = new BuildContext;
}

te::layout::MainLayout::~MainLayout()
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
}

void te::layout::MainLayout::init(QWidget* mainWindow, QMenu* mnuLayout)
{
  bool create = false;

  QSize size(800, 600);
  QRect screen = QApplication::desktop()->screenGeometry();

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
    m_view = new View();
    m_view->setScene(new Scene());
  }
      
  //Resize the dialog and put it in the screen center	
  m_view->move( screen.center() - m_view->rect().center() );

  createLayoutContext(size.width(), size.height());
  createDockLayoutDisplay(mainWindow, m_view);
    
  if(!m_outsideArea)
    m_outsideArea = new OutsideArea(mainWindow, mnuLayout);

  if(create)
  {
    m_view->setOutsideArea(m_outsideArea);
    //Set a new window size
    m_view->config();
  }
  
  m_view->show();
  m_outsideArea->openMainMenu();
  m_outsideArea->openAllDocks();
}

void te::layout::MainLayout::createDockLayoutDisplay(QWidget* mainWindow, View* view)
{
  if(mainWindow)
  {
    QMainWindow* mw = dynamic_cast<QMainWindow*>(mainWindow);
    if(!m_dockLayoutDisplay)
    {
      //Use the Property Browser Framework for create Property Window
      DisplayModel* dockDisplayModel = new DisplayModel();
      DisplayController* dockDisplayController = new DisplayController(dockDisplayModel);
      Observer* itemDockDisplay = (Observer*)dockDisplayController->getView();
      m_dockLayoutDisplay = dynamic_cast<DisplayOutside*>(itemDockDisplay);    

      m_statusBar = new QStatusBar;
      m_statusBar->setMinimumSize(200, 10);
      m_statusBar->showMessage("TerraPrint. Terralib 5 - Ready...");

      QVBoxLayout* vLayout = new QVBoxLayout;
      //view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      vLayout->addWidget(view);
      vLayout->addWidget(m_statusBar);

      m_groupBox = new QGroupBox(m_dockLayoutDisplay);
      //m_groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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

void te::layout::MainLayout::createLayoutContext(int width, int height)
{
  if(!m_buildContext)
    return;

  m_buildContext->createLayoutContext(width, height, m_view);
}

void te::layout::MainLayout::finish()
{
  if(m_dockLayoutDisplay)
  {
    //m_dockLayoutDisplay->removeDock();

    m_dockLayoutDisplay->close();
    delete m_dockLayoutDisplay;
    m_dockLayoutDisplay = 0;
  }
}
