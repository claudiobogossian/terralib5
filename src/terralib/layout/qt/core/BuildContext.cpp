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
  \file BuildContext.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BuildContext.h"
#include "../../core/pattern/singleton/Context.h"
#include "pattern/factory/ItemFactory.h"
#include "pattern/factory/OutsideFactory.h"
#include "../../core/template/TemplateFactory.h"
#include "../../core/Utils.h"
#include "../../item/PaperConfig.h"
#include "Scene.h"
#include "BuildGraphicsItem.h"


te::layout::BuildContext::BuildContext() :
  m_itemFactory(0),
  m_outsideFactory(0),
  m_templateFactory(0),
  m_utils(0),
  m_paperConfig(0),
  m_canvas(0),
  m_buildGraphicsItem(0)
{

}

te::layout::BuildContext::~BuildContext()
{  
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

void te::layout::BuildContext::createLayoutContext( int width, int height, View* view )
{
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
    Scene* lScene = dynamic_cast<Scene*>(view->scene());

    if(lScene)
    {
      te::gm::Envelope* worldbox = lScene->getWorldBox();

      if(!worldbox)
        worldbox = new te::gm::Envelope;

      //Create Canvas
      m_canvas = new te::qt::widgets::Canvas(width, height);    
      m_canvas->setWindow(worldbox->getLowerLeftX(), worldbox->getLowerLeftY(), 
        worldbox->getUpperRightX(), worldbox->getUpperRightY());
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

