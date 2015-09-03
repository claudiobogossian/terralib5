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
  \file terralib/layout/qt/pattern/factory/outside/NewOutsideFactory.h

  \brief This is the abstract factory for items.
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_FACTORY_H 
#define __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_FACTORY_H

// TerraLib
#include "terralib/geometry/Coord2D.h"
#include "../../../../../../common/ParameterizedAbstractFactory.h"
#include "../../../../../core/Config.h"
#include "../../../../../core/pattern/mvc/AbstractOutsideView.h"
#include "OutsideFactoryParamsCreate.h"

// STL
#include <string>

// Qt
#include <QGraphicsItem>

namespace te
{
  namespace layout
  {
    class AbstractOutsideModel;
    /*!
      \class NewOutsideFactory

      \brief This is the abstract factory for outsides.

      It will create objects of type AbstractItemView and will pass
      parameters of type ItemFactoryParamsCreate to their factories constructors.

      If you want a new item you can use a command like:
      <code>
        te::layout::AbstractItem* pEngine = te::layout::NewItemFactory::make("RECTANGLE_ITEM");
      </code>
      Or
      <code>
        te::layout::EnumObjectType* item = Enums::getInstance().getEnumObjectType();
        te::layout::EnumType* rectItem = item->getRectangleItem();
        te::layout::AbstractItem* pEngine = te::layout::NewItemFactory::make(rectItem->getName());
      </code>

      \note The caller of the method make will take the ownership of the returned item.

      \sa AbstractOutsideView, AbstractFactory, ParameterizedAbstractFactory
    */
    class TELAYOUTEXPORT OutsideFactory : public te::common::ParameterizedAbstractFactory<AbstractOutsideView, std::string, OutsideFactoryParamsCreate>
    {
      public:

        /*! \brief Virtual destructor. */
        virtual ~OutsideFactory();

      protected:

        /*!
        \brief It creates the factory.

        The key of a ToolFactory is a string.

        \param factoryKey The key that identifies the factory.
        */
        OutsideFactory(const std::string& factoryKey);

        virtual void setProperties(AbstractOutsideModel* model, OutsideFactoryParamsCreate params);
        
    };
  }
}

#endif
