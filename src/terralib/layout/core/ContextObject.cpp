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
  \file ContextObject.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ContextObject.h"

te::layout::ContextObject::ContextObject( int zoom, int dpiX, int dpiY, PaperConfig* config, EnumType* mode ):
  m_zoom(zoom),
  m_dpiX(dpiX),
  m_dpiY(dpiY),
  m_config(config),
  m_mode(mode)
{
 
}

te::layout::ContextObject::~ContextObject()
{

}

int te::layout::ContextObject::getZoom() const
{
  return m_zoom;
}

int te::layout::ContextObject::getDpiX() const
{
  return m_dpiX;
}

int te::layout::ContextObject::getDpiY() const
{
  return m_dpiY;
}

te::layout::PaperConfig* te::layout::ContextObject::getPaperConfig() const
{
  return m_config;
}

te::layout::EnumType* te::layout::ContextObject::getCurrentMode() const
{
  return m_mode;
}

bool te::layout::ContextObject::isValid()
{
  bool result = true;
  if(m_zoom <= 0 || m_dpiX <= 0 || 
    m_dpiY <= 0 || m_config == 0 || m_mode == 0)
  {
    result = false;
  }
  return result;
}

