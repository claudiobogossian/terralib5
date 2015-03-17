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
  \file PointModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PointModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../core/enum/Enums.h"

te::layout::PointModel::PointModel():
  m_enumPointType(0),
  m_currentPointType(0),
  m_shapeSize(4)
{
  m_type = Enums::getInstance().getEnumObjectType()->getPointItem();

  m_borderColor = te::color::RGBAColor(0, 0, 255, 255);
  m_backgroundColor = te::color::RGBAColor(0, 255, 0, 100);

  m_box = te::gm::Envelope(0., 0., 10., 10.);

  m_enumPointType = new EnumPointType();
  m_currentPointType = m_enumPointType->getCircleType();
}

te::layout::PointModel::~PointModel()
{
  if(m_enumPointType)
  {
    delete m_enumPointType;
    m_enumPointType = 0;
  }
}

te::layout::Properties* te::layout::PointModel::getProperties() const
{
  ItemModelObservable::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pro_pointName = pointProperty();
  if(!pro_pointName.isNull())
  {
    m_properties->addProperty(pro_pointName);
  }

  return m_properties;
}

void te::layout::PointModel::updateProperties( te::layout::Properties* properties )
{
  ItemModelObservable::updateProperties(properties);

  Properties* vectorProps = const_cast<Properties*>(properties);

  Property pro_pointName = vectorProps->contains("point_type");

  if(!pro_pointName.isNull())
  {
    std::string label = pro_pointName.getOptionByCurrentChoice().toString();
    EnumType* enumType = m_enumPointType->searchLabel(label);
    if(enumType)
    {
      m_currentPointType = enumType;
    }
  }
}

te::layout::EnumPointType* te::layout::PointModel::getEnumPointType()
{
  return m_enumPointType;
}

te::layout::EnumType* te::layout::PointModel::getCurrentPointType()
{
  return m_currentPointType;
}

te::layout::Property te::layout::PointModel::pointProperty() const
{
  Property pro_pointName;

  if(!m_currentPointType)
    return pro_pointName;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!dataType)
    return pro_pointName;

  pro_pointName.setName("point_type");
  pro_pointName.setLabel("graphic type");
  pro_pointName.setId("");
  pro_pointName.setValue(m_currentPointType->getLabel(), dataType->getDataTypeStringList());

  Variant v;
  v.setValue(m_currentPointType->getLabel(), dataType->getDataTypeString());
  pro_pointName.addOption(v);
  pro_pointName.setOptionChoice(v);

  for(int i = 0 ; i < m_enumPointType->size() ; ++i)
  {
    EnumType* enumType = m_enumPointType->getEnum(i);

    if(enumType == m_enumPointType->getNoneType() || enumType == m_currentPointType)
      continue;

    Variant v;
    v.setValue(enumType->getLabel(), dataType->getDataTypeString());
    pro_pointName.addOption(v);
  }

  return pro_pointName;
}

double te::layout::PointModel::getShapeSize()
{
  return m_shapeSize;
}


