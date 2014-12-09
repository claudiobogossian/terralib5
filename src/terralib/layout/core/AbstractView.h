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
  \file AbstractView.h
   
  \brief This is the abstract view for View.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_VIEW_H 
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_VIEW_H

// TerraLib
#include "Config.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace layout
  {
  /*!
      \class AbstractView

      \brief This is the abstract view for View.

      \ingroup layout
     */
    class TELAYOUTEXPORT AbstractView 
    {
      public:
        
		/*!
          \brief Constructor
        */ 
        AbstractView();

		/*!
          \brief Destructor
        */ 
        virtual ~AbstractView(void){}
        
		/*!
          \brief Method that return rulers visibility state.
		  
		  \return true if visible, false otherwise	  
        */
        virtual bool isVisibleRulers();

		/*!
          \brief Method that change rulers visibility state.
		  
		  \param visible visibility state	  
        */
        virtual void setVisibleRulers(bool visible);

		/*!
          \brief Initialization method that configures the View and sets the transformation matrix of the scene in the View object.	  
        */
        virtual void config() = 0; 

		/*!
          \brief Method that adds new zoom factor. Ex.: 0.5 - 50%.
		  
		  \param factor factor  
		  \param text percentage
        */
        virtual void addZoomFactor(double factor, std::string text);

		/*!
          \brief Method that clears the zoom factor list.	  
        */
        virtual void clearZoomFactors();

		/*!
          \brief Method that returns the next zoom factor in the list.
		  
		  \param currentFactor current factor	  
        */
        virtual double nextFactor(double currentFactor);

		/*!
          \brief Method that returns the previous zoom factor in the list.
		  
		  \param currentFactor current factor	  
        */
        virtual double previousFactor(double currentFactor);

      protected:

        bool                          m_visibleRulers; //!< rulers visibility state
        std::map<double, std::string> m_zoomFactors; //!< zoom factor list
    };
  }
}

#endif
