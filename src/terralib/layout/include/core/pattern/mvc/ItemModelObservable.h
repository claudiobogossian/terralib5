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
  \file ItemModelObservable.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_MODEL_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_MODEL_H

// TerraLib
#include "Observable.h"

// STL
#include <set>

namespace te
{
  namespace layout
  {
    class Observer;
    class SharedProperties;

    class ItemModelObservable : public Observable
    {
      public:
        ItemModelObservable();
        virtual ~ItemModelObservable();

        virtual bool addObserver(Observer* o);
        virtual bool removeObserver(Observer* o);
        virtual Properties* getProperties() const;

        virtual LayoutAbstractObjectType getType();
        virtual void setType(LayoutAbstractObjectType type);

        virtual te::gm::Envelope boxWithZoomFactor(Utils* utils);
        virtual te::gm::Envelope getBox();
        virtual void setBox(te::gm::Envelope box);

        virtual void setBackgroundColor(te::color::RGBAColor color);
        virtual te::color::RGBAColor getBackgroundColor();
        virtual void setBorderColor(te::color::RGBAColor color);
        virtual te::color::RGBAColor getBorderColor();

        virtual void setName(std::string name);
        virtual std::string getName();

        virtual int getId();
        virtual void setId(int id);

        virtual void setPosition(const double& x, const double& y);
        virtual void draw( ContextItem context ) = 0;

        virtual bool contains(const te::gm::Coord2D &coord) const;

        virtual void updateProperties(te::layout::Properties* properties);

        virtual int getZValue();

        virtual void setZValue(int zValue);

        virtual bool isBorder();

        virtual void setBorder(bool value);

        /* Resize Box */
        virtual void resize(double w, double h);
                
      protected:
        virtual void notifyAll(ContextItem context);

      protected:
        std::set<Observer*>	      m_observers;
        int							          m_id;
        te::gm::Envelope					m_box;
        te::gm::Coord2D			      m_centerCoordinate;
        te::color::RGBAColor			m_backgroundColor;
        te::color::RGBAColor			m_borderColor;
        Properties*               m_properties;
        LayoutAbstractObjectType  m_type;
        int                       m_zValue;
        SharedProperties*         m_sharedProps;
        bool                      m_border;

      private:
        std::string m_name;
    };
  }
}

#endif