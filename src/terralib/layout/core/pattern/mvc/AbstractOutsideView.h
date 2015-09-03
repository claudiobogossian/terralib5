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
  \file AbstractOutsideView.h
   
  \brief Abstract class to represent an observer. "View" part of MVC widget. All classes representing the graphics widget must inherit from this class.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_OUTSIDE_OBSERVER_H 
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_OUTSIDE_OBSERVER_H

// TerraLib
#include "../../property/Properties.h"
#include "../../../../geometry/Coord2D.h"
#include "../../Config.h"

namespace te
{
  namespace layout
  {
    class AbstractOutsideController;

    /*!
      \brief Abstract class to represent an observer. "View" part of MVC widget. All classes representing the graphics component must inherit from this class.
    
      \ingroup layout

      \sa te::layout::Observer
    */
    class TELAYOUTEXPORT AbstractOutsideView 
    {
    public:

      /*!
          \brief Constructor

          \param controller "Controller" part of MVC widget
          \param o "Model" part of MVC widget
      */ 
      AbstractOutsideView(AbstractOutsideController* controller);

      /*!
          \brief Destructor
       */
      virtual ~AbstractOutsideView();
      
      /*!
          \brief Change coordinate llx,lly of the MVC widget.
            Reimplement this function in a ItemController subclass to provide the controller's create implementation.

          \param x llx
          \param y lly
       */
      virtual void setPosition(const double& x, const double& y) = 0;

      /*!
          \brief Method that returns the position llx, lly
            Reimplement this function in a AbstractOusideView subclass to provide the item's getPosition implementation.

          \return coordinate
       */ 
      virtual te::gm::Coord2D getPosition() = 0;

      /*!
          \brief Updated model state.

          \param true if refresh the current position, false otherwise
       */
      virtual void refresh();
      
      /*!
          \brief Returns the "Controller" part of the MVC.

          \return controller
       */
      virtual AbstractOutsideController* getController();
            
    protected:

      AbstractOutsideController*  m_controller; //!< "Controller" part of MVC widget.
    };
  }
}

#endif
