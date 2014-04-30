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
  \file OutsideObserver.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_OBSERVER_H 
#define __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_OBSERVER_H

// TerraLib
#include "Observer.h"
#include "Properties.h"
#include "ContextItem.h"
#include "../../../../../geometry/Coord2D.h"

namespace te
{
  namespace layout
  {
    class OutsideController;
    class Observable;

    class OutsideObserver : public Observer
    {
    public:
      OutsideObserver(OutsideController* controller, Observable* o);
      virtual ~OutsideObserver();
      virtual void updateObserver(ContextItem context) = 0;
      virtual void setPosition(const double& x, const double& y) = 0;
      virtual te::gm::Coord2D getPosition() = 0;
      virtual void refresh();
      virtual Properties* getProperties() const;
      virtual OutsideController* getController();
      virtual Observable* getModel();

    protected:
      OutsideController*	m_controller;
      Observable*		m_model;
    };
  }
}

#endif