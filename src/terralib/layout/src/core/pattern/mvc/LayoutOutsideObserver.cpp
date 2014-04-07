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
  \file LayoutOutsideObserver.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LayoutOutsideObserver.h"
#include "LayoutObservable.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "LayoutOutsideController.h"

te::layout::LayoutOutsideObserver::LayoutOutsideObserver(LayoutOutsideController* controller, LayoutObservable* o) :
  m_controller(controller),
  m_model(o)
{
	m_model->addObserver(this);
}

te::layout::LayoutOutsideObserver::~LayoutOutsideObserver()
{
	if(m_controller)
		delete (LayoutOutsideController*)m_controller;
}

void te::layout::LayoutOutsideObserver::refresh()
{
	te::gm::Coord2D coord = getPosition();
	m_controller->setPosition(coord.x, coord.y);
}

te::layout::LayoutProperties* te::layout::LayoutOutsideObserver::getProperties() const
{
  return m_model->getProperties();
}
