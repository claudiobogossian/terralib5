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
  \file OutsideModelObservable.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_MODEL_H 
#define __TERRALIB_LAYOUT_INTERNAL_OUTSIDE_MODEL_H

// TerraLib
#include "Observable.h"
#include "../../property/Properties.h"
#include "../../ContextItem.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../geometry/Coord2D.h"
#include "../../Config.h"

// STL
#include <set>

namespace te
{
  namespace layout
  {
    class Observer;

    class TELAYOUTEXPORT OutsideModelObservable : public Observable
    {
    public:

      OutsideModelObservable();
      virtual ~OutsideModelObservable();

      virtual bool addObserver(Observer* o);
      virtual bool removeObserver(Observer* o);

      virtual te::gm::Envelope getBox();
      virtual void setBox(te::gm::Envelope box);

      virtual int getColor();
      virtual void setColor(int color);

      virtual void setPosition(const double& x, const double& y);
      virtual EnumType* getType();
      virtual void setType(EnumType* type);

      virtual std::string getName();

      virtual int getZValue();
      virtual void setZValue(int zValue);

      virtual void updateProperties(te::layout::Properties* properties);

      virtual Properties* getProperties() const;

      virtual int getId();

      virtual void setId( int id );

      virtual void setResizable(bool resize);

      virtual bool isResizable();

    protected:

      virtual void notifyAll(ContextItem context);

    protected:

      std::set<Observer*>	m_observers;
      te::gm::Envelope		m_box;
      te::gm::Coord2D 		m_centerCoordinate;
      int							    m_color;
      Properties*         m_properties;
      EnumType*           m_type;
      int                 m_zValue;
      int                 m_id;
      bool                m_resizable;

    private:
      std::string m_name;
    };
  }
}

#endif