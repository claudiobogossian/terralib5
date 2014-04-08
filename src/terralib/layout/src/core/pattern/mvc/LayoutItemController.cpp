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
  \file LayoutItemController.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LayoutItemController.h"
#include "LayoutItemModelObservable.h"
#include "LayoutItemObserver.h"
#include "ContextLayoutItem.h"
#include "LayoutContext.h"
#include "../../../../../maptools/Canvas.h"

te::layout::LayoutItemController::LayoutItemController( LayoutObservable* o )
{
  m_model = o;
}

te::layout::LayoutItemController::~LayoutItemController()
{
  if(m_model)
  {
    LayoutItemModelObservable* model = dynamic_cast<LayoutItemModelObservable*>(m_model);
    if(model)
      delete model;
    m_model = 0;
  }
}

void te::layout::LayoutItemController::redraw( const double& zoomFactor )
{
  LayoutParams params;
  ContextLayoutItem context(params, zoomFactor);

  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();
  canvas->clear();
  if(m_model)
  {
    LayoutItemModelObservable* model = dynamic_cast<LayoutItemModelObservable*>(m_model);
    if(model)
      model->draw(context);
  }
  canvas->clear();
}

const te::layout::LayoutObservable* te::layout::LayoutItemController::getModel()
{
  return m_model;
}

const te::layout::LayoutObserver* te::layout::LayoutItemController::getView()
{
  return m_view;
}

void te::layout::LayoutItemController::setBox( te::gm::Envelope box )
{
  if(m_model)
  {
    LayoutItemModelObservable* model = dynamic_cast<LayoutItemModelObservable*>(m_model);
    if(model)
      model->setBox(box);
  }
}

void te::layout::LayoutItemController::updateProperties( te::layout::LayoutProperties* properties )
{
  if(m_model)
  {
    LayoutItemModelObservable* model = dynamic_cast<LayoutItemModelObservable*>(m_model);
    if(model)
      model->updateProperties(properties);
  }
}

bool te::layout::LayoutItemController::contains( const te::gm::Coord2D &coord ) const
{
  if(m_model)
  {
    LayoutItemModelObservable* model = dynamic_cast<LayoutItemModelObservable*>(m_model);
    if(model)
      return model->contains(coord);
  }
  return false;
}
