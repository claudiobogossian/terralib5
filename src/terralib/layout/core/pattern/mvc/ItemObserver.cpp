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
  m_nameClass("Unknown"),
  m_invertedMatrix(false)
{
  m_model->addObserver(this);
}

te::layout::ItemObserver::~ItemObserver()
{
  if(m_controller)
    delete (ItemController*)m_controller;
}

void te::layout::ItemObserver::redraw( bool bRefresh )
{
  if (bRefresh)
  {
    refresh();
  } 
  else
  {
    double x = 0;
    double y = 0;
    x = m_model->getBox().getLowerLeftX();
    y = m_model->getBox().getLowerLeftY();

    te::gm::Coord2D coord (x, y);

    ContextItem context;
    context.setPos(coord);
    context.setChangePos(true);
    updateObserver(context);
  }

  m_controller->redraw();
}

void te::layout::ItemObserver::refresh(bool pos)
{
  if(pos)
  {
    te::gm::Coord2D coord = getPosition();
    m_controller->setPosition(coord.x, coord.y);
  }
  setZValueItem(getZValueItem());
  applyRotation();
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

std::string te::layout::ItemObserver::getNameClass()
{
  return m_nameClass;
}

bool te::layout::ItemObserver::isInvertedMatrix()
{
  return m_invertedMatrix;
}

void te::layout::ItemObserver::changeZoomFactor( double currentZoomFactor )
{
  //do nothing
}

void te::layout::ItemObserver::recalculateBoundingRect()
{
  //do nothing
}


