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
#include "../../enum/EnumMode.h"
#include "../../enum/AbstractType.h"
#include "../../Utils.h"
#include "../factory/AbstractOutsideFactory.h"
#include "../../../../maptools/Canvas.h"
#include "../../../../geometry/Envelope.h"
#include "../../Config.h"
#include "../../../../common/Singleton.h"
#include "../../SystematicScaleConfig.h"
#include "../../PaperConfig.h"

namespace te
{
  namespace layout
  {
    class AbstractScene;
    class AbstractItemFactory;
    class AbstractOutsideFactory;
    class AbstractTemplateFactory;
    class BuildGraphicsItem;

    class TELAYOUTEXPORT Context : public te::common::Singleton<Context>
    {
      friend class te::common::Singleton<Context>;
      
      protected:

        Context();  

      public:
        virtual ~Context();

        LayoutMode	 getMode();
        void setMode(LayoutMode mode);

        LayoutUnitsMetrics getUnitMetric();
        void setUnitMetric(LayoutUnitsMetrics unit);

        AbstractScene* getScene();
        void setScene(AbstractScene* scene);

        double getZoomFactor();
        void  setZoomFactor(double zoomFactor);

        double getDefaultZoomFactor();
        void  setDefaultZoomFactor(double zoomFactor);
      
        double getOldZoomFactor();
        void  setOldZoomFactor(double zoomFactor);

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

        BuildGraphicsItem* getBuildGraphicsItem();
        void setBuildGraphicsItem(BuildGraphicsItem* build);

        LayoutMode getLineIntersectionMouseMode();
        void setLineIntersectionMouseMode(LayoutMode mode);

        SystematicScaleConfig* getSystematicScaleConfig();
        void setSystematicScaleConfig(SystematicScaleConfig* scale);

        private:
      
            /*!
            \brief Copy constructor not allowed.
       
            \param rhs The right-hand-side copy that would be used to copy from.
            */
            Context(const Context& rhs);
      
            /*!
            \brief Assignment operator not allowed.
       
            \param rhs The right-hand-side copy that would be used to copy from.
       
            \return A reference to this object.
            */
            Context& operator=(const Context& rhs);

      protected:
        
        LayoutMode						    m_mode;
        LayoutMode						    m_lineIntersectionMouseMode;
        LayoutUnitsMetrics        m_unitMetric;
        AbstractScene*					  m_scene;
        double							      m_zoomFactor;
        double                    m_defaultZoomFactor;
        double                    m_oldZoomFactor;
        te::map::Canvas*	        m_canvas;
        AbstractItemFactory*		  m_itemFactory;
        AbstractOutsideFactory*	  m_outsideFactory;
        AbstractTemplateFactory*  m_templateFactory;
        Utils*                    m_utils;
        double                    m_dpiX;
        double                    m_dpiY;
        std::string               m_version;
        PaperConfig*              m_paperConfig;
        BuildGraphicsItem*        m_buildGraphicsItem;
        SystematicScaleConfig*    m_systematicConfig;
    };
  }
}

#endif
