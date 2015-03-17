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
  \file ArrowItem.h
   
   \brief Class that represents a graphic Arrow. 
      Its coordinate system is the same of scene (millimeters). 
      This class is also son of ItemObserver and ObjectItem, so it can become observer of a model (Observable). 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ARROW_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_ARROW_ITEM_H

// TerraLib
#include "ObjectItem.h"
#include "../../core/Config.h"

namespace te
{
  namespace layout
  {
    class Observable;

    /*!
    \brief Class that represents a graphic Arrow. 
        Its coordinate system is the same of scene (millimeters). 
        He is also the son of ItemObserver and ObjectItem, so it can become observer of a model (Observable). 
	  
	    \ingroup layout

	    \sa te::layout::ObjectItem
	  */
    class TELAYOUTEXPORT ArrowItem : public ObjectItem
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        ArrowItem( ItemController* controller, Observable* o );
        
        /*!
          \brief Destructor
         */
        virtual ~ArrowItem();
        
        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

       protected:

        /*!
          \brief Drawing method of a Arrow

          \param canvas
          \param utils
         */
        virtual void drawArrow(QPainter * painter);

        /*!
          \brief Drawing method of a Head Arrow

          \param canvas
          \param utils
          \param box
         */
        virtual void drawHeadArrow(QPainter * painter);
    };
  }
}

#endif
