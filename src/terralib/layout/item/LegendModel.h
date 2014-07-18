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
  \file LegendModel.h
   
  \brief 

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
#include "../core/Utils.h"

namespace te
{
  namespace layout
  {
    class LegendModel : public ItemModelObservable, public AbstractVisitor
    {
      public:

        LegendModel();
        virtual ~LegendModel();

        virtual void draw( ContextItem context );

        virtual Properties* getProperties() const;
        
        virtual void updateProperties(te::layout::Properties* properties);

        virtual void visitDependent(ContextItem context);

        virtual void setBorderDisplacement(double value);
        
        virtual double getBorderDisplacement();

        virtual void setDisplacementBetweenSymbols(double value);

        virtual double getDisplacementBetweenSymbols();

        virtual void setDisplacementBetweenTitleAndSymbols(double value);

        virtual double getDisplacementBetweenTitleAndSymbols();

        virtual void setDisplacementBetweenSymbolAndText(double value);

        virtual double getDisplacementBetweenSymbolAndText();

       protected:

        virtual void drawLegend(te::map::Canvas* canvas, Utils* utils);

        std::string m_mapName;
        te::map::AbstractLayerPtr m_layer;
        double m_borderDisplacement;
        double m_displacementBetweenSymbols;
        double m_displacementBetweenTitleAndSymbols;
        double m_displacementBetweenSymbolsAndText;
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_MAPLAYOUT_MODEL_H