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
  \file ContextItem.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_CONTEXT_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_CONTEXT_ITEM_H

// TerraLib
#include "ParamsCreate.h"
#include "../../../color/RGBAColor.h"

namespace te
{
  namespace layout
  {
    class LayoutPixmap;

    class ContextItem 
    {
      public:

        /*!
         \brief 
        */
        ContextItem();

        /*!
         \brief 
        */
        ContextItem(ParamsCreate params, double zoomFactor);

        /*!
         \brief 
        */
        virtual ~ContextItem();

        /*!
         \brief 
        */
        void setParams(ParamsCreate params);
        
        /*!
         \brief 
        */
        ParamsCreate getParams();

        /*!
         \brief 
        */
        void setZoomFactor(double zoomFactor);
        
        /*!
         \brief 
        */
        double getZoomFactor();

        /*!
         \brief 
        */
        void setPixmap(te::color::RGBAColor** pixmap);
        
        /*!
         \brief 
        */
        te::color::RGBAColor** getPixmap();

        /*!
         \brief 
        */
        void setShow( bool show );
        
        /*!
         \brief 
        */
        bool getShow();

        /*!
         \brief 
        */
        void setWait(bool wait);
        
        /*!
         \brief 
        */
        bool getWait();

        /*!
         \brief 
        */
        bool isResizeCanvas();
        
        /*!
         \brief 
        */
        void setResizeCanvas(bool clear);

      protected:
        ParamsCreate            m_params; //!<
        double                  m_zoomFactor; //!<
        te::color::RGBAColor**  m_pixmap; //!<
        bool		                m_show; //!<
        bool		                m_wait; //!<
        bool                    m_resizeCanvas; //!<
    };
  }
}

#endif