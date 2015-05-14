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
  \file Context.h
   
   \brief Singleton class responsible for keeping active objects while the plugin is loaded in memory 
          and make them available for access anywhere in the plugin or layout module.
      Ex .: factories objects, canvas, class of utility functions, proxy to access the project, etc.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_CONTEXT_H 
#define __TERRALIB_LAYOUT_INTERNAL_CONTEXT_H

// TerraLib
#include "../../enum/AbstractType.h"
#include "../../Utils.h"
#include "../factory/AbstractOutsideFactory.h"
#include "../../../../maptools/Canvas.h"
#include "../../../../geometry/Envelope.h"
#include "../../Config.h"
#include "../../../../common/Singleton.h"
#include "../../SystematicScaleConfig.h"
#include "../../PaperConfig.h"
#include "../mvc/ItemObserver.h"

namespace te
{
  namespace layout
  {
    class AbstractScene;
    class AbstractItemFactory;
    class AbstractOutsideFactory;
    class AbstractTemplateFactory;
    class AbstractBuildGraphicsItem;
    class EnumType;
    class AbstractProxyProject;
    class ItemUtils;
    class AbstractBuildGraphicsOutside;

    /*!
      \brief Singleton class responsible for keeping active objects while the plugin is loaded in memory 
           and make them available for access anywhere in the plugin or layout module.
        Ex .: factories objects, canvas, class of utility functions, proxy to access the project, etc.
	  
	    \ingroup layout

	    \sa te::common::Singleton
	  */
    class TELAYOUTEXPORT Context : public te::common::Singleton<Context>
    {
      friend class te::common::Singleton<Context>;
      
      protected:

        /*!
          \brief Constructor
        */ 
        Context();  

      public:

        /*!
          \brief Destructor
        */ 
        virtual ~Context();

        /*!
          \brief Returns value of the enumeration of mode type.
		  
		      \return value of the enumeration of mode type 	  
        */
        EnumType*	 getMode();

        /*!
        \brief Change value of the enumeration of mode type. Ex.: select, pan, create text, etc.
            The type of mode is used by the context to know what should be done. The mode in context could be modified by the user interaction.
		  
		      \param value of the enumeration of mode type 	  
        */
        void setMode(EnumType* mode);

        LayoutUnitsMetrics getUnitMetric();

        void setUnitMetric(LayoutUnitsMetrics unit);

        /*!
          \brief Returns abstract scene for QGraphicsScene class, part of Graphics View Framework.
		  
		      \return scene 	  
        */
        AbstractScene* getScene();

        /*!
          \brief Change scene, QGraphicsScene class, part of Graphics View Framework.
		  
		      \param scene 	  
        */
        void setScene(AbstractScene* scene);

        /*!
          \brief Returns current zoom factor. Ex.: 0.5 (50%)
		  
		      \return current zoom factor 	  
        */
        double getZoomFactor();

        /*!
          \brief Change current zoom factor. Ex.: 0.5 (50%)
		  
		      \param zoomFactor current zoom factor 	 	  
        */
        void  setZoomFactor(double zoomFactor);

        /*!
          \brief Default zoom factor. Ex.: 0.5 (50%) 
		  
		      \return default zoom factor	  
        */
        double getDefaultZoomFactor();

        /*!
          \brief Change default zoom factor. Ex.: 0.5 (50%)
		  
		      \param zoomFactor default zoom factor
        */
        void  setDefaultZoomFactor(double zoomFactor);

        /*!
          \brief Returns previous zoom factor. Ex.: 0.5 (50%)
		  
		      \return previous zoom factor	  
        */
        double getOldZoomFactor();

        /*!
          \brief Change previous zoom factor. Ex.: 0.5 (50%)
		  
		      \param zoomFactor previous zoom factor	  
        */
        void  setOldZoomFactor(double zoomFactor);

        /*! 
          \brief Returns Abstract Factory that provide an interface for creating families of related or dependent graphic objects (MVC components).
		  
		      \return factory for creating families of related or dependent graphic objects (MVC components)  
        */ 
        AbstractItemFactory* getItemFactory();

        /*!
          \brief Change factory for creating families of related or dependent graphic objects (MVC components).
		  
		      \param factory for creating families of related or dependent graphic objects (MVC components)  
        */ 
        void setItemFactory(AbstractItemFactory* factory);

        /*!
          \brief Returns Abstract Factory provide an interface for creating families of related or dependent graphic widgets (MVC widgets).
		  
		      \return factory provide an interface for creating families of related or dependent graphic widgets (MVC widgets)  
        */
        AbstractOutsideFactory* getOutsideFactory();

        /*!
          \brief Change factory provide an interface for creating families of related or dependent graphic widgets (MVC widgets).
		  
		      \param factory provide an interface for creating families of related or dependent graphic widgets (MVC widgets)  
        */
        void setOutsideFactory(AbstractOutsideFactory* factory);

        /*!
          \brief Returns Abstract Factory provide an interface for creating families of related or dependent templates.
		  
		      \return factory provide an interface for creating families of related or dependent templates. 		  
        */
        AbstractTemplateFactory* getTemplateFactory();

        /*!
          \brief Change factory provide an interface for creating families of related or dependent templates.
		  
		      \param factory provide an interface for creating families of related or dependent templates. 		  
        */
        void setTemplateFactory(AbstractTemplateFactory* factory);
                
