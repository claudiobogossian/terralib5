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
  \file TitleModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TitleModel.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../core/enum/Enums.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/property/TextGridSettingsConfigProperties.h"

te::layout::TitleModel::TitleModel() :
  m_textgridProperties(0),
  m_title("Title"),
  m_spacing(1),
  m_padding(1),
  m_width(100),
  m_addRow(false),
  m_rowNumber(2),
  m_addColumn(false),
  m_columnNumber(1),
  m_columnWidth(100),
  m_borderWidth(0.5)

{

  m_type = Enums::getInstance().getEnumObjectType()->getTitleItem();

  m_box = te::gm::Envelope(0., 0., 190., 170.);

  m_tableColor.setColor(0,0,0,255);
  m_borderGridColor.setColor(0,0,0,255);
  m_headerHorizontalColor.setColor(255,255,255,0);
  m_headerVerticalColor.setColor(192,192,192,255);

  m_textgridProperties = new TextGridSettingsConfigProperties;
  m_text = "Text";
}

te::layout::TitleModel::~TitleModel()
{
  if(m_textgridProperties)
  {
    delete m_textgridProperties;
    m_textgridProperties = 0;
  }
}

te::layout::Properties* te::layout::TitleModel::getProperties() const
{
  TextModel::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  Property pro_title(m_hashCode);
  pro_title.setName(m_textgridProperties->getTitle());
  pro_title.setValue(m_title, dataType->getDataTypeString());
  pro_title.addSubProperty(pro_title);
  m_properties->addProperty(pro_title);

  //------------------------------------------------------------------------

  Property pro_textGrid(m_hashCode);  
  pro_textGrid.setName(m_textgridProperties->getName());
  std::string sValuePlanar = "Settings";
  pro_textGrid.setMenu(true);
  pro_textGrid.setValue(sValuePlanar, dataType->getDataTypeTextGridSettings());

  /* Subproperties of DataTypeGridSettings property */

  Property pro_spacing(m_hashCode);
  pro_spacing.setName(m_textgridProperties->getSpacing());
  pro_spacing.setValue(m_spacing, dataType->getDataTypeDouble());
  pro_textGrid.addSubProperty(pro_spacing);

  Property pro_padding(m_hashCode);
  pro_padding.setName(m_textgridProperties->getPadding());
  pro_padding.setValue(m_padding, dataType->getDataTypeDouble());
  pro_textGrid.addSubProperty(pro_padding);
  
  Property pro_tableColor(m_hashCode);
  pro_tableColor.setName(m_textgridProperties->getTableColor());
  pro_tableColor.setValue(m_tableColor, dataType->getDataTypeColor());
  pro_textGrid.addSubProperty(pro_tableColor);

  Property pro_width(m_hashCode);
  pro_width.setName(m_textgridProperties->getWidth());
  pro_width.setValue(m_width, dataType->getDataTypeDouble());
  pro_textGrid.addSubProperty(pro_width);

  Property pro_rowNumber(m_hashCode);
  pro_rowNumber.setName(m_textgridProperties->getRowNumber());
  pro_rowNumber.setValue(m_rowNumber, dataType->getDataTypeInt());
  pro_textGrid.addSubProperty(pro_rowNumber);

  Property pro_evenRow(m_hashCode);
  pro_evenRow.setName(m_textgridProperties->getEvenRow());
  pro_evenRow.setValue(m_evenRow, dataType->getDataTypeColor());
  pro_textGrid.addSubProperty(pro_evenRow);

  Property pro_oddRow(m_hashCode);
  pro_oddRow.setName(m_textgridProperties->getOddRow());
  pro_oddRow.setValue(m_oddRow, dataType->getDataTypeColor());
  pro_textGrid.addSubProperty(pro_oddRow);

  Property pro_columnNumber(m_hashCode);
  pro_columnNumber.setName(m_textgridProperties->getColumnNumber());
  pro_columnNumber.setValue(m_columnNumber, dataType->getDataTypeInt());
  pro_textGrid.addSubProperty(pro_columnNumber);

  Property pro_columnWidth(m_hashCode);
  pro_columnWidth.setName(m_textgridProperties->getColumnWidth());
  pro_columnWidth.setValue(m_columnWidth, dataType->getDataTypeDouble());
  pro_textGrid.addSubProperty(pro_columnWidth);

  Property pro_borderWidth(m_hashCode);
  pro_borderWidth.setName(m_textgridProperties->getBorderWidth());
  pro_borderWidth.setValue(m_borderWidth, dataType->getDataTypeDouble());
  pro_textGrid.addSubProperty(pro_borderWidth);

  Property pro_borderGridColor(m_hashCode);
  pro_borderGridColor.setName(m_textgridProperties->getBorderGridColor());
  pro_borderGridColor.setValue(m_borderGridColor, dataType->getDataTypeColor());
  pro_textGrid.addSubProperty(pro_borderGridColor);
  
  Property pro_headerHorizontalColor(m_hashCode);
  pro_headerHorizontalColor.setName(m_textgridProperties->getHeaderHorizontalColor());
  pro_headerHorizontalColor.setValue(m_headerHorizontalColor, dataType->getDataTypeColor());
  pro_textGrid.addSubProperty(pro_headerHorizontalColor);
  
  Property headerVerticalColor(m_hashCode);
  headerVerticalColor.setName(m_textgridProperties->getHeaderVerticalColor());
  headerVerticalColor.setValue(m_headerVerticalColor, dataType->getDataTypeColor());
  pro_textGrid.addSubProperty(headerVerticalColor);
  
  m_properties->addProperty(pro_textGrid);

  //-------------------------------------------------------------------------

  return m_properties;
}

