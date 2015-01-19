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
  \file ArrowModel.h
   
   \brief Class that represents a "Model" part of Arrow MVC component. 
      Its coordinate system is the same of scene (millimeters). 
      This is also son of ItemModelObservable, so it can become observable.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ARROW_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_ARROW_MODEL_H

// TerraLib
#include "../core/pattern/mvc/ItemModelObservable.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"

namespace te
{
  namespace layout
  {
    /*!
      \brief Class that represents a "Model" part of Arrow MVC component. 
          Its coordinate system is the same of scene (millimeters). 
          This is also son of ItemModelObservable, so it can become observable.
      	  
	    \ingroup layout

      \sa te::layout::ItemModelObservable
	  */
    class ArrowModel : public ItemModelObservable
    {
      public:
           
        /*!
          \brief Constructor
        */
        ArrowModel();

        /*!
          \brief Destructor
        */ 
        virtual ~ArrowModel();

        /*!
          \brief Reimplemented from ItemModelObservable
         */
        virtual void draw( ContextItem context );

      protected:

        /*!
          \brief Drawing method of a Arrow

          \param canvas
          \param utils
         */
        virtual void drawArrow(te::map::Canvas* canvas, Utils* utils);

        /*!
          \brief Drawing method of a Head Arrow

          \param canvas
          \param utils
          \param box
         */
        virtual void drawHeadArrow(te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box);
    };
  }
}

#endif 
