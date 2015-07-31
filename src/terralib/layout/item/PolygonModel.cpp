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
  \file PolygonModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PolygonModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../core/enum/Enums.h"
#include "../../geometry/Point.h"
#include "../core/pattern/singleton/Context.h"
#include "../core/Utils.h"
#include "../core/pattern/mvc/ItemObserver.h"
#include "../core/property/Properties.h"

te::layout::PolygonModel::PolygonModel()
{
  m_type = Enums::getInstance().getEnumObjectType()->getPolygonItem();

  m_fillColor = te::color::RGBAColor(255, 255, 255, 255);
}

te::layout::PolygonModel::~PolygonModel()
{

}

te::layout::Properties* te::layout::PolygonModel::getProperties() const
{
  LineModel::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pro_fillColor(m_hashCode);
  pro_fillColor.setName("fill_color");
  pro_fillColor.setValue(m_fillColor, dataType->getDataTypeColor());
  pro_fillColor.setMenu(true);
  m_properties->addProperty(pro_fillColor);

  return m_properties;
}

void te::layout::PolygonModel::updateProperties( te::layout::Properties* properties, bool notify )
{
  LineModel::updateProperties(properties, false);

  Properties* vectorProps = const_cast<Properties*>(properties);

  {
    Property prop = vectorProps->getProperty("fill_color");
    if(prop.isNull() == false)
    {
      m_fillColor = prop.getValue().toColor();
    }
  }

  if(notify)
  {
    ContextItem context;
    notifyAll(context);
  }
}

const te::color::RGBAColor& te::layout::PolygonModel::getFillColor() const
{
  return m_fillColor;
}

void te::layout::PolygonModel::setFillColor( const te::color::RGBAColor& color )
{
  m_fillColor = color;
}

