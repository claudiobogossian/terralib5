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
  \file ItemParamsCreate.h
   
  \brief Parameters to creation new object. Ex.: "Model" and "Controller" of the new object (MVC component).

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_PARAMS_CREATE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_PARAMS_CREATE_H

// TerraLib
#include "../../ParamsCreate.h"

namespace te
{
  namespace layout
  {
    class ItemController;
    class Observable;

    class AbstractItemController;
    class AbstractItemModel;

    /*!
    \brief Parameters to creation new object. Ex.: "Model" and "Controller" of the new object (MVC component).
    
    \ingroup layout

    \sa te::layout::ParamsCreate
    */
    class ItemParamsCreate : public ParamsCreate 
    {
      public:

        /*!
          \brief Constructor
        */ 
        ItemParamsCreate();

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        ItemParamsCreate(ItemController* controller, Observable* model);

        /*!
          \brief Destructor
        */ 
        virtual ~ItemParamsCreate();

        /*!
          \brief Returns the "Controller" part of the MVC.

          \return controller
        */
        ItemController*  getController();

        /*!
          \brief Returns the "Model" part of the MVC.

          \return model
        */
        Observable*    getModel();
        
        protected:
        ItemController*  m_controller; //!< "Controller" part of MVC component.
        Observable*    m_model; //!< "Model" part of MVC component.

        //temporary
      public:
        AbstractItemController* m_newController; //!< "Controller" part of MVC component.
        AbstractItemModel*      m_newModel; //!< "Model" part of MVC component.
    };

    /*
    ** ---------------------------------------------------------------
    ** Inline methods:
    */
    inline ItemParamsCreate::ItemParamsCreate(ItemController* controller, Observable* model) 
      : m_controller(controller)
      , m_model(model)
      , m_newController(0)
      , m_newModel(0)
    {
    }

    inline ItemParamsCreate::~ItemParamsCreate() 
    {
    }

    inline ItemController* ItemParamsCreate::getController()
    {
      return m_controller;
    }

    inline Observable* ItemParamsCreate::getModel()
    {
      return m_model;
    }
  }
}
#endif