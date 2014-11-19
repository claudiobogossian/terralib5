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
  \file TextGridModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TextGridModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../core/enum/Enums.h"

te::layout::TextGridModel::TextGridModel() 
{

  m_type = Enums::getInstance().getEnumObjectType()->getTextGridItem();

  m_spacing = 1;
  m_padding = 1;
  m_columnNumber = 5;
  m_rowNumber = 4;

  m_box = te::gm::Envelope(0., 0., 190., 170.);

  m_tableColor.setColor(0,0,0,255);
  m_borderGridColor.setColor(0,0,0,255);
  m_headerHorizontalColor.setColor(192,192,192,255);
  m_headerVerticalColor.setColor(192,192,192,255);
}

te::layout::TextGridModel::~TextGridModel()
{
  clearTextCells();
}

void te::layout::TextGridModel::addText( te::gm::Point* cell, std::string text )
{
  m_textCells[cell] = text;
}

void te::layout::TextGridModel::removeCell( te::gm::Point* cell )
{
  if(!cell)
    return;

  m_textCells.erase(cell);

  delete cell;
  cell = 0;
}

std::map<te::gm::Point*, std::string> te::layout::TextGridModel::getTextCells()
{
  return m_textCells;
}

void te::layout::TextGridModel::clearTextCells()
{
  std::map<te::gm::Point*, std::string>::iterator it;
  for (it = m_textCells.begin(); it != m_textCells.end(); ++it) 
  {
    te::gm::Point* pt = it->first;
    if(pt)
    {
      delete pt;
      pt = 0;
    }
  }

  m_textCells.clear();
}
