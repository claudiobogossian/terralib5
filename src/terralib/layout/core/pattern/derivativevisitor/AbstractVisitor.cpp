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
  \file AbstractVisitor.h
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AbstractVisitor.h"
#include "../../../../maptools/Canvas.h"
#include "../../../core/ContextObject.h"
#include "../../Utils.h"
#include "../../AbstractScene.h"
#include "../singleton/Context.h"
#include "Visitable.h"

te::layout::AbstractVisitor::AbstractVisitor() :
  m_visitable(0)
{

}

te::layout::AbstractVisitor::~AbstractVisitor( void )
{

}

void te::layout::AbstractVisitor::visit( Visitable* visitable )
{
  ContextItem context = getContextItem();

  if(m_visitable)
  {
    if(visitable)
    {
      if(m_visitable != visitable)
      {
        m_visitable->unacceptVisitor(this);
        m_visitable = visitable;
      }
    }
    visitDependent(context);
  }
  else
  {
    if(visitable)
    {
      m_visitable = visitable;
      visitDependent(context);
    }
  }
}

void te::layout::AbstractVisitor::disassociate()
{
  m_visitable = 0;
}

te::layout::ContextItem te::layout::AbstractVisitor::getContextItem()
{
  ContextItem context;
  AbstractScene* scene = Context::getInstance().getScene();
  if(!scene)
  {
    return context;
  }

  ContextObject contextObj = scene->getContext();

  te::map::Canvas* canvas = Context::getInstance().getCanvas();
  Utils* utils = Context::getInstance().getUtils();
  int zoom = contextObj.getZoom();
  int dpiX = contextObj.getDpiX();
  int dpiY = contextObj.getDpiY();

  context.setCanvas(canvas);
  context.setUtils(utils);
  context.setZoom(zoom);
  context.setDpiX(dpiX);
  context.setDpiY(dpiY);

  return context;
}

te::layout::Visitable* te::layout::AbstractVisitor::getVisitable()
{
  return m_visitable;
}
