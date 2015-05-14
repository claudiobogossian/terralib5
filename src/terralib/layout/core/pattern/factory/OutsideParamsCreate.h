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
    class OutsideController;
    class Observable;

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
      OutsideParamsCreate(OutsideController* controller, Observable* model);

      /*!
          \brief Destructor
       */ 
      virtual ~OutsideParamsCreate();

      /*!
          \brief Returns the "Controller" part of the MVC.

          \return controller
       */
      OutsideController*	getController();

      /*!
          \brief Returns the "Model" part of the MVC.

          \return model
       */
      Observable*		getModel();

    protected:
      OutsideController*	m_controller; //!< "Controller" part of MVC widget.
      Observable*		m_model; //!< "Model" part of MVC widget.
    };

    /*
    ** ---------------------------------------------------------------
    ** Inline methods:
    */
    inline OutsideParamsCreate::OutsideParamsCreate(OutsideController* controller, Observable* model) :
	    m_controller(controller),
	    m_model(model)
    {
    }

    inline OutsideParamsCreate::~OutsideParamsCreate() 
    {
    }

    inline OutsideController* OutsideParamsCreate::getController()
    {
	    return m_controller;
    }

    inline Observable* OutsideParamsCreate::getModel()
    {
	    return m_model;
    }
  }
}

#endif
