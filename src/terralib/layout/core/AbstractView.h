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
  \file AbstractView.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_VIEW_H 
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_VIEW_H

// TerraLib
#include "Config.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace layout
  {
    class TELAYOUTEXPORT AbstractView 
    {
      public:
        
        AbstractView();

        virtual ~AbstractView(void){}
        
        virtual bool getVisibleRulers();

        virtual void setVisibleRulers(bool visible);

        virtual void config() = 0; 

        virtual void addZoomFactor(double factor, std::string text);

        virtual void clearZoomFactors();

        virtual double nextFactor(double currentFactor);

        virtual double previousFactor(double currentFactor);

      protected:

        bool                          m_visibleRulers;
        std::map<double, std::string> m_zoomFactors;
    };
  }
}

#endif
