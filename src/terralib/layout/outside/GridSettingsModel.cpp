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
  \file GridSettingsModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridSettingsModel.h"
#include "../core/property/Properties.h"
#include "../core/property/PlanarGridSettingsConfigProperties.h"
#include "../core/property/GeodesicGridSettingsConfigProperties.h"
#include "../core/enum/Enums.h"

te::layout::GridSettingsModel::GridSettingsModel() :
  AbstractOutsideModel()
{
  m_type = Enums::getInstance().getEnumObjectType()->getGridSettings();

  m_box = te::gm::Envelope(0., 0., 200., 200.);
}

te::layout::GridSettingsModel::~GridSettingsModel()
{

}

void te::layout::GridSettingsModel::setProperties(std::vector<te::layout::Properties*> properties)
{
  m_properties = properties;
}

te::layout::Property te::layout::GridSettingsModel::containsProperty( std::string name, EnumType* enumType )
{
  Property prop;

  Properties* props = containsGrid(enumType);
  if(!props)
  {
    return prop;
  }

  prop = props->getProperty(name);
  return prop;
}

bool te::layout::GridSettingsModel::updateProperty( Property prop, EnumType* enumType )
{
  Properties* props = containsGrid(enumType);

  if(!props)
  {
    return false;
  }
  
  bool result = props->contains(prop);
  if(!result)
  {
    return result;
  }

  if(props->removeProperty(prop.getName()))
  {
    props->addProperty(prop);
  }
  return result;
}

te::layout::Properties* te::layout::GridSettingsModel::containsGrid(EnumType* enumType)
{
  Properties* properties = 0;

  std::vector<Properties*>::iterator it = m_properties.begin();
  for(it ; it != m_properties.end() ; ++it)
  {
    Properties* props = (*it);
    if(props)
    {
      if(props->getTypeObj() == enumType)
      {
        properties = props;
        break;
      }
    }
  }

  return properties;
}