void te::layout::TitleModel::updateProperties( te::layout::Properties* properties, bool notify )
{
  TextModel::updateProperties(properties, false);

  Properties* vectorProps = const_cast<Properties*>(properties);

  Property pro_textGrid = vectorProps->getProperty(m_textgridProperties->getName());

  Property pro_spacing = pro_textGrid.containsSubProperty(m_textgridProperties->getSpacing());

  if(!pro_spacing.isNull())
  {
   m_spacing = pro_spacing.getValue().toDouble();
  }

  Property pro_padding = pro_textGrid.containsSubProperty(m_textgridProperties->getPadding());

  if(!pro_padding.isNull())
  {
    m_padding = pro_padding.getValue().toDouble();
  }

  Property pro_tableColor = pro_textGrid.containsSubProperty(m_textgridProperties->getTableColor());

  if(!pro_tableColor.isNull())
  {
    m_tableColor = pro_tableColor.getValue().toColor();
  }

  Property pro_width = pro_textGrid.containsSubProperty(m_textgridProperties->getWidth());

  if(!pro_width.isNull())
  {
    m_width = pro_width.getValue().toDouble();
  }

  Property pro_rowNumber = pro_textGrid.containsSubProperty(m_textgridProperties->getRowNumber());

  if(!pro_rowNumber.isNull())
  {
    m_rowNumber = pro_rowNumber.getValue().toInt();
  }

  Property pro_evenRow = pro_textGrid.containsSubProperty(m_textgridProperties->getEvenRow());

  if(!pro_evenRow.isNull())
  {
    m_evenRow = pro_evenRow.getValue().toColor();
  }

  Property pro_oddRow = pro_textGrid.containsSubProperty(m_textgridProperties->getOddRow());

  if(!pro_oddRow.isNull())
  {
    m_oddRow = pro_oddRow.getValue().toColor();
  }

  Property pro_columnNumber = pro_textGrid.containsSubProperty(m_textgridProperties->getColumnNumber());

  if(!pro_columnNumber.isNull())
  {
    m_columnNumber = pro_columnNumber.getValue().toInt();
  }

  Property pro_columnWidth = pro_textGrid.containsSubProperty(m_textgridProperties->getColumnWidth());

  if(!pro_columnWidth.isNull())
  {
    m_columnWidth = pro_columnWidth.getValue().toDouble();
  }

  Property pro_borderWidth = pro_textGrid.containsSubProperty(m_textgridProperties->getBorderWidth());

  if(!pro_borderWidth.isNull())
  {
    m_borderWidth = pro_borderWidth.getValue().toDouble();
  }

  Property pro_borderGridColor = pro_textGrid.containsSubProperty(m_textgridProperties->getBorderGridColor());

  if(!pro_borderGridColor.isNull())
  {
    m_borderGridColor = pro_borderGridColor.getValue().toColor();
  }

  Property pro_headerHorizontalColor = pro_textGrid.containsSubProperty(m_textgridProperties->getHeaderHorizontalColor());

  if(!pro_headerHorizontalColor.isNull())
  {
    m_headerHorizontalColor = pro_headerHorizontalColor.getValue().toColor();
  }

  Property pro_headerVerticalColor = pro_textGrid.containsSubProperty(m_textgridProperties->getHeaderVerticalColor());

  if(!pro_headerVerticalColor.isNull())
  {
    m_headerVerticalColor = pro_headerVerticalColor.getValue().toColor();
  }

  if(notify)
  {
    ContextItem context;
    notifyAll(context);
  }
}

