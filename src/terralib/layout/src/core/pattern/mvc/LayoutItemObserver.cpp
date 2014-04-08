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
  \file LayoutItemObserver.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LayoutItemObserver.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "LayoutProperties.h"
#include "LayoutItemController.h"
#include "LayoutObservable.h"
#include "../../../../../geometry/Coord2D.h"

te::layout::LayoutItemObserver::LayoutItemObserver(LayoutItemController* controller, LayoutObservable* o) :
  m_controller(controller),
  m_model(o),
  m_printable(true),
  m_ppi(96)
{
  m_model->addObserver(this);
}

te::layout::LayoutItemObserver::~LayoutItemObserver()
{
  if(m_controller)
    delete (LayoutItemController*)m_controller;
}

te::layout::LayoutProperties* te::layout::LayoutItemObserver::getProperties() const
{
  return m_model->getProperties();
}

void te::layout::LayoutItemObserver::redraw( const double& scaleFactor /*= 1.*/ )
{
  refresh();
  m_controller->redraw(scaleFactor);
}

void te::layout::LayoutItemObserver::refresh()
{
  te::gm::Coord2D coord = getPosition();
  m_controller->setPosition(coord.x, coord.y);
}

void te::layout::LayoutItemObserver::setItemPosition( const double& x, const double& y )
{
  setPosition(x, y);
  refresh();
}

void te::layout::LayoutItemObserver::setPPI( const double& ppi )
{
  m_ppi = ppi;
}

void te::layout::LayoutItemObserver::updateProperties( te::layout::LayoutProperties* properties )
{
  if(m_controller)
    m_controller->updateProperties(properties);
}

void te::layout::LayoutItemObserver::setPrintable( bool print )
{
  m_printable = print;
}

bool te::layout::LayoutItemObserver::isPrintable()
{
  return m_printable;
}
