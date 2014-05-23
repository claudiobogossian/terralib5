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
  \file GridSettingsModel.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GRID_SETTINGS_MODEL_H 
#define __TERRALIB_LAYOUT_INTERNAL_GRID_SETTINGS_MODEL_H

// TerraLib
#include "OutsideModelObservable.h"
#include "ContextItem.h"
#include "Properties.h"
#include "EnumMode.h"

namespace te
{
  namespace layout
  {
    class PlanarGridSettingsConfigProperties;
    class GeodesicGridSettingsConfigProperties;

    class GridSettingsModel : public OutsideModelObservable
    {
    public:

      GridSettingsModel();
      virtual ~GridSettingsModel();

      virtual void updateProperties(te::layout::Properties* properties);

      virtual Properties* getProperties() const;

      virtual void setOutsideProperty(Property property);

      virtual Property getOutsideProperty();

      virtual Property containsGrid(LayoutGridType gridType);

      virtual te::layout::Property containsOutsideSubProperty( std::string name, LayoutGridType gridType );
      
      virtual void updateOutsideSubProperty(Property subProperty, LayoutGridType gridType);

    protected:

      Property m_property;
      PlanarGridSettingsConfigProperties* m_planarGridSettings;
      GeodesicGridSettingsConfigProperties* m_geodesicGridSettings;
    };
  }
}

#endif