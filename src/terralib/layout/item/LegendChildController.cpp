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
  \file LegendChildController.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LegendChildController.h"
#include "../core/ContextItem.h"
#include "../core/pattern/factory/AbstractItemFactory.h"
#include "../core/pattern/singleton/Context.h"
#include "../core/pattern/mvc/ItemModelObservable.h"
#include "../core/pattern/factory/ItemParamsCreate.h"
#include "../core/pattern/mvc/ItemObserver.h"
#include "../core/enum/Enums.h"

te::layout::LegendChildController::LegendChildController( Observable* o ) :
  LegendController(o, 0)
{
  create();
}

te::layout::LegendChildController::LegendChildController( Observable* o, EnumType* type ) :
  LegendController(o, type)
{

}

te::layout::LegendChildController::~LegendChildController()
{
	
}

void te::layout::LegendChildController::create()
{
  AbstractItemFactory* factory = Context::getInstance().getItemFactory(); 
  ItemParamsCreate params(this, m_model);
  m_view = (Observer*)factory->make(m_model->getType(), params);
}
