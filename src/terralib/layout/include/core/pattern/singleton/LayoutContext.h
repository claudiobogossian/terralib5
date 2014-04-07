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
  \file LayoutContext.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LAYOUT_CONTEXT_H 
#define __TERRALIB_LAYOUT_INTERNAL_LAYOUT_CONTEXT_H

// TerraLib
#include "LayoutMode.h"
#include "LayoutAbstractType.h"
#include "LayoutUtils.h"
#include "LayoutAbstractOutsideFactory.h"
#include "../../../../../maptools/Canvas.h"
#include "../../../../../geometry/Envelope.h"

namespace te
{
  namespace layout
  {
    class LayoutScene;
    class LayoutAbstractItemFactory;
    class LayoutAbstractOutsideFactory;
    class LayoutAbstractTemplateFactory;

    class LayoutContext 
    {
      private:
        LayoutContext() :
          m_mode(TypeNone),
          m_scene(0),
          m_canvas(0),
          m_zoomFactor(1.),
          m_itemFactory(0),
          m_outsideFactory(0),
          m_utils(0),
          m_dpiX(96),
          m_dpiY(96),
          m_templateFactory(0)
        {
        };  // Private so that it can  not be called	
        LayoutContext(LayoutContext const&){};             // copy constructor is private	
        LayoutContext& operator=(LayoutContext const&){};  // assignment operator is private	
        static LayoutContext* _instance;

      public:
        virtual ~LayoutContext(){};
        static LayoutContext* getInstance();

        LayoutMode	 getMode();
        void setMode(LayoutMode mode);

        LayoutUnitsMetrics getUnitMetric();
        void setUnitMetric(LayoutUnitsMetrics unit);

        LayoutScene* getScene();
        void setScene(LayoutScene* scene);

        double getZoomFactor();
        void  setZoomFactor(double zoomFactor);
      
        LayoutAbstractItemFactory* getItemFactory();
        void setItemFactory(LayoutAbstractItemFactory* factory);

        LayoutAbstractOutsideFactory* getOutsideFactory();
        void setOutsideFactory(LayoutAbstractOutsideFactory* factory);

        LayoutAbstractTemplateFactory* getTemplateFactory();
        void setTemplateFactory(LayoutAbstractTemplateFactory* factory);
        
        te::gm::Envelope* getBoxPaper();
        void setBoxPaper(te::gm::Envelope* box);

        LayoutAbstractPaperType getTypePaper();
        void setTypePaper(LayoutAbstractPaperType type);

        te::map::Canvas* getCanvas();
        void setCanvas(te::map::Canvas* canvas);

        LayoutUtils* getUtils();
        void setUtils(LayoutUtils* utils);

        double getDpiX();
        void setDpiX(double dpiX);

        double getDpiY();
        void setDpiY(double dpiY);

      protected:
        LayoutMode						          m_mode;
        LayoutUnitsMetrics              m_unitMetric;
        LayoutScene*					          m_scene;
        double							            m_zoomFactor;
        te::map::Canvas*	        			m_canvas;
        LayoutAbstractItemFactory*		  m_itemFactory;
        LayoutAbstractOutsideFactory*	  m_outsideFactory;
        LayoutAbstractTemplateFactory*  m_templateFactory;
        te::gm::Envelope*		            m_paperBox; //--->>>>>>>>>>>> Change for just object, no pointer!
        LayoutAbstractPaperType			    m_typePaper;
        LayoutUtils*                    m_utils;
        double                          m_dpiX;
        double                          m_dpiY;
    };
  }
}

#endif