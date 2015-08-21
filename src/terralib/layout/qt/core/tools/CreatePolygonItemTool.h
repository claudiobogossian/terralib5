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
  \file terralib/layout/qt/core/tools/CreatePolygonItemTool.h

  \brief This class implements a concrete tool to create polygon item.
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_CREATE_POLYGON_ITEM_TOOL_H
#define __TERRALIB_LAYOUT_INTERNAL_CREATE_POLYGON_ITEM_TOOL_H

//TerraLib
#include "CreateLineItemTool.h"

namespace te
{
  namespace layout
  {
      // Forward declarations
    class EnumType;

      /*!
        \class CreatePolygonItemTool

        \brief This class implements a concrete tool to create polygon in a QGraphics Item.
      */
    class TELAYOUTEXPORT CreatePolygonItemTool : public CreateLineItemTool
    {
      public:

        /** @name Initializer Methods
          *  Methods related to instantiation and destruction.
          */
        //@{

        /*!
          \brief It constructs a create polygon item tool associate with a View.

          \param view The tool's view.
          \param parent The tool's parent..
        */
        CreatePolygonItemTool(View* view, EnumType* itemType = 0, QObject* parent = 0);

        /*! \brief Destructor. */
        ~CreatePolygonItemTool();

        //@}
    };

  }   // end namespace layout
}     // end namespace te

#endif  // __TERRALIB_LAYOUT_INTERNAL_CREATE_POLYGON_ITEM_TOOL_H
