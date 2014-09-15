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
  \file GridSettingsModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridSettingsModel.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/property/PlanarGridSettingsConfigProperties.h"
#include "../core/property/GeodesicGridSettingsConfigProperties.h"

te::layout::GridSettingsModel::GridSettingsModel() 
{
  m_box = te::gm::Envelope(0., 0., 200., 200.);

  m_planarGridSettings = new PlanarGridSettingsConfigProperties;
  m_geodesicGridSettings = new GeodesicGridSettingsConfigProperties;
}

te::layout::GridSettingsModel::~GridSettingsModel()
{
  if(m_planarGridSettings)
  {
    delete m_planarGridSettings;
    m_planarGridSettings = 0;
  }

  if(m_geodesicGridSettings)
  {
    delete m_geodesicGridSettings;
    m_geodesicGridSettings = 0;
  }
}

te::layout::Properties* te::layout::GridSettingsModel::getProperties() const
{
  m_properties->clear();

  Property pro_name;
  pro_name.setName("GridSettings");

  m_properties->addProperty(pro_name);

  m_properties->setTypeObj(m_type);
  return m_properties;
}

void te::layout::GridSettingsModel::updateProperties( te::layout::Properties* properties )
{

}

void te::layout::GridSettingsModel::setOutsideProperty( Property property )
{
  m_property.clear();
  m_property = property;
}

te::layout::Property te::layout::GridSettingsModel::getOutsideProperty()
{
  return m_property;
}

te::layout::Property te::layout::GridSettingsModel::containsOutsideSubProperty( std::string name, LayoutGridType gridType )
{
  Property prop = containsGrid(gridType);
  return prop.containsSubProperty(name);
}

void te::layout::GridSettingsModel::updateOutsideSubProperty( Property subProperty, LayoutGridType gridType )
{
  Property sub;

  sub = containsGrid(gridType);

  if(sub.isNull())
    return;

  if(sub.containsSubProperty(subProperty))
  {
    sub.removeSubProperty(subProperty);
    sub.addSubProperty(subProperty);

    m_property.removeSubProperty(sub);
    m_property.addSubProperty(sub);
  }
}

te::layout::Property te::layout::GridSettingsModel::containsGrid( LayoutGridType gridType )
{
  Property prop;

  if(gridType == TypeGeodesic)
  {
    prop = m_property.containsSubProperty(m_geodesicGridSettings->getName());
  }
  if(gridType == TypePlanar)
  {
    prop = m_property.containsSubProperty(m_planarGridSettings->getName());
  }

  return prop;
}
