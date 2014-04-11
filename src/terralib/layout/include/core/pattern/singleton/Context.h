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
  \file Context.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_CONTEXT_H 
#define __TERRALIB_LAYOUT_INTERNAL_CONTEXT_H

// TerraLib
#include "EnumMode.h"
#include "AbstractType.h"
#include "Utils.h"
#include "AbstractOutsideFactory.h"
#include "../../../../../maptools/Canvas.h"
#include "../../../../../geometry/Envelope.h"

namespace te
{
  namespace layout
  {
    class AbstractScene;
    class AbstractItemFactory;
    class AbstractOutsideFactory;
    class AbstractTemplateFactory;
    class PaperConfig;

    class Context 
    {
      private:
        Context() :
          m_mode(TypeNone),
          m_scene(0),
          m_canvas(0),
          m_zoomFactor(1.),
          m_itemFactory(0),
          m_outsideFactory(0),
          m_utils(0),
          m_dpiX(96),
          m_dpiY(96),
          m_templateFactory(0),
          m_version("TerraPrintQt4_1.0.0"),
          m_paperConfig(0)
        {
        };  // Private so that it can  not be called	
        Context(Context const&){};             // copy constructor is private	
        Context& operator=(Context const&){};  // assignment operator is private	
        static Context* _instance;

      public:
        virtual ~Context(){};
        static Context* getInstance();

        LayoutMode	 getMode();
        void setMode(LayoutMode mode);

        LayoutUnitsMetrics getUnitMetric();
        void setUnitMetric(LayoutUnitsMetrics unit);

        AbstractScene* getScene();
        void setScene(AbstractScene* scene);

        double getZoomFactor();
        void  setZoomFactor(double zoomFactor);
      
        AbstractItemFactory* getItemFactory();
        void setItemFactory(AbstractItemFactory* factory);

        AbstractOutsideFactory* getOutsideFactory();
        void setOutsideFactory(AbstractOutsideFactory* factory);

        AbstractTemplateFactory* getTemplateFactory();
        void setTemplateFactory(AbstractTemplateFactory* factory);
        
        te::gm::Envelope* getBoxPaper();
        void setBoxPaper(te::gm::Envelope* box);

        LayoutAbstractPaperType getTypePaper();
        void setTypePaper(LayoutAbstractPaperType type);

        te::map::Canvas* getCanvas();
        void setCanvas(te::map::Canvas* canvas);

        Utils* getUtils();
        void setUtils(Utils* utils);

        double getDpiX();
        void setDpiX(double dpiX);

        double getDpiY();
        void setDpiY(double dpiY);

        std::string getVersion();

        PaperConfig* getPaperConfig() const;
        void setPaperConfig(PaperConfig* config);

      protected:
        LayoutMode						    m_mode;
        LayoutUnitsMetrics        m_unitMetric;
        AbstractScene*					  m_scene;
        double							      m_zoomFactor;
        te::map::Canvas*	        m_canvas;
        AbstractItemFactory*		  m_itemFactory;
        AbstractOutsideFactory*	  m_outsideFactory;
        AbstractTemplateFactory*  m_templateFactory;
        Utils*                    m_utils;
        double                    m_dpiX;
        double                    m_dpiY;
        std::string               m_version;
        PaperConfig*              m_paperConfig;
    };
  }
}

#endif