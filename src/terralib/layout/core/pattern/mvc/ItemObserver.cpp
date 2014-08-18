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
#include "../singleton/Context.h"
#include "../../AbstractScene.h"
#include "../../property/Properties.h"
#include "ItemController.h"
#include "Observable.h"
#include "../../../../geometry/Coord2D.h"

te::layout::ItemObserver::ItemObserver(ItemController* controller, Observable* o) :
  m_controller(controller),
  m_model(o),
  m_printable(true),
  m_canChangeGraphicOrder(true),
  m_canZoom(true),
  m_nameClass("Unknown")
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

void te::layout::ItemObserver::redraw()
{
  refresh();
  m_controller->redraw();
}

void te::layout::ItemObserver::refresh()
{
  te::gm::Coord2D coord = getPosition();
  m_controller->setPosition(coord.x, coord.y);
  setZValueItem(getZValueItem());
}

void te::layout::ItemObserver::updateProperties( te::layout::Properties* properties )
{
  if(m_controller)
  {
    m_controller->updateProperties(properties);
    redraw();
  }
}

void te::layout::ItemObserver::setPrintable( bool print )
{
  m_printable = print;
}

bool te::layout::ItemObserver::isPrintable()
{
  return m_printable;
}

te::layout::ItemController* te::layout::ItemObserver::getController()
{
  return m_controller;
}

te::layout::Observable* te::layout::ItemObserver::getModel()
{
  return m_model;
}

std::string te::layout::ItemObserver::getName()
{
  return m_model->getName();
}

void te::layout::ItemObserver::setZValueItem( int z )
{
  m_model->setZValue(z);
}

void te::layout::ItemObserver::setCanChangeGraphicOrder( bool canChange )
{
  m_canChangeGraphicOrder = canChange;
}

bool te::layout::ItemObserver::isCanChangeGraphicOrder()
{
  return m_canChangeGraphicOrder;
}

bool te::layout::ItemObserver::isCanZoom()
{
  return m_canZoom;
}

void te::layout::ItemObserver::setCanZoom( bool zoom )
{
  m_canZoom = zoom;
}

std::string te::layout::ItemObserver::getNameClass()
{
  return m_nameClass;
}
