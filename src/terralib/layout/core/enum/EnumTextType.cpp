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
  \file EnumObjectType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumTextType.h"

te::layout::EnumTextType::EnumTextType() :
  m_fontItem(0)
{
  init();
}

te::layout::EnumTextType::~EnumTextType()
{
  if(m_fontItem)
  {
    delete m_fontItem;
    m_fontItem = 0;
  } 
}

void te::layout::EnumTextType::init()
{
  m_fontItem = createEnum("Font Item", this);
}

te::layout::EnumType* te::layout::EnumTextType::getFontItem() const
{
  return m_fontItem;
}