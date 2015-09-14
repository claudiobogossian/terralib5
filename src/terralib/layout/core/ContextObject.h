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
  \file ContextObject.h
   
   \brief Class with the most important common data. These data could be used in other module layers or accessed from outside. 
   The common data reported here are some state attributes in te::layout::View;

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_CONTEXT_OBJECT_H
#define __TERRALIB_LAYOUT_INTERNAL_CONTEXT_OBJECT_H

// TerraLib
#include "Config.h"

namespace te
{
  namespace layout
  {
    class PaperConfig;
    class EnumType;

    /*!
    \brief Class with the most important common data. These data could be used in other module layers or accessed from outside. 
    The common data reported here are some state attributes in te::layout::View and te::layout::Scene.
    
    \ingroup layout
    */
    class TELAYOUTEXPORT ContextObject
    {
      public:

        ContextObject( int zoom, int dpiX, int dpiY, EnumType* mode);

        virtual ~ContextObject();

        virtual int getZoom() const;

        virtual int getDpiX() const;

        virtual int getDpiY() const;

        virtual EnumType* getCurrentMode() const;

        virtual bool isValid();

      protected:

        int           m_zoom;
        int           m_dpiX;
        int           m_dpiY;
        EnumType*     m_mode;
    };
  }
}

#endif // __TERRALIB_LAYOUT_INTERNAL_CONTEXT_OBJECT_H


