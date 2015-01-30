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
  \file TitleController.h
   
   \brief Class that represents a "Controller" part of Title MVC component. 
   Its coordinate system is the same of scene (millimeters).
   This is also son of ItemController, so it can become a controller.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TITLE_CONTROLLER_H 
#define __TERRALIB_LAYOUT_INTERNAL_TITLE_CONTROLLER_H

// TerraLib
#include "TextController.h"
#include "../core/Config.h"

namespace te
{
  namespace layout
  {
    /*!
      \brief Class that represents a "Controller" part of Title MVC component. 
          Its coordinate system is the same of scene (millimeters). 
          This is also the son of ItemController, so it can become a controller.
      	  
	    \ingroup layout

      \sa te::layout::TextController
	  */
    class TELAYOUTEXPORT TitleController : public TextController
    {
      public:

        /*!
          \brief Constructor

          \param o "Model" part of MVC component
        */
        TitleController( Observable* o );

        /*!
          \brief Destructor
        */
        virtual ~TitleController();

        /*!
          \brief Reimplemented from TextController
         */
        virtual void setPosition(const double& x, const double& y);

      protected:

        /*!
          \brief Constructor.

          \param o "Model" part of MVC component
          \param type type of the MVC component
        */
        TitleController( Observable* o, EnumType* type );

        /*!
          \brief Reimplemented from ItemController
         */
        virtual void create();
    };
  }
}

#endif
