/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file LayoutItemModelObservable.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LAYOUT_ITEM_MODEL_H 
#define __TERRALIB_LAYOUT_INTERNAL_LAYOUT_ITEM_MODEL_H

// TerraLib
#include "LayoutObservable.h"

// STL
#include <set>

namespace te
{
  namespace layout
  {
    class LayoutObserver;

    class LayoutItemModelObservable : public LayoutObservable
    {
      public:
        LayoutItemModelObservable();
        virtual ~LayoutItemModelObservable();

        virtual bool addObserver(LayoutObserver* o);
        virtual bool removeObserver(LayoutObserver* o);
        virtual LayoutProperties* getProperties() const;

        virtual LayoutAbstractObjectType getType();

        virtual te::gm::Envelope getBox();
        virtual void setBox(te::gm::Envelope box);

        virtual void setBackgroundColor(te::color::RGBAColor color);
        virtual te::color::RGBAColor getBackgroundColor();
        virtual void setBorderColor(te::color::RGBAColor color);
        virtual te::color::RGBAColor getBorderColor();

        virtual void setName(std::string name);
        virtual std::string getName();

        virtual void setPosition(const double& x, const double& y);
        virtual void draw( ContextLayoutItem context ) = 0;

        virtual bool contains(const te::gm::Coord2D &coord) const;

        virtual void updateProperties(te::layout::LayoutProperties* properties);

        virtual int getZValue();

        virtual void setZValue(int zValue);
                
      protected:
        virtual void notifyAll(ContextLayoutItem context);

      protected:
        std::set<LayoutObserver*>	m_observers;
        int							          m_id;
        te::gm::Envelope					m_box;
        te::gm::Coord2D			      m_centerCoordinate;
        te::color::RGBAColor			m_backgroundColor;
        te::color::RGBAColor			m_borderColor;
        LayoutProperties*         m_properties;
        LayoutAbstractObjectType  m_type;
        int                       m_zValue;

      private:
        std::string m_name;
    };
  }
}

#endif