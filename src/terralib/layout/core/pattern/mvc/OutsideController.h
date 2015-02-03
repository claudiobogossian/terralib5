/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file OutsideController.h
   
  \brief Abstract class to represent a controller. "Controller" part of MVC widget. All classes representing the controller of a widget must inherit from this class.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_CONTROLLER_H 
#define __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_CONTROLLER_H

// TerraLib
#include "../../enum/AbstractType.h"
#include "../../Config.h"

namespace te
{
  namespace layout
  {
    class Observable;
    class Observer;
    class EnumType;

    /*!
      \brief Abstract class to represent a controller. "Controller" part of MVC widget. All classes representing the controller of a widget must inherit from this class.
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT OutsideController 
    {
    public:

      /*!
          \brief Constructor

          \param o "Model" part of MVC widget
       */
      OutsideController(Observable* o);

      /*!
          \brief Constructor

          \param o "Model" part of MVC widget
          \param type type of the MVC widget.
       */
      OutsideController(Observable* o, EnumType* type);

      /*!
          \brief Destructor
       */ 
      virtual ~OutsideController();

      /*!
          \brief Change coordinate llx,lly of the MVC widget.

          \param x llx
          \param y lly
        */
      virtual void setPosition(const double& x, const double& y);

      /*!
          \brief Returns the "Model" part of the MVC widget.

          \return model
       */
      const Observable* getModel();

      /*!
          \brief Returns the "View" part of the MVC widget.

          \return view 
       */
      const Observer* getView();

    protected:

      /*!
          \brief Call factory to create the "View" part of the MVC widget and passes the model and himself as controller.
         */
      virtual void create();

      Observable* m_model; //!< "Model" part of the MVC widget.
      Observer* m_view; //!< "View" part of the MVC widget.
    };
  }
}

#endif
