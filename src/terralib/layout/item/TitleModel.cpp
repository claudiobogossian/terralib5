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

te::layout::TitleModel::TitleModel() :
  m_title("Title"),
  m_spacing(1),
  m_padding(1),
  m_columnNumber(1),
  m_rowNumber(2)
{
  m_box = te::gm::Envelope(0., 0., 190., 170.);
}

te::layout::TitleModel::~TitleModel()
{

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

te::layout::Properties* te::layout::TitleModel::getProperties() const
{
  DefaultTextModel::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  Property pro_textGrid;
  pro_textGrid.setName("TextGridSettings");
  pro_textGrid.setId("");
  pro_textGrid.setValue("TextGridSettings", dataType->getDataTypeTextGridSettings());
  pro_textGrid.setMenu(true);
  m_properties->addProperty(pro_textGrid);

  return m_properties;
}

void te::layout::TitleModel::updateProperties( te::layout::Properties* properties )
{
  DefaultTextModel::updateProperties(properties);

  Properties* vectorProps = const_cast<Properties*>(properties);

  Property pro_textGrid = vectorProps->contains("TextGridSettings");

  /*if(!pro_textGrid.isNull())
  {
    m_font = pro_textGrid.getValue().toFont();
  }*/
}
