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
  \file ScaleModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ScaleModel.h"
#include "MapModel.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/property/SharedProperties.h"
#include "../../geometry/Polygon.h"
#include "../core/enum/Enums.h"

// STL
#include <string>
#include <sstream> // std::stringstream


te::layout::ScaleModel::ScaleModel() :
  m_mapName(""),
  m_mapScale(0),
  m_scaleGapX(20),
  m_scaleGapY(5),
  m_enumScaleType(0),
  m_currentScaleType(0)
{

  m_type = Enums::getInstance().getEnumObjectType()->getScaleItem();

  m_box = te::gm::Envelope(0., 0., 70., 30.);

  m_border = false;

  m_enumScaleType = new EnumScaleType();
  m_currentScaleType = m_enumScaleType->getDoubleAlternatingScaleBarType();
}

te::layout::ScaleModel::~ScaleModel()
{
  if(m_enumScaleType)
  {
    delete m_enumScaleType;
    m_enumScaleType = 0;
  }
}

te::layout::Properties* te::layout::ScaleModel::getProperties() const
{
  ItemModelObservable::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pro_widthGap;
  pro_widthGap.setName("scale_width_rect_gap");
  pro_widthGap.setId("");
  pro_widthGap.setValue(m_scaleGapX, dataType->getDataTypeDouble());
  m_properties->addProperty(pro_widthGap);

  Property pro_heightGap;
  pro_heightGap.setName("scale_height_rect_gap");
  pro_heightGap.setId("");
  pro_heightGap.setValue(m_scaleGapY, dataType->getDataTypeDouble());
  m_properties->addProperty(pro_heightGap);

  Property pro_mapName;
  pro_mapName.setName(m_sharedProps->getMapName());
  pro_mapName.setId("");
  pro_mapName.setValue(m_mapName, dataType->getDataTypeStringList());
  Variant v;
  v.setValue(m_mapName, dataType->getDataTypeString());
  pro_mapName.addOption(v);
  m_properties->addProperty(pro_mapName);

  Property pro_scaleName = scaleProperty();
  if(!pro_scaleName.isNull())
  {
    m_properties->addProperty(pro_scaleName);
  }

  return m_properties;
}

void te::layout::ScaleModel::updateProperties( te::layout::Properties* properties )
{
  ItemModelObservable::updateProperties(properties);

  Properties* vectorProps = const_cast<Properties*>(properties);

  Property pro_mapName = vectorProps->contains(m_sharedProps->getMapName());

  if(!pro_mapName.isNull())
  {
    m_mapName = pro_mapName.getOptionByCurrentChoice().toString();
  }

  Property pro_widthGap = vectorProps->contains("scale_width_rect_gap");

  if(!pro_widthGap.isNull())
  {
    m_scaleGapX = pro_widthGap.getValue().toDouble();
  }

  Property pro_heightGap = vectorProps->contains("scale_height_rect_gap");

  if(!pro_heightGap.isNull())
  {
    m_scaleGapY = pro_heightGap.getValue().toDouble();
  }

  Property pro_scaleName = vectorProps->contains("scale_type");

  if(!pro_scaleName.isNull())
  {
    std::string label = pro_scaleName.getOptionByCurrentChoice().toString();
    EnumType* enumType = m_enumScaleType->searchLabel(label);
    if(enumType)
    {
      m_currentScaleType = enumType;
    }
  }
}

void te::layout::ScaleModel::visitDependent(ContextItem context)
{
  MapModel* map = dynamic_cast<MapModel*>(m_visitable);

  if(map)
  {
    m_mapScale = map->getScale();

    notifyAll(context);
  }	
}

void te::layout::ScaleModel::setScaleGapX( double x )
{
  m_scaleGapX = x;
}

double te::layout::ScaleModel::getScaleGapX()
{
  return m_scaleGapX;
}

void te::layout::ScaleModel::setScaleGapY( double y )
{
  m_scaleGapY = y;
}

double te::layout::ScaleModel::getScaleGapY()
{
  return m_scaleGapY;
}

double te::layout::ScaleModel::getMapScale()
{
  return m_mapScale;
}

te::layout::EnumScaleType* te::layout::ScaleModel::getEnumScaleType()
{
  return m_enumScaleType;
}

te::layout::Property te::layout::ScaleModel::scaleProperty() const
{
  Property pro_scaleName;

  if(!m_currentScaleType)
    return pro_scaleName;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!dataType)
    return pro_scaleName;

  pro_scaleName.setName("scale_type");
  pro_scaleName.setLabel("graphic type");
  pro_scaleName.setId("");
  pro_scaleName.setValue(m_currentScaleType->getLabel(), dataType->getDataTypeStringList());

  Variant v;
  v.setValue(m_currentScaleType->getLabel(), dataType->getDataTypeString());
  pro_scaleName.addOption(v);
  pro_scaleName.setOptionChoice(v);

  for(int i = 0 ; i < m_enumScaleType->size() ; ++i)
  {
    EnumType* enumType = m_enumScaleType->getEnum(i);

    if(enumType == m_enumScaleType->getNoneType() || enumType == m_currentScaleType)
      continue;

    Variant v;
    v.setValue(enumType->getLabel(), dataType->getDataTypeString());
    pro_scaleName.addOption(v);
  }

  return pro_scaleName;
}

te::layout::EnumType* te::layout::ScaleModel::getCurrentScaleType()
{
  return m_currentScaleType;
}

