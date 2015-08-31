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
  \file Observable.h
   
  \brief Abstract class to represent an observable. "Model" part of MVC component. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACTITEMVIEW_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACTITEMVIEW_H

// TerraLib
#include "../../Config.h"
#include "../../../core/ContextObject.h"

namespace te
{
  namespace layout
  {

    class AbstractItemController;
    class AbstractItemModel;
    class ContextObject;

    /*!
      \brief Abstract class to represent an observable. "Model" part of MVC component. 
    
      \ingroup layout
    */
    class TELAYOUTEXPORT AbstractItemView
    {
      public:

        /*!
          \brief Constructor
        */ 
        AbstractItemView(AbstractItemController* controller, AbstractItemModel* model, bool invertedMatrix = false);

        /*!
          \brief Destructor
        */ 
        virtual ~AbstractItemView();

        /*!
          \brief Gets the controller
        */ 
        virtual AbstractItemController* getController() const;

        /*!
          \brief Refreshes the drawings of the view
        */
        virtual void refresh() = 0;

        /*!
        \brief This function is called every time the context is updated. It is usually used by the item to configure its drawings due to changes in the zoom factor or in the DPI of the device.
        */
        virtual void contextUpdated(const ContextObject& context) = 0;

        /*!
          \brief Gets the rotation
        */ 
        virtual double getItemRotation() const = 0;

        /*!
          \brief Sets the rotation
        */ 
        virtual void setItemRotation(double rotation) = 0;

        /*!
        \brief Returns whether the graph component has the inverted matrix, otherwise the matrix scene.

        \return true if inverted, false otherwise
        */
        bool isInverted();

        virtual void setEditionMode(bool editionMode);

        bool isEditionMode();

        virtual void enterEditionMode();
        
        virtual void leaveEditionMode();
        
      protected:

        AbstractItemController* m_controller; //!< The controller
        bool                    m_invertedMatrix;
        bool                    m_isEditionMode;
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_ABSTRACTITEMVIEW_H
