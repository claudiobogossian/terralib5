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
  \file AbstractEnum.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_ENUM_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_ENUM_H

// TerraLib
#include "EnumType.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace layout
  {
    class AbstractEnum
    {
      public:
       
        AbstractEnum();

        virtual ~AbstractEnum();

        virtual EnumType* getEnum(int enumId) const;

        virtual EnumType* getEnum(std::string name) const;

      protected:
        
          virtual void init() = 0;

      protected:

        std::vector<EnumType*> m_enums;
    };
  }
}

#endif
