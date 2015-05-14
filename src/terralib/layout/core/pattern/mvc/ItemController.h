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
  \file ItemController.h
   
  \brief Abstract class to represent a controller. "Controller" part of MVC component. All classes representing the controller of a component must inherit from this class.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_CONTROLLER_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_CONTROLLER_H

// TerraLib
#include "../../../../geometry/Envelope.h"
#include "../../../../geometry/Coord2D.h"
#include "../../../../color/RGBAColor.h"
#include "../../enum/AbstractType.h"
#include "../../enum/EnumType.h"
#include "../../Config.h"

namespace te
{
  namespace layout
  {
    class Observable;
    class Observer;
    class Properties;

    /*!
      \brief Abstract class to represent a controller. "Controller" part of MVC component. All classes representing the controller of a component must inherit from this class.
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT ItemController 
    {
      public:

        /*!
          \brief Constructor

          \param o "Model" part of MVC component
        */
        ItemController(Observable* o);
        
        /*!
          \brief Destructor
        */ 
        virtual ~ItemController();

        /*!
          \brief Change coordinate llx,lly of the MVC component.

          \param x llx
          \param y lly
         */
        virtual void setPosition(const double& x, const double& y);

        /*!
          \brief Redraws the MVC component. 
            Creates the ContextItem object and configures it. Calls the draw method of the model.
        */
        virtual void redraw();

        /*!
          \brief Returns the "Model" part of the MVC component.

          \return model
        */
        const Observable* getModel();

        /*!
          \brief Returns the "View" part of the MVC component.

          \return view 
        */
        const Observer* getView();

        /*!
          \brief Change the bounding rectangle.

          \param bounding rectangle
         */
        virtual void setBox(te::gm::Envelope box);
        
        /*!
          \brief Checks if the coordinate is contained within the bounding rectangle.

          \param coord coordinated to be verified
          \return true if contains, false otherwise
         */
        virtual bool contains(const te::gm::Coord2D &coord) const;

      protected:

        /*!
          \brief Call factory to create the "View" part of the MVC component and passes the model and himself as controller.
         */
        virtual void create();

        Observable* m_model; //!< "Model" part of the MVC component.
        Observer* m_view; //!< "View" part of the MVC component.
    };
  }
}

#endif


