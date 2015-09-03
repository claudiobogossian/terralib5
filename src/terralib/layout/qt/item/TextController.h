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

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEXT_CONTROLLER_H
#define __TERRALIB_LAYOUT_INTERNAL_TEXT_CONTROLLER_H

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
    class TELAYOUTEXPORT TextController : public AbstractItemController
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        TextController(AbstractItemModel* model);

        /*!
          \brief Destructor
        */ 
        virtual ~TextController();

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
          \brief Synchronizes the information between the model and the view
          \return TRUE if the size of the view changed due to the changes in the model. FALSE if the size did not change
         */
        virtual bool sync();

        /*!
          \brief This function will be called by the view every time the item´s scene was changed
         */
        virtual void sceneChanged();

        /*!
          \brief This function will be called by the view every time the item´s text was changed
         */
        virtual void textChanged();
    
      protected:

        /*!
        \brief Reimplemented from AbstractItemController
        */
        virtual void refresh();
    };
  }
}
#endif //__TERRALIB_LAYOUT_INTERNAL_TEXT_CONTROLLER_H
