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
  \file ContextItem.h
   
  \brief Class responsible for maintaining the drawing context of a graphic component.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_CONTEXT_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_CONTEXT_ITEM_H

// TerraLib
#include "ParamsCreate.h"
#include "../../color/RGBAColor.h"
#include "Utils.h"
#include "../../maptools/Canvas.h"
#include "Config.h"
#include "../../geometry/Coord2D.h"

namespace te
{
  namespace layout
  {
    /*!
      \brief Class responsible for maintaining the drawing context of a MVC component.
       It is always used by the "Model" part of the MVC in draw method, as well as the part "View" in updateObserver method.
    
      \ingroup layout
    */
    class TELAYOUTEXPORT ContextItem 
    {
      public:

        /*!
          \brief Constructor
        */ 
        ContextItem();

        /*!
          \brief Constructor

          \param params
          \param zoom zoom
        */ 
        ContextItem(ParamsCreate params, int zoom);

        /*!
          \brief Destructor
        */ 
        virtual ~ContextItem();

        /*!
         \brief 
        */
        virtual void setParams(ParamsCreate params);
        
        /*!
         \brief 
        */
        virtual ParamsCreate getParams();

        /*!
         \brief Change current zoom.

         \param zoom current zoom
        */
        virtual void setZoom(int zoom);
        
        /*!
         \brief Returns current zoom.

         \return current zoom
        */
        virtual int getZoom();

        /*!
        \brief Stores pixmap generated after drawing.

        \param pixmap generated after drawing
        */
        virtual void setPixmap(te::color::RGBAColor** pixmap);
        
        /*!
         \brief Returns pixmap generated after drawing.

         \return pixmap
        */
        virtual te::color::RGBAColor** getPixmap();
        
        /*!
         \brief Returns whether the canvas should or should not be resized.

         \return true if canvas should be resized, false otherwise
        */
        virtual bool isResizeCanvas();
        
        /*!
         \brief Change whether the canvas should or should not be resized.

         \param clear true if canvas should be resized, false otherwise.
        */
        virtual void setResizeCanvas(bool clear);

        /*!
         \brief Return canvas.

         \return canvas
        */
        virtual te::map::Canvas* getCanvas();

        /*!
         \brief Change canvas.

         \param canvas
        */
        virtual void setCanvas(te::map::Canvas* canvas);

        /*!
         \brief 
        */
        virtual Utils* getUtils();

        /*!
         \brief 
        */
        virtual void setUtils(Utils* utils);

        /*!
         \brief 
        */
        virtual double getDpiX();

        /*!
         \brief 
        */
        virtual void setDpiX(double dpi);

        /*!
         \brief 
        */
        virtual double getDpiY();

        /*!
         \brief 
        */
        virtual void setDpiY(double dpi);

        /*
         \brief 
        */
        virtual void setShow( bool show );
        
        /*!
         \brief 
        */
        virtual bool isShow();

        void setPos (te::gm::Coord2D pos);

        te::gm::Coord2D getPos ();

        void setChangePos (bool change);

        bool isChangePos ();

      protected:
        ParamsCreate            m_params; //!<
        int                     m_zoom; //!<
        te::color::RGBAColor**  m_pixmap; //!<
        bool                    m_resizeCanvas; //!<
        bool                    m_show; //!<
        Utils*                  m_utils; //!<
        te::map::Canvas*        m_canvas; //!<
        double                  m_dpiX; //!<
        double                  m_dpiY; //!<
        te::gm::Coord2D         m_pos;
        bool                    m_changePos;
    };
  }
}

#endif
