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
  \file ItemObserver.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemObserver.h"
#include "Context.h"
#include "AbstractScene.h"
#include "Properties.h"
#include "ItemController.h"
#include "Observable.h"
#include "../../../../../geometry/Coord2D.h"

te::layout::ItemObserver::ItemObserver(ItemController* controller, Observable* o) :
  m_controller(controller),
  m_model(o),
  m_printable(true),
  m_ppi(96)
{
  m_model->addObserver(this);
}

te::layout::ItemObserver::~ItemObserver()
{
  if(m_controller)
    delete (ItemController*)m_controller;
}

te::layout::Properties* te::layout::ItemObserver::getProperties() const
{
  return m_model->getProperties();
}

void te::layout::ItemObserver::redraw( const double& scaleFactor /*= 1.*/ )
{
  refresh();
  m_controller->redraw(scaleFactor);
}

void te::layout::ItemObserver::refresh()
{
  te::gm::Coord2D coord = getPosition();
  m_controller->setPosition(coord.x, coord.y);
}

void te::layout::ItemObserver::setItemPosition( const double& x, const double& y )
{
  setPosition(x, y);
  refresh();
}

void te::layout::ItemObserver::setPPI( const double& ppi )
{
  m_ppi = ppi;
}

void te::layout::ItemObserver::updateProperties( te::layout::Properties* properties )
{
  if(m_controller)
    m_controller->updateProperties(properties);
}

void te::layout::ItemObserver::setPrintable( bool print )
{
  m_printable = print;
}

bool te::layout::ItemObserver::isPrintable()
{
  return m_printable;
}
