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
    class EnumType;
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
          \brief Method that adds new zoom factor. Ex.: 50 - 50%.
      
      \param factor factor  
      \param text percentage
        */
        virtual void addZoom(int zoom, std::string text);

        /*!
          \brief Method that remove a zoom factor. Ex.: 50 - 50%.
      
          \param factor zoom factor  
        */
        virtual void removeZoom(int zoom);

    /*!
          \brief Method that clears the zoom list.    
        */
        virtual void clearZoomList();

    /*!
          \brief Change the current factor with the next zoom factor in the list.
        */
        virtual int nextZoom();

    /*!
          \brief Change the current factor with the previous zoom factor in the list.    
        */
        virtual int previousZoom();

        int getCurrentZoom();

        int getOldZoom();

        void setDefaultZoom(int zoom);

        int getDefaultZoom();

        void setMaxZoomLimit(int zoom);

        int getMaxZoomLimit();

        void setMinZoomLimit(int zoom);

        int getMinZoomLimit();

        /*!
          \brief Sets the zoom of the View to the given value. Change the current zoom.
        */
        virtual void setZoom(int zoom) = 0;

        /*!
        \brief Change value of the enumeration of mode type. Ex.: printing.
          The type of mode is used by the context to know what should be done. 
          The mode in context could be modified by the user interaction.
        */
        EnumType* getCurrentMode();

        EnumType* getOldMode();

      protected:

        virtual void applyScale(double newScale) = 0;

        virtual bool isLimitExceeded(int newZoom);

        virtual void setCurrentZoom(int zoom);

        virtual void setCurrentMode(EnumType* mode);

      protected:

        bool                          m_visibleRulers; //!< rulers visibility state
        std::map<int, std::string>    m_zooms; //!< zoom factor list

      private:
        
        int                           m_currentZoom; //!< current zoom
        int                           m_defaultZoom; //!< default zoom
        int                           m_oldZoom; //!< old zoom
        int                           m_maxZoomLimit;
        int                           m_minZoomLimit;
        EnumType*                     m_currentMode; //!< current value of the enumeration of mode type
        EnumType*                     m_oldMode; //!< old value of the enumeration of mode type
    };
  }
}

#endif
