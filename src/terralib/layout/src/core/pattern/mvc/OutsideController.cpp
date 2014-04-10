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
  \file OutsideController.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "OutsideController.h"
#include "OutsideObserver.h"
#include "OutsideModelObservable.h"

te::layout::OutsideController::OutsideController( Observable* o )
{
	m_model = o;
}

te::layout::OutsideController::~OutsideController()
{
  if(m_model)
  {
    OutsideModelObservable* model = dynamic_cast<OutsideModelObservable*>(m_model);
    if(model)
      delete model;
    m_model = 0;
  }
}

const te::layout::Observable* te::layout::OutsideController::getModel()
{
	return m_model;
}

const te::layout::Observer* te::layout::OutsideController::getView()
{
	return m_view;
}

