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
  \file OutsideObserver.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "OutsideObserver.h"
#include "Observable.h"
#include "../singleton/Context.h"
#include "../../AbstractScene.h"
#include "OutsideController.h"

te::layout::OutsideObserver::OutsideObserver(OutsideController* controller, Observable* o) :
  m_controller(controller),
  m_model(o)
{
	m_model->addObserver(this);
}

te::layout::OutsideObserver::~OutsideObserver()
{
	if(m_controller)
		delete (OutsideController*)m_controller;
}

void te::layout::OutsideObserver::refresh()
{
	te::gm::Coord2D coord = getPosition();
	m_controller->setPosition(coord.x, coord.y);
}

te::layout::Properties* te::layout::OutsideObserver::getProperties() const
{
  return m_model->getProperties();
}

te::layout::OutsideController* te::layout::OutsideObserver::getController()
{
  return m_controller;
}

te::layout::Observable* te::layout::OutsideObserver::getModel()
{
  return m_model;
}
