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
  \file AbstractBuildGraphicsItem.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_BUILD_GRAPHICS_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_BUILD_GRAPHICS_ITEM_H

// TerraLib
#include "../../geometry/Coord2D.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace layout
  {
    class Properties;
    class EnumType;
    class SharedProperties;

    class AbstractBuildGraphicsItem
    {
      public:
       
        AbstractBuildGraphicsItem();

        virtual ~AbstractBuildGraphicsItem();

      protected:
        
        virtual te::gm::Coord2D findCoordinate(te::layout::Properties* props);

        virtual int findZValue(te::layout::Properties* props);

        virtual std::string nameItem(std::string name, te::layout::EnumType* type) = 0;

        virtual void clear();

      protected:

        SharedProperties*       m_sharedProps;
        te::layout::Properties* m_props;
        te::gm::Coord2D         m_coord;
        int                     m_zValue;
        int                     m_id;
        bool                    m_redraw;
        std::string             m_name;
    };
  }
}

#endif
