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
  \file HorizontalRulerController.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "HorizontalRulerController.h"
#include "ItemController.h"
#include "ContextItem.h"
#include "ItemModelObservable.h"
#include "AbstractItemFactory.h"
#include "Context.h"

te::layout::HorizontalRulerController::HorizontalRulerController( Observable* o ) :
  ItemController(o, TPHorizontalRuler)
{
  AbstractItemFactory* factory = Context::getInstance()->getItemFactory(); 
  ItemParamsCreate params(this, m_model);
  m_view = (Observer*)factory->make(TPHorizontalRuler, params);
}

te::layout::HorizontalRulerController::~HorizontalRulerController()
{

}

void te::layout::HorizontalRulerController::setPosition( const double& x, const double& y )
{
  ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
  model->setPosition(x, y);
}
