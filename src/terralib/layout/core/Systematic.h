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
  \file Systematic.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_SYSTEMATIC_H 
#define __TERRALIB_LAYOUT_INTERNAL_SYSTEMATIC_H

namespace te
{
  namespace layout
  {
    class Systematic 
    {
      public:

        /*!
         \brief 
        */
        Systematic();

        /*!
         \brief 
        */
        virtual ~Systematic();

        virtual void setWidth(double w);

        virtual double getWidth();

        virtual void setHeight(double h);

        virtual double getHeight();

        virtual void setScale(int scale);

        virtual int getScale();

        virtual void setPlanarGap(double gap);

        virtual double getPlanarGap();

        virtual void setGeoGap(double gap);

        virtual double getGeoGap();

      protected:

        double m_width;
        double m_height;
        int m_scale;
        double m_planarGap;
        double m_geoGap;//minutes
    };
  }
}

#endif
