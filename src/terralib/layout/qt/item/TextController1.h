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
  \file TextController.h
   
  \brief Class that represents text controller.
  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEXT_CONTROLLER1_H
#define __TERRALIB_LAYOUT_INTERNAL_TEXT_CONTROLLER1_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractItemController.h"

class QSizeF;

namespace te
{
  namespace layout
  {
    /*!
    \brief Class that represents text controller.
    
    \ingroup layout
    \sa te::layout::AbstractItemController
    */
    class TELAYOUTEXPORT TextController1 : public AbstractItemController
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        TextController1( AbstractItemModel* model);

        /*!
          \brief Destructor
        */ 
        virtual ~TextController1();

        /*!
          \brief Method called by the subject to inform changes in the model
        */
        virtual void update(const Subject* subject);

        /*!
          \brief Updates the view with the current configuration of the model
          \return The new size of the view
        */
        virtual QSizeF updateView();

        /*!
          \brief Updates the model with the new size of the view
          \param size The new size of the view
        */
        virtual void updateModel(const QSizeF& size);

        /*!
          \brief This function will be called by the view every time the view is being changed
                 If the return 'newValue' has been changed, the return value must be true
         */
        bool itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & currentValue, QVariant& newValue );
    };
  }
}
#endif //__TERRALIB_LAYOUT_INTERNAL_TEXT_CONTROLLER_H
