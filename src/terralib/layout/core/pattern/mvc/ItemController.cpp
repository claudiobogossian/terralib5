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
  \file ItemController.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemController.h"
#include "ItemModelObservable.h"
#include "ItemObserver.h"
#include "../../ContextItem.h"
#include "../singleton/Context.h"
#include "../../../../maptools/Canvas.h"
#include "../factory/ItemParamsCreate.h"
#include "../factory/AbstractItemFactory.h"
#include "../../../qt/core/ContextObject.h"
#include "../../AbstractScene.h"
#include "Observer.h"

te::layout::ItemController::ItemController( Observable* o ) :
  m_model(o)
{
  create();
}

te::layout::ItemController::~ItemController()
{
  if(m_model)
  {
    ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
    if(model)
      delete model;
    m_model = 0;
  }
}

void te::layout::ItemController::redraw()
{
  if(!m_model)
    return;

  ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
  if(!model)
    return;

  ParamsCreate params;
  AbstractScene* sc = Context::getInstance().getScene();
  ContextObject context = sc->getContext();

  int zoom = context.getZoom();
  ContextItem contxt(params, zoom);

  te::map::Canvas* canvas = Context::getInstance().getCanvas();
  Utils* utils = Context::getInstance().getUtils();
  double dpiX = context.getDpiX();
  double dpiY = context.getDpiY();

  contxt.setCanvas(canvas);
  contxt.setUtils(utils);
  contxt.setDpiX(dpiX);
  contxt.setDpiY(dpiY);

  utils->resetCanvas();
  model->draw(contxt);
  canvas->clear();
}

const te::layout::Observable* te::layout::ItemController::getModel()
{
  return m_model;
}

const te::layout::Observer* te::layout::ItemController::getView()
{
  return m_view;
}

void te::layout::ItemController::setBox( te::gm::Envelope box )
{
  if(m_model)
  {
    ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
    if(model)
      model->setBox(box);
  }
}

bool te::layout::ItemController::contains( const te::gm::Coord2D &coord ) const
{
  if(m_model)
  {
    ItemObserver* iOb = (ItemObserver*)m_view;
    if(iOb)
    {
      iOb->refresh(); /* Update the position of the object */
    }

    ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
    if(model)
      return model->contains(coord);
  }
  return false;
}

void te::layout::ItemController::create()
{
  AbstractItemFactory* factory = Context::getInstance().getItemFactory(); 
  ItemParamsCreate params(this, m_model);
  m_view = (Observer*)factory->make(m_model->getType(), params);
}

void te::layout::ItemController::setPosition( const double& x, const double& y )
{
  if(m_model)
  {
    ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
    if(model)
      return model->setPosition(x, y);
  }
}
