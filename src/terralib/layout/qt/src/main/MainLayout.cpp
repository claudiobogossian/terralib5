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
#include "ItemFactory.h"
#include "OutsideFactory.h"
#include "Context.h"
#include "Utils.h"
#include "View.h"
#include "../../../../color/RGBAColor.h"
#include "../../../../geometry/Envelope.h"
#include "DisplayModel.h"
#include "DisplayController.h"
#include "OutsideObserver.h"
#include "DisplayOutside.h"
#include "Scene.h"
#include "Observer.h"
#include "TemplateFactory.h"
#include "OutsideArea.h"
#include "PaperConfig.h"

// Qt
#include <QGraphicsScene>
#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QString>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QGroupBox>
#include "BuildGraphicsItem.h"

te::layout::MainLayout::MainLayout() :
  m_view(0),
  m_dockLayoutDisplay(0),
  m_outsideArea(0),
  m_itemFactory(0),
  m_outsideFactory(0),
  m_templateFactory(0),
  m_utils(0),
  m_paperConfig(0),
  m_canvas(0),
  m_buildGraphicsItem(0)
{

}

te::layout::MainLayout::~MainLayout()
{
  finish();  

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

  if(m_itemFactory)
  {
    delete m_itemFactory;
    m_itemFactory = 0;
  }

  if(m_outsideFactory)
  {
    delete m_outsideFactory;
    m_outsideFactory = 0;
  }

  if(m_templateFactory)
  {
    delete m_templateFactory;
    m_templateFactory = 0;
  }

  if(m_utils)
  {
    delete m_utils;
    m_utils = 0;
  }

  if(m_paperConfig)
  {
    delete m_paperConfig;
    m_paperConfig = 0;
  }

  if(m_buildGraphicsItem)
  {
    delete m_buildGraphicsItem;
    m_buildGraphicsItem = 0;
  }

  if(m_canvas)
  {
    delete m_canvas;
    m_canvas = 0;
  }
}

void te::layout::MainLayout::init(QWidget* mainWindow)
{
  bool create = false;

  QSize size(800, 600);

  if(mainWindow)
  {
    QMainWindow* mw = dynamic_cast<QMainWindow*>(mainWindow);
    size = mw->centralWidget()->size();
  }

  if(!m_view)
  {
    create = true;
    m_view = new View();
    m_view->setScene(new Scene());
  }
    
  //Resize the dialog and put it in the screen center	
  const QRect screen = QApplication::desktop()->screenGeometry();
  m_view->move( screen.center() - m_view->rect().center() );

  createLayoutContext(size.width(), size.height(), m_view);
  createDockLayoutDisplay(mainWindow, m_view);
  
  if(!m_outsideArea)
    m_outsideArea = new OutsideArea(mainWindow);

  if(create)
  {
    m_view->setOutsideArea(m_outsideArea);
    //Set a new window size
    m_view->config();
  }
  
  m_view->show();

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

      QStatusBar* status = new QStatusBar;
      status->setMinimumSize(200, 10);
      status->showMessage("TerraPrint. Terralib 5 - Ready...");

      QVBoxLayout* vLayout = new QVBoxLayout;
      vLayout->addWidget(view);
      vLayout->addWidget(status);

      m_groupBox = new QGroupBox(m_dockLayoutDisplay);
      m_groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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

void te::layout::MainLayout::createLayoutContext( int width, int height, View* view )
{
  ItemFactory* f = dynamic_cast<ItemFactory*>(Context::getInstance()->getItemFactory());

  if(!Context::getInstance()->getItemFactory())
  {
    m_itemFactory = new ItemFactory;
    Context::getInstance()->setItemFactory(m_itemFactory);
  }

  if(!Context::getInstance()->getOutsideFactory())
  {
    m_outsideFactory = new OutsideFactory;
    Context::getInstance()->setOutsideFactory(m_outsideFactory);
  }

  if(!Context::getInstance()->getTemplateFactory())
  {
    m_templateFactory = new TemplateFactory;
    Context::getInstance()->setTemplateFactory(m_templateFactory);
  }

  if(!Context::getInstance()->getUtils())
  {
    m_utils = new Utils;
    Context::getInstance()->setUtils(m_utils);
  } 

  if(!Context::getInstance()->getPaperConfig())
  {
    m_paperConfig = new PaperConfig;
    Context::getInstance()->setPaperConfig(m_paperConfig);
  } 

  if(!Context::getInstance()->getCanvas())
  {
    Scene* lScene = dynamic_cast<Scene*>(m_view->scene());

    if(lScene)
    {
      te::gm::Envelope* worldbox = lScene->getWorldBox();

      if(!worldbox)
        worldbox = new te::gm::Envelope;

      //Create Canvas
      m_canvas = new te::qt::widgets::Canvas(width, height);    
      m_canvas->setWindow(worldbox->getLowerLeftX(), worldbox->getLowerLeftY(), worldbox->getUpperRightX(), worldbox->getUpperRightY());
      m_canvas->clear();
      Context::getInstance()->setCanvas(m_canvas);
    }
  }

  if(!Context::getInstance()->getScene())
  {
    Scene* lScene = dynamic_cast<Scene*>(view->scene());
    if(lScene)
      Context::getInstance()->setScene(lScene);
  }  
  if(!Context::getInstance()->getBuildGraphicsItem())
  {
    m_buildGraphicsItem = new BuildGraphicsItem;
    Context::getInstance()->setBuildGraphicsItem(m_buildGraphicsItem);
  }
}

void te::layout::MainLayout::finish()
{
  if(m_dockLayoutDisplay)
  {
    m_dockLayoutDisplay->close();
    delete m_dockLayoutDisplay;
    m_dockLayoutDisplay = 0;
  }
}
