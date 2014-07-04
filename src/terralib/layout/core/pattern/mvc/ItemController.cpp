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

te::layout::ItemController::ItemController( Observable* o ) :
  m_model(o)
{

}

te::layout::ItemController::ItemController( Observable* o, LayoutAbstractObjectType type ) :
  m_model(o)
{
  if(m_model)
  {
    m_model->setType(type);
  }
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
  double zoomFactor = Context::getInstance()->getZoomFactor();
  ContextItem contxt(params, zoomFactor);

  te::map::Canvas* canvas = Context::getInstance()->getCanvas();
  Utils* utils = Context::getInstance()->getUtils();
  double dpiX = Context::getInstance()->getDpiX();
  double dpiY = Context::getInstance()->getDpiY();

  contxt.setCanvas(canvas);
  contxt.setUtils(utils);
  contxt.setDpiX(dpiX);
  contxt.setDpiY(dpiY);

  canvas->clear();
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

void te::layout::ItemController::updateProperties( te::layout::Properties* properties )
{
  if(m_model)
  {
    ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
    if(model)
      model->updateProperties(properties);
  }
}

bool te::layout::ItemController::contains( const te::gm::Coord2D &coord ) const
{
  if(m_model)
  {
    ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
    if(model)
      return model->contains(coord);
  }
  return false;
}
