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
  \file terralib/layout/qt/core/pattern/factory/ImageItemFactory.h

  \brief This is the concrete factory for image item.
 */

#ifndef __TERRALIB_LAYOUT_INTERNAL_IMAGE_ITEM_FACTORY_H
#define __TERRALIB_LAYOUT_INTERNAL_IMAGE_ITEM_FACTORY_H

// TerraLib
#include "../../../../../core/Config.h"
#include "ItemFactory.h"
#include "ItemFactoryParamsCreate.h"

namespace te
{
  namespace layout
  {
    class AbstractItemView;
    /*!
      \class ImageItemFactory

      \brief This is the concrete factory for image item.

      \sa NewItemFactory, ParameterizedAbstractFactory, AbstractItemView
    */
    class TELAYOUTEXPORT ImageItemFactory : public ItemFactory
    {
      public:

        /*! It register the factory in the abstract dictionary. */
        ImageItemFactory();

        /*! \brief Destructor. */
        ~ImageItemFactory();

      protected:

        /*!
          \brief It creates a new zoom area tool.

          \return It returns zoom area tool.
        */
        AbstractItemView* build(ItemFactoryParamsCreate params);
    };

  } // end namespace layout
}   // end namespace te


#endif


