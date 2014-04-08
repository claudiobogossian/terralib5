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
  \file LayoutObservable.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OBSERVABLE_H
#define __TERRALIB_LAYOUT_INTERNAL_OBSERVABLE_H

// TerraLib
#include "ContextLayoutItem.h"
#include "../../../../../geometry/Envelope.h"
#include "../../../../../geometry/Coord2D.h"
#include "../../../color/RGBAColor.h"
#include "LayoutAbstractType.h"

namespace te
{
  namespace layout
  {
    class LayoutObserver; 
    class LayoutProperties;

    class LayoutObservable 
    {
      public:

        virtual ~LayoutObservable(void) {}
        virtual bool addObserver(LayoutObserver* o) = 0;
        virtual bool removeObserver(LayoutObserver* o) = 0;
        virtual te::layout::LayoutProperties* getProperties() const = 0;
        virtual void updateProperties(te::layout::LayoutProperties* properties) = 0;
        virtual te::gm::Envelope getBox() = 0;
        virtual LayoutAbstractObjectType getType() = 0;
        virtual int getZValue() = 0;
        virtual void setZValue(int zValue) = 0;

      protected:
        virtual void notifyAll(ContextLayoutItem context) = 0;
    };

  }
}

#endif