        /*!
          \brief Returns abstraction of a drawing area.
		  
		      \return abstraction of a drawing area 		  
        */
        te::map::Canvas* getCanvas();

        /*!
          \brief Change abstraction of a drawing area.
		  
		      \param abstraction of a drawing area 		  
        */
        void setCanvas(te::map::Canvas* canvas);

        /*!
          \brief Returns pointer with functions to manipulate the canvas and conversion between projections.
		  
		      \return A te::layout::Utils pointer		  
        */
        Utils* getUtils();

        /*!
          \brief Change pointer with functions to manipulate the canvas and conversion between projections.
		  
		      \param A te::layout::Utils pointer		  
        */
        void setUtils(Utils* utils);

        double getDpiX();

        void setDpiX(double dpiX);

        double getDpiY();

        void setDpiY(double dpiY);

        /*!
          \brief Template structure version.
		  
		      \param version		  
        */
        std::string getVersion();

        /*!
          \brief Returns paper setting. 
		  
		      \return A te::layout::PaperConfig pointer		  
        */
        PaperConfig* getPaperConfig() const;

        /*!
          \brief Change paper setting. 
		  
		      \param A te::layout::PaperConfig pointer		  
        */
        void setPaperConfig(PaperConfig* config);

        /*!
          \brief Returns pointer for build graphics MVC components. 
		  
		      \return a AbstractBuildGraphicsItem pointer 		  
        */
        AbstractBuildGraphicsItem* getAbstractBuildGraphicsItem();

        /*!
          \brief Change a pointer for build graphics MVC components. 
		  
		      \param a AbstractBuildGraphicsItem pointer		  
        */
        void setAbstractBuildGraphicsItem(AbstractBuildGraphicsItem* build);

        EnumType* getLineIntersectionMouseMode();

        void setLineIntersectionMouseMode(EnumType* mode);

        SystematicScaleConfig* getSystematicScaleConfig();

        void setSystematicScaleConfig(SystematicScaleConfig* scale);

        /*!
          \brief Change the proxy to provide a surrogate or placeholder for te::qt::af::Project to control access to it
		  
		      \param proxy for te::qt::af::Project 		  
        */
        void setProxyProject(AbstractProxyProject* project);

        /*!
          \brief Returns proxy to provide a surrogate or placeholder for te::qt::af::Project to control access to it
		  
		      \return proxy for te::qt::af::Project 		  
        */
        AbstractProxyProject* getProxyProject();
        
        /*!
          \brief Change pointer for manipulating items in the scene and vectorization of text and legend.
		  
		      \param A te::layout::ItemUtils pointer		  
        */
        void setItemUtils(ItemUtils* utils);

        /*!
          \brief Returns pointer for manipulating items in the scene and vectorization of text and legend.
		  
		      \return A te::layout::ItemUtils pointer		  
        */
        ItemUtils* getItemUtils();

        EnumType* getWait();

        void setWait(EnumType* wait);

        ItemObserver* getItem();

        void setItem(ItemObserver* item);

        /*!
          \brief Returns pointer for build graphics MVC components. 
		  
		      \return a AbstractBuildGraphicsItem pointer 		  
        */
        AbstractBuildGraphicsOutside* getAbstractBuildGraphicsOutside();

        /*!
          \brief Change a pointer for build graphics MVC components. 
		  
		      \param a AbstractBuildGraphicsItem pointer		  
        */
        void setAbstractBuildGraphicsOutside(AbstractBuildGraphicsOutside* build);

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
        
        EnumType*						          m_mode; //!< value of the enumeration of mode type
        EnumType*						          m_lineIntersectionMouseMode; //!<
        LayoutUnitsMetrics            m_unitMetric; //!<
        AbstractScene*					      m_scene; //!< abstract scene for QGraphicsScene class, part of Graphics View Framework.
        double							          m_zoomFactor; //!< current zoom factor. Ex.: 0.5 (50%)
        double                        m_defaultZoomFactor; //!< default zoom factor. Ex.: 0.5 (50%) 
        double                        m_oldZoomFactor; //!< previous zoom factor. Ex.: 0.5 (50%)
        te::map::Canvas*	            m_canvas; //!< abstraction of a drawing area
        AbstractItemFactory*		      m_itemFactory; //!< factory that provide an interface for creating families of related or dependent graphic objects (MVC components)
        AbstractOutsideFactory*	      m_outsideFactory; //!< factory provide an interface for creating families of related or dependent graphic widgets (MVC widgets)
        AbstractTemplateFactory*      m_templateFactory; //!< factory provide an interface for creating families of related or dependent templates
        Utils*                        m_utils; //!< pointer with functions to manipulate the canvas and conversion between projections
        double                        m_dpiX; //!<
        double                        m_dpiY; //!<
        std::string                   m_version; //!< template structure version
        PaperConfig*                  m_paperConfig; //!< paper settings
        AbstractBuildGraphicsItem*    m_buildGraphicsItem; //!< build graphics MVC components. 
        SystematicScaleConfig*        m_systematicConfig; //!<
        AbstractProxyProject*         m_proxyProject; //!< provide a surrogate or placeholder for te::qt::af::Project to control access to it
        ItemUtils*                    m_itemUtils; //!< pointer for manipulating items in the scene and vectorization of text and legend
        EnumType*				              m_wait;
        ItemObserver*			            m_item;
        AbstractBuildGraphicsOutside* m_buildGraphicsOutside; //!< build MVC widgets. 
    };
  }
}

#endif
