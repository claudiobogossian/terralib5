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
  \file TextGridSettingsConfigProperties.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TextGridSettingsConfigProperties.h"


te::layout::TextGridSettingsConfigProperties::TextGridSettingsConfigProperties() :
  m_name("textgridsettings"),
  m_title("textgrid_title"),
  m_spacing("textgrid_spacing"),
  m_padding("textgrid_padding"),
  m_tableColor("textgrid_tablecolor"),
  m_widthConstraints("textgrid_widthconstraints"),
  m_width("textgrid_width"),
  m_addRow("textgrid_addrow"),
  m_rowNumber("textgrid_rownumber"),
  m_evenRow("textgrid_evenrow"),
  m_oddRow("textgrid_oddrow"),
  m_addColumn("textgrid_addcolumn"),
  m_columnNumber("textgrid_columnnumber"),
  m_columnWidth("textgrid_columnwidth"),
  m_columnWidthConstraints("textgrid_columnwidthconstraints"),
  m_borderWidth("textgrid_borderwidth"),
  m_borderGridColor("textgrid_bordergridcolor"),
  m_borderStyle("textgrid_borderstyle"),
  m_headerAlignment("textgrid_headeralignment"), 
  m_headerHorizontalColor("textgrid_headerhorizontalcolor"),
  m_headerVerticalColor("textgrid_headerverticalcolor"),
  m_cellAlignment("textgrid_cellalignment")
{

}

te::layout::TextGridSettingsConfigProperties::~TextGridSettingsConfigProperties()
{

}

std::string te::layout::TextGridSettingsConfigProperties::getName()
{
  return m_name;
}

std::string te::layout::TextGridSettingsConfigProperties::getTitle()
{
  return m_title;
}

std::string te::layout::TextGridSettingsConfigProperties::getSpacing()
{
  return m_spacing;
}

std::string te::layout::TextGridSettingsConfigProperties::getPadding()
{
  return m_padding;
}

std::string te::layout::TextGridSettingsConfigProperties::getTableColor()
{
  return m_tableColor;
}

std::string te::layout::TextGridSettingsConfigProperties::getWidthConstraints()
{
  return m_widthConstraints;
}

std::string te::layout::TextGridSettingsConfigProperties::getWidth()
{
  return m_width;
}

std::string te::layout::TextGridSettingsConfigProperties::getAddRow()
{
  return m_addRow;
}

std::string te::layout::TextGridSettingsConfigProperties::getRowNumber()
{
  return m_rowNumber;
}

std::string te::layout::TextGridSettingsConfigProperties::getEvenRow()
{
  return m_evenRow;
}

std::string te::layout::TextGridSettingsConfigProperties::getOddRow()
{
  return m_oddRow;
}

std::string te::layout::TextGridSettingsConfigProperties::getAddColunm()
{
  return m_addColumn;
}

std::string te::layout::TextGridSettingsConfigProperties::getColumnNumber()
{
  return m_columnNumber;
}

std::string te::layout::TextGridSettingsConfigProperties::getColumnWidth()
{
  return m_columnWidth;
}

std::string te::layout::TextGridSettingsConfigProperties::getColumnWidthConstraints()
{
  return m_columnWidthConstraints;
}

std::string te::layout::TextGridSettingsConfigProperties::getBorderWidth()
{
  return m_borderWidth;
}

std::string te::layout::TextGridSettingsConfigProperties::getBorderGridColor()
{
  return m_borderGridColor;
}

std::string te::layout::TextGridSettingsConfigProperties::getBorderStyle()
{
  return m_borderStyle;
}

std::string te::layout::TextGridSettingsConfigProperties::getHeaderAlignment()
{
  return m_headerAlignment;
}

std::string te::layout::TextGridSettingsConfigProperties::getHeaderHorizontalColor()
{
  return m_headerHorizontalColor;
}

std::string te::layout::TextGridSettingsConfigProperties::getHeaderVerticalColor()
{
  return m_headerVerticalColor;
}

std::string te::layout::TextGridSettingsConfigProperties::getCellAlignment()
{
  return m_cellAlignment;
}