void te::layout::TitleModel::setTitle( std::string title )
{
  m_title = title;
}

std::string te::layout::TitleModel::getTitle()
{
  return m_title;
}

void te::layout::TitleModel::setSpacing( double value )
{
  m_spacing = value;
}

double te::layout::TitleModel::getSpacing()
{
  return m_spacing;
}

void te::layout::TitleModel::setPadding( double value )
{
  m_padding = value;
}

double te::layout::TitleModel::getPadding()
{
  return m_padding;
}

void te::layout::TitleModel::setNumberColumns( int value )
{
  m_columnNumber = value;
}

int te::layout::TitleModel::getNumberColumns()
{
  return m_columnNumber;
}

void te::layout::TitleModel::setNumberRows( int value )
{
  m_rowNumber = value;
}

int te::layout::TitleModel::getNumberRows()
{
  return m_rowNumber;
}

void te::layout::TitleModel::setTableColor( te::color::RGBAColor color )
{
  m_tableColor = color;
}

te::color::RGBAColor te::layout::TitleModel::getTableColor()
{
  return m_tableColor;
}

void te::layout::TitleModel::setWidth(double width)
{
  m_width = width;
}

double te::layout::TitleModel::getWidth()
{
  return m_width;
}

void te::layout::TitleModel::setEvenRow( te::color::RGBAColor color )
{
  m_evenRow = color;
}

te::color::RGBAColor te::layout::TitleModel::getEvenRow()
{
  return m_evenRow;
}

void te::layout::TitleModel::setOddRow( te::color::RGBAColor color )
{
  m_oddRow = color;
}

te::color::RGBAColor te::layout::TitleModel::getOddRow()
{
  return m_oddRow;
}

void te::layout::TitleModel::setColumnWidth( double width )
{
  m_columnWidth = width;
}

double te::layout::TitleModel::getColumnWidth()
{
  return m_columnWidth;
}

void te::layout::TitleModel::setBorderWidth( double width )
{
  m_borderWidth = width;
}

double te::layout::TitleModel::getBorderWidth()
{
  return m_borderWidth;
}

void te::layout::TitleModel::setBorderGridColor( te::color::RGBAColor color )
{
  m_borderGridColor = color;
}

te::color::RGBAColor te::layout::TitleModel::getBorderGridColor()
{
  return m_borderGridColor;
}

void te::layout::TitleModel::setHeaderHorizontalColor( te::color::RGBAColor color )
{
  m_headerHorizontalColor = color;
}

te::color::RGBAColor te::layout::TitleModel::getHeaderHorizontalColor()
{
  return m_headerHorizontalColor;
}

void te::layout::TitleModel::setHeaderVerticalColor( te::color::RGBAColor color )
{
  m_headerVerticalColor = color;
}

te::color::RGBAColor te::layout::TitleModel::getHeaderVerticalColor()
{
  return m_headerVerticalColor;
}
