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
  \file WaitView.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_WAIT_VIEW_H
#define __TERRALIB_LAYOUT_INTERNAL_WAIT_VIEW_H

//TerraLib
#include "../../core/Config.h"
#include "../../../geometry/Point.h"

//STL
#include <vector>

//Qt
#include <QVector>
#include <QPoint>
#include <QPointF>

namespace te
{
  namespace layout
  {
    class View;

    class TELAYOUTEXPORT WaitView 
    {
      public:

        WaitView ();

        WaitView(View *view);

        virtual ~WaitView();

        virtual void addCoord ( QPointF point);

        virtual void clear ();

        virtual QVector<QPoint> getCoords ();

        virtual std::vector<te::gm::Point*> getCoordsW ();

      protected:

        std::vector<te::gm::Point*> m_coords;
        QVector<QPoint> m_points;
        View *m_view;
               
    };
  }
}

#endif