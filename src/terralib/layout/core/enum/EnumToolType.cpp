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
  \file EnumTemplateType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumToolType.h"

te::layout::EnumToolType::EnumToolType() :
  m_noneTool(0),
  m_zoomAreaTool(0),
  m_createLineItemTool(0),
  m_createPolygonItemTool(0)
{
  init();
}

te::layout::EnumToolType::~EnumToolType()
{
  if (m_noneTool)
  {
    delete m_noneTool;
    m_noneTool = 0;
  }
  if (m_zoomAreaTool)
  {
    delete m_zoomAreaTool;
    m_zoomAreaTool = 0;
  }
  if (m_createLineItemTool)
  {
    delete m_createLineItemTool;
    m_createLineItemTool = 0;
  }
  if (m_createLineItemTool)
  {
    delete m_createLineItemTool;
    m_createLineItemTool = 0;
  }
  if (m_createPolygonItemTool)
  {
    delete m_createPolygonItemTool;
    m_createPolygonItemTool = 0;
  }
}

void te::layout::EnumToolType::init()
{
  m_noneTool = createEnum("NoneTool", this);

  m_zoomAreaTool = createEnum("ZoomAreaTool", this);

  m_createLineItemTool = createEnum("CreateLineItemTool", this);

  m_createPolygonItemTool = createEnum("CreatePolygonItemTool", this);
}

te::layout::EnumType* te::layout::EnumToolType::getNoneTool() const
{
  return m_noneTool;
}

te::layout::EnumType* te::layout::EnumToolType::getZoomAreaTool() const
{
  return m_zoomAreaTool;
}

te::layout::EnumType* te::layout::EnumToolType::getCreateLineItemTool() const
{
  return m_createLineItemTool;
}

te::layout::EnumType* te::layout::EnumToolType::getCreatePolygonItemTool() const
{
  return m_createPolygonItemTool;
}




