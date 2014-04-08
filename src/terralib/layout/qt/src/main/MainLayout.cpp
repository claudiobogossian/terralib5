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
#include "QLayoutScene.h"
#include "QLayoutItemFactory.h"
#include "QLayoutOutsideFactory.h"
#include "LayoutContext.h"
#include "LayoutUtils.h"
#include "QLayoutView.h"
#include "../../../../qt/widgets/canvas/Canvas.h"
#include "../../../../color/RGBAColor.h"
#include "../../../../geometry/Envelope.h"
#include "DisplayWindowLayoutModel.h"
#include "DisplayWindowLayoutController.h"
#include "LayoutOutsideObserver.h"
#include "QDisplayWindowOutside.h"
#include "LayoutScene.h"
#include "LayoutObserver.h"
#include "LayoutTemplateFactory.h"

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
  m_dockLayoutDisplay(0)
{

}

te::layout::MainLayout::~MainLayout()
{
  finish();  
}

void te::layout::MainLayout::init(QWidget* mainWindow)
{
  m_view = new QLayoutView();

  m_view->setScene(new QLayoutScene());
    
  m_view->setGeometry(0,0,1920,1080);
  m_view->setDockPropertiesParent(mainWindow);

  //Resize the dialog and put it in the screen center	
  const QRect screen = QApplication::desktop()->screenGeometry();
  m_view->move( screen.center() - m_view->rect().center() );

  createLayoutContext(1920, 1080, m_view);
  createDockLayoutDisplay(mainWindow, m_view);

  //Set a new window size
  m_view->config();
  m_view->show();

}

void te::layout::MainLayout::createDockLayoutDisplay(QWidget* mainWindow, QLayoutView* view)
{
  if(mainWindow)
  {
    QMainWindow* mw = (QMainWindow*)mainWindow;
    if(!m_dockLayoutDisplay)
    {
      //Use the Property Browser Framework for create Property Window
      DisplayWindowLayoutModel* dockDisplayModel = new DisplayWindowLayoutModel();
      DisplayWindowLayoutController* dockDisplayController = new DisplayWindowLayoutController(dockDisplayModel);
      LayoutObserver* itemDockDisplay = (LayoutObserver*)dockDisplayController->getView();
      m_dockLayoutDisplay = dynamic_cast<QDisplayWindowOutside*>(itemDockDisplay);    
    }
    m_dockLayoutDisplay->setPreviousCentralWidget(mw->centralWidget());
    mw->removeDockWidget((QDockWidget*)mw->centralWidget());
    m_dockLayoutDisplay->setParent(mw);    

    QStatusBar* status = new QStatusBar;
    status->setMinimumSize(200, 10);
    status->showMessage("TerraPrint 5.0 - Ready...");

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addWidget(view);
    vLayout->addWidget(status);
    
    QGroupBox* groupBox = new QGroupBox;
    groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    groupBox->setLayout(vLayout);

    m_dockLayoutDisplay->setWidget(groupBox);
    mw->setCentralWidget(m_dockLayoutDisplay);
    m_dockLayoutDisplay->setVisible(true);      
  }
}

void te::layout::MainLayout::createLayoutContext( int width, int height, QLayoutView* view )
{
  QLayoutItemFactory* f = dynamic_cast<QLayoutItemFactory*>(LayoutContext::getInstance()->getItemFactory());

  if(!LayoutContext::getInstance()->getItemFactory())
  {
    LayoutContext::getInstance()->setItemFactory(new QLayoutItemFactory);
  }

  if(!LayoutContext::getInstance()->getOutsideFactory())
  {
    LayoutContext::getInstance()->setOutsideFactory(new QLayoutOutsideFactory);
  }

  if(!LayoutContext::getInstance()->getTemplateFactory())
  {
    LayoutContext::getInstance()->setTemplateFactory(new LayoutTemplateFactory);
  }

  if(!LayoutContext::getInstance()->getUtils())
  {
    LayoutContext::getInstance()->setUtils(new LayoutUtils);
  } 

  if(!LayoutContext::getInstance()->getCanvas())
  {
    QLayoutScene* lScene = dynamic_cast<QLayoutScene*>(m_view->scene());

    if(lScene)
    {
      te::gm::Envelope worldbox = lScene->getWorldBox();
      //Create Canvas
      te::qt::widgets::Canvas* canvas = new te::qt::widgets::Canvas(width, height);    
      //canvas->calcAspectRatio(m_extent.m_llx, m_extent.m_lly, m_extent.m_urx, m_extent.m_ury, m_hAlign, m_vAlign);
      canvas->setWindow(worldbox.getLowerLeftX(), worldbox.getLowerLeftY(), worldbox.getUpperRightX(), worldbox.getUpperRightY());
      te::color::RGBAColor color(255,255,255, 255);
      canvas->setBackgroundColor(color);
      canvas->clear();
      LayoutContext::getInstance()->setCanvas(canvas);
    }
  }

  if(!LayoutContext::getInstance()->getScene())
  {
    LayoutScene* lScene = dynamic_cast<LayoutScene*>(view->scene());
    if(lScene)
      LayoutContext::getInstance()->setScene(lScene);
  }  
}

void te::layout::MainLayout::finish()
{
  if(m_dockLayoutDisplay)
  {
    m_dockLayoutDisplay->close();
  }

  /*if(_view)
  {
  delete _view;
  _view = 0;
  }*/

  if(m_dockLayoutDisplay)
  {
    delete m_dockLayoutDisplay;
    m_dockLayoutDisplay = 0;
  }

  if(LayoutContext::getInstance())
  {
    delete LayoutContext::getInstance();
  }
}
