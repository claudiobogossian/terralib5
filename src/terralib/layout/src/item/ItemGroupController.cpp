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
  \file ItemGroupController.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemGroupController.h"
#include "ItemController.h"
#include "ContextItem.h"
#include "AbstractItemFactory.h"
#include "Context.h"
#include "ItemModelObservable.h"
#include "ItemParamsCreate.h"
#include "Observer.h"

te::layout::ItemGroupController::ItemGroupController( Observable* o ) :
  ItemController(o, TPItemGroup)
{
  AbstractItemFactory* factory = Context::getInstance()->getItemFactory(); 
  ItemParamsCreate params(this, m_model);
  m_view = (Observer*)factory->make(TPItemGroup, params);
}

te::layout::ItemGroupController::~ItemGroupController()
{
	
}

void te::layout::ItemGroupController::setPosition( const double& x, const double& y )
{
  if(m_model)
  {
    ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
    if(model)
      return model->setPosition(x, y);
  }
}
