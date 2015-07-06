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
  \file EllipseModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EllipseModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../core/enum/Enums.h"
#include "../core/property/Properties.h"

// STL
#include <cmath>

te::layout::EllipseModel::EllipseModel() 
{
  m_type = Enums::getInstance().getEnumObjectType()->getEllipseItem();

  m_box = te::gm::Envelope(0., 0., 22., 20.);

  m_border = false;

  m_fillColor = te::color::RGBAColor(255, 255, 255, 255);
  m_contourColor = te::color::RGBAColor(0, 0, 0, 255);
}

te::layout::EllipseModel::~EllipseModel()
{

}

te::layout::Properties* te::layout::EllipseModel::getProperties() const
{
  ItemModelObservable::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pro_fillColor(m_hashCode);
  pro_fillColor.setName("fill_color");
  pro_fillColor.setValue(m_fillColor, dataType->getDataTypeColor());
  pro_fillColor.setMenu(true);
  m_properties->addProperty(pro_fillColor);

  Property pro_bordercolor(m_hashCode);
  pro_bordercolor.setName("contour_color");
  pro_bordercolor.setValue(m_contourColor, dataType->getDataTypeColor());
  pro_bordercolor.setMenu(true);
  m_properties->addProperty(pro_bordercolor);

  return m_properties;
}

void te::layout::EllipseModel::updateProperties( te::layout::Properties* properties, bool notify)
{
  ItemModelObservable::updateProperties(properties);

  Properties* vectorProps = const_cast<Properties*>(properties);

  {
    Property prop = vectorProps->contains("fill_color");
    if(prop.isNull() == false)
    {
      m_fillColor = prop.getValue().toColor();
    }
  }
  {
    Property prop = vectorProps->contains("contour_color");
    if(prop.isNull() == false)
    {
      m_contourColor = prop.getValue().toColor();
    }
  }

  if(notify)
  {
    ContextItem context;
    notifyAll(context);
  }
}

const te::color::RGBAColor& te::layout::EllipseModel::getFillColor() const
{
  return m_fillColor;
}

void te::layout::EllipseModel::setFillColor( const te::color::RGBAColor& color )
{
  m_fillColor = color;
}

const te::color::RGBAColor& te::layout::EllipseModel::getContourColor() const
{
  return m_contourColor;
}

void te::layout::EllipseModel::setContourColor(const te::color::RGBAColor& color)
{
  m_contourColor = color;
}

