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
  \file MapModel.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_MAP_MODEL_H

// TerraLib
#include "../core/pattern/mvc/ItemModelObservable.h"
#include "../core/ContextItem.h"
#include "../core/pattern/derivativevisitor/Visitable.h"
#include "../../maptools/AbstractLayer.h"
#include "../../common/UnitOfMeasure.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace layout
  {
    class Systematic;

    class MapModel : public ItemModelObservable, public Visitable
    {
      public:

        MapModel();
        virtual ~MapModel();

        virtual void draw( ContextItem context );

        virtual te::layout::Properties* getProperties() const;

        virtual void updateProperties(te::layout::Properties* properties);

        virtual bool refreshLayer(te::map::AbstractLayerPtr layer);

        virtual te::map::AbstractLayerPtr getLayer();

        virtual double getScale();

        virtual te::gm::Envelope getWorldInMeters();

        virtual te::gm::Envelope getWorldInDegrees();

        virtual te::common::UnitOfMeasurePtr unitMeasureLayer();

        virtual void setBox(te::gm::Envelope box);

        virtual te::gm::Envelope getMapBox();

        virtual void setPosition(const double& x, const double& y);

        virtual double getDisplacementX();

        virtual double getDisplacementY();

        virtual void setSystematic(Systematic* systematic);
                
        virtual void generateSystematic(te::gm::Coord2D coord);

        virtual bool isFixedScale();

        virtual void setFixedScale(bool fixed);

        virtual bool isPlanar();

        virtual te::gm::Envelope getWorldBox();

        virtual std::map<te::gm::Coord2D, std::string> getTextMapAsObjectInfo();
        
      protected:
        
        te::map::AbstractLayerPtr m_layer;
        te::gm::Envelope          m_mapBoxMM;
        double                    m_mapDisplacementX;
        double                    m_mapDisplacementY;
        Systematic*               m_systematic;
        bool                      m_fixedScale;
        te::gm::Envelope          m_worldBox;
    };
  }
}

#endif 
