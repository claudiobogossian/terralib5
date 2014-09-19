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
  \file EnumLineStyleType.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ENUM_LINESTYLE_TYPE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENUM_LINESTYLE_TYPE_H

// TerraLib
#include "AbstractEnum.h"
#include "../Config.h"

namespace te
{
  namespace layout
  {
    class EnumType;

    class TELAYOUTEXPORT EnumLineStyleType : public AbstractEnum
    {
      public:

        EnumLineStyleType(); 

        virtual ~EnumLineStyleType();
        
        virtual EnumType* getStyleNone() const;

        virtual EnumType* getStyleSolid() const;

        virtual EnumType* getStyleDash() const;

        virtual EnumType* getStyleDot() const;

        virtual EnumType* getStyleDashDot() const;

        virtual EnumType* getStyleDashDotDot() const;

        virtual EnumType* getStyleCustomDash() const;

      protected:

        virtual void init();

      protected:

        EnumType* m_styleNone;
        EnumType* m_styleSolid;
        EnumType* m_styleDash;
        EnumType* m_styleDot;
        EnumType* m_styleDashDot;
        EnumType* m_styleDashDotDot;
        EnumType* m_styleCustomDash;
    };
  }
}

#endif
