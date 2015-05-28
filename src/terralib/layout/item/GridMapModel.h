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
  \file GridMapModel.h
   
   \brief Class that represents a "Model" part of GridMap MVC component. 
      Its coordinate system is the same of scene (millimeters). 
      This is also son of ItemModelObservable, so it can become observable.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GRIDMAP_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_GRIDMAP_MODEL_H

// TerraLib
#include "../core/property/Properties.h"
#include "../../maptools/Canvas.h"
#include "../../maptools/Enums.h"
#include "../core/property/Property.h"
#include "../../geometry/Envelope.h"
#include "../core/Utils.h"
#include "../../geometry/Coord2D.h"
#include "../../geometry/Point.h"
#include "../core/pattern/mvc/ItemModelObservable.h"
#include "../core/ContextItem.h"
#include "../core/Config.h"
#include "../core/pattern/derivativevisitor/AbstractVisitor.h"
#include "../core/Systematic.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace layout
  {
    class GridSettingsConfigProperties;

    /*!
      \brief Class that represents a "Model" part of GridMap MVC component. 
          Its coordinate system is the same of scene (millimeters). 
          He is also the son of ItemModelObservable, so it can become observable.
      	  
	    \ingroup layout

      \sa te::layout::ItemModelObservable
	  */
    class TELAYOUTEXPORT GridMapModel : public ItemModelObservable, public AbstractVisitor
    {
      public:

        /*!
          \brief Constructor
        */
        GridMapModel();

        /*!
          \brief Constructor
        */
        GridMapModel(GridSettingsConfigProperties* settingsConfig);

        /*!
          \brief Destructor
        */ 
        virtual ~GridMapModel();

        virtual Properties* getProperties() const;

        virtual void updateProperties(te::layout::Properties* properties, bool notify = true);   
        
        virtual bool isVisible();

        virtual void setVisible(bool visible);

        virtual void setMapScale(double scale);

        virtual double getMapScale();

        virtual void setWorldBox(te::gm::Envelope box);

        virtual void setBoxMapMM(te::gm::Envelope box);

        virtual void setBoundingBoxItemMM(te::gm::Envelope box);

        virtual void setSystematic(Systematic* sys);

        virtual std::map<te::gm::Point*, std::string> getGridInfo();

        virtual void setVisibleAllTexts(bool visible);
        
        virtual void setMapDisplacementX(double displacement);

        virtual double getMapDisplacementX();

        virtual void setMapDisplacementY(double displacement);  
        
        virtual double getMapDisplacementY();
   
        virtual void setMapName(std::string name);

        virtual std::string getMapName();

        virtual int getSRID();

        virtual te::gm::Envelope  getBoxMapMM();

        virtual te::gm::Envelope getWorldBox();
        
        virtual double getLneHrzGap();

        virtual double getLneVrtGap();

        virtual double getInitialGridPointX();

        virtual double getInitialGridPointY();

        virtual EnumType* getGridStyle();

        virtual EnumType* getLineStyle();

        virtual te::color::RGBAColor  getLineColor();

        virtual int getLineWidth();

        virtual int getTextPointSize();

        virtual std::string getFontFamily();

        virtual te::color::RGBAColor getTextColor();

        virtual bool isVisibleAllTexts();

        virtual bool isSuperscriptText();

        virtual double getLneVrtDisplacement();

        virtual double getLneHrzDisplacement();

        virtual bool isBottomText();

        virtual bool isLeftText();

        virtual bool isRightText();

        virtual bool isTopText();

        virtual bool isBottomRotateText();

        virtual bool isLeftRotateText();

        virtual bool isRightRotateText();

        virtual bool isTopRotateText();

      protected:

        virtual void init();

        virtual void visitDependent(ContextItem context);

        virtual void calculateGaps(te::gm::Envelope box);

        virtual void gridTextFreeMemory();

        std::string                             m_mapName;
        GridSettingsConfigProperties*           m_settingsConfig;

        double                                  m_mapScale;
        int                                     m_srid;
        Systematic*                             m_systematic;
        std::map<te::gm::Point*, std::string>   m_gridTexts;

        te::gm::Envelope                        m_boxMapMM;
        te::gm::Envelope                        m_boundingBoxItemMM;
        te::gm::Envelope                        m_worldBox;
        double                                  m_mapDisplacementX;
        double                                  m_mapDisplacementY;

        /* Grid */
        bool                                    m_visible;
        double                                  m_lneHrzGap;
        double                                  m_lneVrtGap;
        double                                  m_initialGridPointX; 
        double                                  m_initialGridPointY;

        /* Just one is visible */
        EnumType*                               m_gridStyle;

        /* Line */
        EnumType*                               m_lineStyle;
        te::color::RGBAColor                    m_lineColor;
        int                                     m_lineWidth;

        /* Text: Basic Configuration */
        int                                     m_pointTextSize;
        std::string                             m_fontText;
        te::color::RGBAColor                    m_textColor;

        /* Text: Advanced configuration */
        bool                                    m_visibleAllTexts;
        bool                                    m_superscriptText;
        double                                  m_lneVrtDisplacement;
        double                                  m_lneHrzDisplacement;
        bool                                    m_bottomText;
        bool                                    m_leftText;
        bool                                    m_rightText;
        bool                                    m_topText;
        bool                                    m_bottomRotateText;
        bool                                    m_leftRotateText;
        bool                                    m_rightRotateText;
        bool                                    m_topRotateText;  
    };
  }
}

#endif


