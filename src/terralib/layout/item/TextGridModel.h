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
  \file TextGridModel.h
   
   \brief Class that represents a "Model" part of TextGrid MVC component.  
   Its coordinate system is the same of scene (millimeters). 
   This is also son of ItemModelObservable, so it can become observable.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEXT_GRID_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_TEXT_GRID_MODEL_H

// TerraLib
#include "TitleModel.h"
#include "../../geometry/Point.h"
#include "../core/Config.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace layout
  {
    /*!
    \brief Class that represents a "Model" part of TextGrid MVC component.  
    Its coordinate system is the same of scene (millimeters). 
    This is also son of ItemModelObservable, so it can become observable.
      	  
	    \ingroup layout

      \sa te::layout::TitleModel
	  */
    class TELAYOUTEXPORT TextGridModel : public TitleModel
    {
      public:

        /*!
          \brief Constructor
        */
        TextGridModel();

        /*!
          \brief Destructor
        */ 
        virtual ~TextGridModel();

        virtual void addText(te::gm::Point* cell, std::string text);

        virtual void removeCell(te::gm::Point* cell);
        
        virtual std::map<te::gm::Point*, std::string> getTextCells();

        virtual void clearTextCells();

      protected:

        std::map<te::gm::Point*, std::string> m_textCells;
    };
  }
}

#endif
