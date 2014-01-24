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
#include "QLayoutView.h"
#include "QLayoutScene.h"
#include "QLayoutItemFactory.h"
#include "LayoutContext.h"
#include "LayoutUtils.h"

#include "../../../../qt/widgets/canvas/Canvas.h"
#include "../../../../color/RGBAColor.h"
#include "../../../../geometry/Envelope.h"

#include <QGraphicsScene>
#include <QApplication>
#include <QDesktopWidget>

te::layout::MainLayout::MainLayout()
{

}

te::layout::MainLayout::~MainLayout()
{

}

void te::layout::MainLayout::init()
{
	QLayoutView* w = new QLayoutView();
  {
	  QLayoutScene* scene = new QLayoutScene();
	  w->setScene((QGraphicsScene*)scene);
	  w->setGeometry(0,0,1920,1080);
		
	  int width = QApplication::desktop()->screenGeometry().width();
	  int height = QApplication::desktop()->screenGeometry().height();
    
    //Resize the dialog and put it in the screen center	
	  const QRect screen = QApplication::desktop()->screenGeometry();
    w->move( screen.center() - w->rect().center() );
		
	  QLayoutItemFactory* itemFactory = new QLayoutItemFactory;
    LayoutUtils* utils = new LayoutUtils; 
    te::gm::Envelope* worldbox = w->getWorldBox();

    //Create Canvas
    //MUDAR PARA TE::MAP::CANVAS* - Abstração
    te::qt::widgets::Canvas* canvas = new te::qt::widgets::Canvas(1920, 1080);    
    //canvas->calcAspectRatio(m_extent.m_llx, m_extent.m_lly, m_extent.m_urx, m_extent.m_ury, m_hAlign, m_vAlign);
    canvas->setWindow(worldbox->getLowerLeftX(), worldbox->getLowerLeftY(), worldbox->getUpperRightX(), worldbox->getUpperRightY());
    te::color::RGBAColor color(255,255,255, 255);
    canvas->setBackgroundColor(color);
    canvas->clear();

    LayoutContext::getInstance()->setCanvas(canvas);
	  LayoutContext::getInstance()->setScene((LayoutScene*)scene);
	  LayoutContext::getInstance()->setItemFactory(itemFactory);
    LayoutContext::getInstance()->setUtils(utils);

    //Set a new window size
    w->config();
  }
  w->show();
}
