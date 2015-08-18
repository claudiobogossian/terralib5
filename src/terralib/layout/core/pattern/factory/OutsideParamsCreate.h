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
  \file OutsideParamsCreate.h
   
  \brief Parameters to creation new object. Ex.: "Model" and "Controller" of the new object (MVC widget).

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_PARAMS_CREATE_H 
#define __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_PARAMS_CREATE_H

// TerraLib
#include "../../ParamsCreate.h"

namespace te
{
  namespace layout
  {
		class AbstractOutsideController;

    /*!
	  \brief Parameters to creation new object. Ex.: "Model" and "Controller" of the new object (MVC widget).
	  
	  \ingroup layout

	  \sa te::layout::ParamsCreate
	  */
    class OutsideParamsCreate : public ParamsCreate 
    {
    public:

      /*!
          \brief Constructor
       */ 
      OutsideParamsCreate();

      /*!
          \brief Constructor

          \param controller "Controller" part of MVC widget
          \param o "Model" part of MVC widget
       */ 
      OutsideParamsCreate(AbstractOutsideController* controller);

      /*!
          \brief Destructor
       */ 
      virtual ~OutsideParamsCreate();

      /*!
          \brief Returns the "Controller" part of the MVC.

          \return controller
       */
      AbstractOutsideController*	getController();
			
    protected:
      AbstractOutsideController*	m_controller; //!< "Controller" part of MVC widget.
    };

    /*
    ** ---------------------------------------------------------------
    ** Inline methods:
    */
		inline OutsideParamsCreate::OutsideParamsCreate(AbstractOutsideController* controller) :
	    m_controller(controller)
    {
    }

    inline OutsideParamsCreate::~OutsideParamsCreate() 
    {
    }

		inline AbstractOutsideController* OutsideParamsCreate::getController()
    {
	    return m_controller;
    }
  }
}

#endif
