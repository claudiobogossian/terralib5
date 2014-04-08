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
  \file LayoutItemController.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LAYOUT_ITEM_CONTROLLER_H 
#define __TERRALIB_LAYOUT_INTERNAL_LAYOUT_ITEM_CONTROLLER_H

// TerraLib
#include "../../../../../geometry/Envelope.h"
#include "../../../../../geometry/Coord2D.h"
#include "../../../color/RGBAColor.h"

namespace te
{
  namespace layout
  {
    class LayoutObservable;
    class LayoutObserver;
    class LayoutProperties;

    class LayoutItemController 
    {
      public:

        LayoutItemController(LayoutObservable* o);
        virtual ~LayoutItemController();

        virtual void setPosition(const double& x, const double& y) = 0;
        virtual void redraw(const double& zoomFactor);
        const LayoutObservable* getModel();
        const LayoutObserver* getView();
        virtual void setBox(te::gm::Envelope box);
        virtual void updateProperties(te::layout::LayoutProperties* properties);
        virtual bool contains(const te::gm::Coord2D &coord) const;

      protected:
        LayoutObservable* m_model;
        LayoutObserver* m_view;
    };
  }
}

#endif