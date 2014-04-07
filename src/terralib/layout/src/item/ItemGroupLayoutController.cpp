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
  \file ItemGroupLayoutController.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemGroupLayoutController.h"
#include "LayoutItemController.h"
#include "ContextLayoutItem.h"
#include "LayoutAbstractItemFactory.h"
#include "LayoutContext.h"
#include "LayoutItemModelObservable.h"
#include "LayoutItemParamsCreate.h"
#include "LayoutObserver.h"

te::layout::ItemGroupLayoutController::ItemGroupLayoutController( LayoutObservable* o ) :
  LayoutItemController(o)
{
  LayoutAbstractItemFactory* factory = LayoutContext::getInstance()->getItemFactory(); 
  LayoutItemParamsCreate params(this, m_model);
  m_view = (LayoutObserver*)factory->make(TPItemGroup, params);
}

te::layout::ItemGroupLayoutController::~ItemGroupLayoutController()
{
	
}

void te::layout::ItemGroupLayoutController::setPosition( const double& x, const double& y )
{
  if(m_model)
  {
    LayoutItemModelObservable* model = dynamic_cast<LayoutItemModelObservable*>(m_model);
    if(model)
      return model->setPosition(x, y);
  }
}
