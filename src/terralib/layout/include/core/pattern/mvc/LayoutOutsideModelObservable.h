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
  \file LayoutOutsideModelObservable.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LAYOUT_OUTSIDE_MODEL_H 
#define __TERRALIB_LAYOUT_INTERNAL_LAYOUT_OUTSIDE_MODEL_H

// TerraLib
#include "LayoutObservable.h"
#include "LayoutProperties.h"
#include "ContextLayoutItem.h"
#include "../../../../../geometry/Envelope.h"
#include "../../../../../geometry/Coord2D.h"

// STL
#include <set>

namespace te
{
  namespace layout
  {
    class LayoutObserver;

    class LayoutOutsideModelObservable : public LayoutObservable
    {
    public:

      LayoutOutsideModelObservable();
      virtual ~LayoutOutsideModelObservable();

      virtual bool addObserver(LayoutObserver* o);
      virtual bool removeObserver(LayoutObserver* o);
      virtual te::gm::Envelope getBox();
      virtual void setBox(te::gm::Envelope box);
      virtual int getColor();
      virtual void setColor(int color);

      virtual void setPosition(const double& x, const double& y);
      virtual LayoutAbstractObjectType getType();

      virtual int getZValue();
      virtual void setZValue(int zValue);

      virtual void updateProperties(te::layout::LayoutProperties* properties);

    protected:

      virtual void notifyAll(ContextLayoutItem context);

      virtual LayoutProperties* getProperties() const;


    protected:
      std::set<LayoutObserver*>	m_observers;
      te::gm::Envelope					m_box;
      te::gm::Coord2D 					m_centerCoordinate;
      int							          m_color;
      LayoutProperties*         m_properties;
      LayoutAbstractObjectType  m_type;
      int                       m_zValue;
    };
  }
}

#endif