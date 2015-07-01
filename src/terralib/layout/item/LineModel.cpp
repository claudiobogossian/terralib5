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
  \file LineModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LineModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../core/enum/Enums.h"
#include "../../geometry/Point.h"
#include "../core/pattern/singleton/Context.h"
#include "../core/Utils.h"
#include "../core/pattern/mvc/ItemObserver.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/enum/EnumLineStyleType.h"

te::layout::LineModel::LineModel() :
  m_currentLineStyleType(0),
  m_enumLineStyleType(0)
{
  m_type = Enums::getInstance().getEnumObjectType()->getLineItem();

  m_box = te::gm::Envelope(0., 0., 20., 20.);

  m_enumLineStyleType = new EnumLineStyleType();
  m_currentLineStyleType = m_enumLineStyleType->getStyleSolid();

  m_border = false;

  m_color = te::color::RGBAColor(0, 0, 0, 255);
}

te::layout::LineModel::~LineModel()
{
  if(!m_coords.empty())
  {
    std::vector<te::gm::Point*>::iterator ito;

    for(ito = m_coords.begin() ; ito != m_coords.end() ; ++ito)
    { 
      if(*ito)
      {
        te::gm::Point* pt = *ito;
        if(pt)
        {
          delete pt;
          pt = 0;
        }
      }
    }

    m_coords.clear();
  }
}

void te::layout::LineModel::setCoords( std::vector<te::gm::Point*> coords )
{
  m_coords = coords;

  int sizeMCoords = m_coords.size();

  te::gm::LinearRing *lineOfPoints = new te::gm::LinearRing(sizeMCoords, te::gm::LineStringType);
  
  for(int i = 0; i < sizeMCoords ; ++i)
  {
    lineOfPoints->setPointN( i, te::gm::Point(m_coords[i]->getX(), m_coords[i]->getY()));    
  }

  const te::gm::Envelope *returnBox = lineOfPoints->getMBR();
  double x1 = 0;
  double y1 = 0;
  double x2 = 0;
  double y2 = 0;
  x1 = returnBox->getLowerLeftX();
  y1 = returnBox->getLowerLeftY();
  x2 = returnBox->getUpperRightX();
  y2 = returnBox->getUpperRightY();

  m_box = te::gm::Envelope(x1, y1, x2, y2);

  if (lineOfPoints)
  {
    delete lineOfPoints;
    lineOfPoints = 0;
  }  

  double x = m_box.getLowerLeftX();
  double y = m_box.getLowerLeftY();

  te::gm::Coord2D coord (x, y);

  ContextItem context;
  context.setPos(coord);
  context.setChangePos(true);

  notifyAll(context);
}

std::vector<te::gm::Point*> te::layout::LineModel::getCoords()
{
  return m_coords;
}

te::layout::Properties* te::layout::LineModel::getProperties() const
{
  ItemModelObservable::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pro_lineStyleType = lineProperty();
  if(!pro_lineStyleType.isNull())
    m_properties->addProperty(pro_lineStyleType);

  Property pro_fillColor(m_hashCode);
  pro_fillColor.setName("color");
  pro_fillColor.setValue(m_color, dataType->getDataTypeColor());
  pro_fillColor.setMenu(true);
  m_properties->addProperty(pro_fillColor);

  return m_properties;
}

void te::layout::LineModel::updateProperties( te::layout::Properties* properties, bool notify )
{
  ItemModelObservable::updateProperties(properties, false);

  Properties* vectorProps = const_cast<Properties*>(properties);

  Property pro_lineType = vectorProps->contains("line_style_type");
  if(!pro_lineType.isNull())
  {
    std::string label = pro_lineType.getOptionByCurrentChoice().toString();
    EnumType* enumType = m_enumLineStyleType->searchLabel(label);
    if(enumType)
      m_currentLineStyleType = enumType;
  }

  {
    Property prop = vectorProps->contains("color");
    if(prop.isNull() == false)
    {
      m_color = prop.getValue().toColor();
    }
  }

  if(notify)
  {
    ContextItem context;
    notifyAll(context);
  }
}

te::layout::EnumLineStyleType* te::layout::LineModel::getEnumLineStyleType()
{
  return m_enumLineStyleType;
}

te::layout::EnumType* te::layout::LineModel::getCurrentLineStyleType()
{
  return m_currentLineStyleType;
}

te::layout::Property te::layout::LineModel::lineProperty() const
{
  Property pro_lineStyleType(m_hashCode);

  if(!m_currentLineStyleType)
    return pro_lineStyleType;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!dataType)
    return pro_lineStyleType;

  pro_lineStyleType.setName("line_style_type");
  pro_lineStyleType.setLabel("line style type");
  pro_lineStyleType.setValue(m_currentLineStyleType->getLabel(), dataType->getDataTypeStringList());

  Variant v;
  v.setValue(m_currentLineStyleType->getLabel(), dataType->getDataTypeString());
  pro_lineStyleType.addOption(v);
  pro_lineStyleType.setOptionChoice(v);

  for(int i = 0; i < m_enumLineStyleType->size(); i++)
  {
    EnumType* enumType = m_enumLineStyleType->getEnum(i);

    if(enumType == m_enumLineStyleType->getStyleNone() || enumType == m_currentLineStyleType || enumType == m_enumLineStyleType->getStyleCustomDash())
      continue;

    Variant v;
    v.setValue(enumType->getLabel(), dataType->getDataTypeString());
    pro_lineStyleType.addOption(v);
  }

  return pro_lineStyleType;
}

const te::color::RGBAColor& te::layout::LineModel::getColor() const
{
  return m_color;
}

void te::layout::LineModel::setColor(const te::color::RGBAColor& color)
{
  m_color = color;
}


