/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file LayoutAbstractItemFactory.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_ITEM_FACTORY_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_ITEM_FACTORY_H

// TerraLib
#include "LayoutItemParamsCreate.h"
#include "LayoutAbstractType.h"

namespace te
{
  namespace layout
  {
    class LayoutObserver;

    class LayoutAbstractItemFactory 
    {
      public:
        virtual ~LayoutAbstractItemFactory(void) {}
        virtual LayoutObserver* make(LayoutAbstractObjectType type, LayoutItemParamsCreate params = LayoutItemParamsCreate()) = 0;
      };
  }
}

#endif