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
  \file SharedProperties.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_SHARED_PROPERTIES_H
#define __TERRALIB_LAYOUT_INTERNAL_SHARED_PROPERTIES_H

// STL
#include <string>

namespace te
{
  namespace layout
  {
    class SharedProperties
    {
      public:

        SharedProperties();
        virtual ~SharedProperties(void);
      
        std::string getName();
        std::string getId();
        std::string getMapName();
        std::string getX1();
        std::string getX2();
        std::string getY1();
        std::string getY2();
        std::string getZValue();
        std::string getBorder();
        std::string getBackgroundcolor();
        std::string getBordercolor();
        std::string getAngle();
        std::string getWidth();
        std::string getHeight();

      protected:

        std::string m_name;
        std::string m_id;
        std::string m_mapName;
        std::string m_x1;
        std::string m_x2;
        std::string m_y1;
        std::string m_y2;
        std::string m_zValue;
        std::string m_border;
        std::string m_backgroundcolor;
        std::string m_bordercolor;
        std::string m_angle;
        std::string m_width;
        std::string m_height;
    };
  }
}

#endif