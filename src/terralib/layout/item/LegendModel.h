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
  \file LegendModel.h
   
   \brief Class that represents a "Model" part of Legend MVC component.  
   Its coordinate system is the same of scene (millimeters). 
   This is also son of ItemModelObservable, so it can become observable, and son of AbstractVisitor, so it can become visitor.
   It is must visit the map, via te::layout::Visitable*, to get the layers.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LEGEND_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_LEGEND_MODEL_H

// TerraLib
#include "../core/pattern/mvc/ItemModelObservable.h"
#include "../core/ContextItem.h"
#include "../core/pattern/derivativevisitor/AbstractVisitor.h"
#include "../../maptools/AbstractLayer.h"
#include "../../maptools/Canvas.h"
#include "../../maptools/Grouping.h"
#include "../core/Utils.h"
#include "../core/Font.h"
#include "../core/Config.h"

// STL
#include <map>

namespace te
{
  namespace layout
  {
    /*!
    \brief Class that represents a "Model" part of Legend MVC component.  
    Its coordinate system is the same of scene (millimeters). 
    This is also son of ItemModelObservable, so it can become observable, and son of AbstractVisitor, so it can become visitor.
    It is must visit the map, via te::layout::Visitable*, to get the layers.
      	  
	    \ingroup layout

      \sa te::layout::ItemModelObservable , te::layout::AbstractVisitor
	  */
    class TELAYOUTEXPORT LegendModel : public ItemModelObservable, public AbstractVisitor
    {
      public:

        /*!
          \brief Constructor
        */
        LegendModel();

        /*!
          \brief Destructor
        */ 
        virtual ~LegendModel();

        virtual void draw( ContextItem context );

        virtual Properties* getProperties() const;
        
        virtual void updateProperties(te::layout::Properties* properties);

        virtual void setBorderDisplacement(double value);
        
        virtual double getBorderDisplacement();

        virtual void setDisplacementBetweenSymbols(double value);

        virtual double getDisplacementBetweenSymbols();

        virtual void setDisplacementBetweenTitleAndSymbols(double value);

        virtual double getDisplacementBetweenTitleAndSymbols();

        virtual void setDisplacementBetweenSymbolAndText(double value);

        virtual double getDisplacementBetweenSymbolAndText();

        virtual double getSymbolSize();

        virtual void setSymbolSize(const double& value);

        virtual std::map<te::gm::Point*, std::string> getCoordChildren();

        virtual te::map::AbstractLayerPtr getLayer();

        virtual Font getFont();

        virtual te::color::RGBAColor getFontColor();

    private:
        
        virtual void drawGroupingLegend(te::map::Grouping* grouping, te::map::Canvas* canvas, Utils* utils);

        virtual void drawStyleLegend(te::se::Style* style, te::map::Canvas* canvas, Utils* utils);

        virtual void updateBox(ContextItem context);

    protected:

        virtual void visitDependent(ContextItem context);

        virtual void drawLegend(te::map::Canvas* canvas, Utils* utils);

        virtual void childrenFreeMemory();

        std::string               m_mapName;
        te::map::AbstractLayerPtr m_layer;
        double                    m_borderDisplacement;
        double                    m_displacementBetweenSymbols;
        double                    m_displacementBetweenTitleAndSymbols;
        double                    m_displacementBetweenSymbolsAndText;
        double                    m_symbolsize;
        Font                      m_font;
        te::color::RGBAColor			m_fontColor;
        std::map<te::gm::Point*, std::string>    m_coordChildren;
    };
  }
}

#endif 
