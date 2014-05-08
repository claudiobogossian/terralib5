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
  \file MapController.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapController.h"
#include "ContextItem.h"
#include "AbstractItemFactory.h"
#include "Context.h"
#include "ItemModelObservable.h"
#include "ItemParamsCreate.h"
#include "ItemObserver.h"
#include "MapModel.h"

te::layout::MapController::MapController( Observable* o ) :
  ItemController(o, TPMapItem)
{
  AbstractItemFactory* factory = Context::getInstance()->getItemFactory(); 
  ItemParamsCreate params(this, m_model);
  m_view = (Observer*)factory->make(TPMapItem, params);
}

te::layout::MapController::~MapController()
{
	
}

void te::layout::MapController::setPosition( const double& x, const double& y )
{
  if(m_model)
  {
    ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
    if(model)
      return model->setPosition(x, y);
  }
}

void te::layout::MapController::refreshLayers( std::list<te::map::AbstractLayerPtr> layers )
{
  if(!m_model)
    return;

  ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
  if(!model)
    return;

  MapModel* mpModel = dynamic_cast<MapModel*>(model);
  if(!mpModel)
    return;

  mpModel->setLayers(layers);
}
