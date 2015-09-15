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
  \file TitleItem.h
   
  \brief Class daughter of te::layout::TextItem representing a grid with two cells which can be inserted texts.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TITLE_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_TITLE_ITEM_H

// TerraLib
#include "TextItem.h"
#include "../../core/Config.h"

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    /*!
      \brief Class daughter of te::layout::TextItem representing a grid with two cells which can be inserted texts.
      \ingroup layout
      \sa te::layout::TextItem
    */
    class TELAYOUTEXPORT TitleItem : public TextItem
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */
        TitleItem(AbstractItemController* controller, bool invertedMatrix = false);

        /*!
          \brief Destructor
         */
        virtual ~TitleItem();

      protected slots:

        virtual void updateGeometry( int position, int charsRemoved, int charsAdded );
    };
  }
}

#endif
