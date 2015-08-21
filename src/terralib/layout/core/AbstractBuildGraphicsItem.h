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
  \file AbstractBuildGraphicsItem.h
   
  \brief Abstract class for build graphics MVC components. 
    
  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_BUILD_GRAPHICS_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_BUILD_GRAPHICS_ITEM_H

// TerraLib
#include "../../geometry/Coord2D.h"
#include "property/Properties.h"
#include "Config.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace layout
  {
    class Properties;
    class EnumType;

  /*!
      \class AbstractBuildGraphicsItem

      \brief Abstract class for build graphics MVC components. 

      \ingroup layout
     */

    class TELAYOUTEXPORT AbstractBuildGraphicsItem
    {
      public:
    
        /*!
          \brief Constructor
        */       
        AbstractBuildGraphicsItem();
    
        /*!
          \brief Destructor
        */
        virtual ~AbstractBuildGraphicsItem();
        
        /*!
          \brief Clear all configuration for create or build a graphic object. 
        */
        virtual void clear();

      protected:
        
        /*!
          \brief Search for property of a graphic object that represent the x,y coordinate.

          \param props properties

          \return coordinate
        */
        virtual te::gm::Coord2D findCoordinate(te::layout::Properties props);

        /*!
          \brief Search for property of a graphic object that represent Z Value.

          \param props properties

          \return z value
        */
        virtual int findZValue(te::layout::Properties props);

        /*!
          \brief Creates the name of the new graphic object. Adds the number that corresponds to how many objects of this type have already been created.

          \param name name of the class type of the graphic object
          \param type type of the object

          \return name
        */
        virtual std::string nameItem(te::layout::EnumType* type) = 0;

      protected:

        te::layout::Properties  m_props; //!< Properties of a graphic object.
        te::gm::Coord2D         m_coord; //!< Coordinate of a graphic object.
        int                     m_zValue; //!< Z Value of a graphic object.
        int                     m_id; //!< Id of a graphic object.
        double                  m_width;
        double                  m_height;
        std::string             m_name;
    };
  }
}

#endif
