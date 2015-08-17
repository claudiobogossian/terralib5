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
  \file BuildContext.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BuildContext.h"
#include "../../../layout/core/pattern/singleton/Context.h"
#include "../../../layout/qt/core/pattern/factory/item/ItemFactory.h"
#include "../../../layout/qt/core/pattern/factory/outside/OutsideFactory.h"
#include "../../../layout/core/template/TemplateFactory.h"
#include "../../../layout/core/Utils.h"
#include "../../../layout/qt/core/Scene.h"
#include "../../../layout/qt/core/BuildGraphicsItem.h"
#include "../../../layout/qt/core/ItemUtils.h"
#include "../../../layout/qt/core/BuildGraphicsOutside.h"

te::layout::BuildContext::BuildContext() :
  m_itemFactory(0),
  m_outsideFactory(0),
  m_templateFactory(0),
  m_utils(0),
  m_canvas(0),
  m_buildGraphicsItem(0),
  m_itemUtils(0),
  m_buildGraphicsOutside(0)
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

  if(m_itemUtils)
  {
    delete m_itemUtils;
    m_itemUtils = 0;
  }

  if(m_buildGraphicsOutside)
  {
    delete m_buildGraphicsOutside;
    m_buildGraphicsOutside = 0;
  }
}

void te::layout::BuildContext::createLayoutContext( int width, int height, View* view )
{
  if(!Context::getInstance().getItemFactory())
  {
    m_itemFactory = new ItemFactory;
    Context::getInstance().setItemFactory(m_itemFactory);
  }

  if(!Context::getInstance().getOutsideFactory())
  {
    m_outsideFactory = new OutsideFactory;
    Context::getInstance().setOutsideFactory(m_outsideFactory);
  }

  if(!Context::getInstance().getTemplateFactory())
  {
    m_templateFactory = new TemplateFactory;
    Context::getInstance().setTemplateFactory(m_templateFactory);
  }

  if(!Context::getInstance().getUtils())
  {
    m_utils = new Utils;
    Context::getInstance().setUtils(m_utils);
  } 
  
  if(!Context::getInstance().getCanvas())
  {
    Scene* lScene = dynamic_cast<Scene*>(view->scene());

    if(lScene)
    {
      te::gm::Envelope worldbox = lScene->getSceneBox();

      //Create Canvas
      m_canvas = new te::qt::widgets::Canvas(width, height);    
      m_canvas->setWindow(worldbox.getLowerLeftX(), worldbox.getLowerLeftY(), 
        worldbox.getUpperRightX(), worldbox.getUpperRightY());
      m_canvas->clear();
      Context::getInstance().setCanvas(m_canvas);
    }
  }

  if(!Context::getInstance().getScene())
  {
    Scene* lScene = dynamic_cast<Scene*>(view->scene());
    if(lScene)
      Context::getInstance().setScene(lScene);
  }  
  if(!Context::getInstance().getAbstractBuildGraphicsItem())
  {
    m_buildGraphicsItem = new BuildGraphicsItem;
    Context::getInstance().setAbstractBuildGraphicsItem(m_buildGraphicsItem);
  }
  if(!Context::getInstance().getSystematicScaleConfig())
  {
    m_systematicConfig = new SystematicScaleConfig;
    Context::getInstance().setSystematicScaleConfig(m_systematicConfig);
  }
  if(!Context::getInstance().getItemUtils())
  {
    Scene* lScene = dynamic_cast<Scene*>(view->scene());
    if(lScene)
    {
      m_itemUtils = new ItemUtils(view->scene());
      Context::getInstance().setItemUtils(m_itemUtils);
    }
  }
  if(!Context::getInstance().getAbstractBuildGraphicsOutside())
  {
    m_buildGraphicsOutside = new BuildGraphicsOutside;
    Context::getInstance().setAbstractBuildGraphicsOutside(m_buildGraphicsOutside);
  }
}




