/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file GridPlanarItem.h
   
   \brief Class that represents a graphic GridMap. 
      Its coordinate system is the same of scene (millimeters). 
      This is also son of ItemObserver and ObjectItem, so it can become observer of a model (Observable). 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GRIDPLANAR_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_GRIDPLANAR_ITEM_H

// TerraLib
#include "GridMapItem.h"

// Qt
#include <QLineF>
#include <QRectF>
#include <QString>

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    /*!
    \brief Class that represents a graphic GridMap. 
        Its coordinate system is the same of scene (millimeters). 
        He is also the son of ItemObserver and ObjectItem, so it can become observer of a model (Observable). 
    
      \ingroup layout

      \sa te::layout::ObjectItem
    */
    class TELAYOUTEXPORT GridPlanarItem : public GridMapItem
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        GridPlanarItem(AbstractItemController* controller, bool invertedMatrix = false);

        /*!
          \brief Destructor
         */
        virtual ~GridPlanarItem();

        virtual void calculateGrid();
        
      protected:

        virtual void drawGrid(QPainter* painter);

        virtual void calculateVertical(const te::gm::Envelope& geoBox, const te::gm::Envelope& boxMM);

        virtual void calculateHorizontal(const te::gm::Envelope& geoBox, const te::gm::Envelope& boxMM);

        virtual double initVerticalLines(const te::gm::Envelope& geoBox);

        virtual double initHorizontalLines(const te::gm::Envelope& geoBox);
    };
  }
}

#endif
