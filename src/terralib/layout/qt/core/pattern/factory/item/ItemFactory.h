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
  \file ItemFactory.h
   
  \brief Factory for creating families of related or dependent graphic objects (MVC components).

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_FACTORY_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_FACTORY_H

// TerraLib
#include "../../../../../core/pattern/factory/AbstractItemFactory.h"
#include "../../../../../core/Config.h"

namespace te
{
  namespace layout
  {
    /*!
    \brief Factory for creating families of related or dependent graphic objects (MVC components).
    
    \ingroup layout

    \sa te::layout::AbstractItemFactory
    */
    class TELAYOUTEXPORT ItemFactory : public AbstractItemFactory
    {
      public:

        /*!
          \brief Constructor
        */
        ItemFactory();

        /*!
          \brief Destructor
        */
        virtual ~ItemFactory();

        /*!
          \brief Reimplemented from AbstractItemFactory
        */
        virtual Observer* make(EnumType* type, ItemParamsCreate params = ItemParamsCreate());

        virtual AbstractItemView* makeNew(EnumType* type, ItemParamsCreate params = ItemParamsCreate());
    };
  }
}

#endif
