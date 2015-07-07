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
  \file MapModel.h
   
   \brief Class that represents a "Model" part of Map MVC component.  
   Its coordinate system is the same of scene (millimeters). 
   This is also son of ItemModelObservable, so it can become observable, and son of Visitable, so it can become visitable.

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
#include "../../geometry/Point.h"
#include "../core/Config.h"

// STL
#include <map>
#include <string>
#include <list>
#include <vector>

namespace te
{
  namespace layout
  {
    class Systematic;

    /*!
    \brief Class that represents a "Model" part of Map MVC component.  
    Its coordinate system is the same of scene (millimeters). 
    This is also son of ItemModelObservable, so it can become observable, and son of Visitable, so it can become visitable.
      	  
	    \ingroup layout

      \sa te::layout::ItemModelObservable , te::layout::Visitable
	  */
    class TELAYOUTEXPORT MapModel : public ItemModelObservable, public Visitable
    {
      public:

        /*!
          \brief Constructor
        */
        MapModel();

        /*!
          \brief Destructor
        */ 
        virtual ~MapModel();

        virtual te::layout::Properties* getProperties() const;

        virtual void updateProperties(te::layout::Properties* properties, bool notify = true);

        virtual bool addLayer(te::map::AbstractLayerPtr layer);

        virtual bool removeLayer(te::map::AbstractLayerPtr layer);

        std::list<te::map::AbstractLayerPtr> getLayers();

        virtual double getScale();

        virtual te::gm::Envelope getWorldInMeters();

        virtual te::gm::Envelope getWorldInDegrees();

        virtual te::common::UnitOfMeasurePtr unitMeasureLayer();

        virtual void setBox(te::gm::Envelope box);

        virtual te::gm::Envelope getMapBox();

        virtual void setPosition(const double& x, const double& y);

        virtual double getDisplacementX();

        virtual void setDisplacementX(double displacement);

        virtual double getDisplacementY();

        virtual void setDisplacementY(double displacement);

        virtual void setSystematic(Systematic* systematic);
                
        virtual void generateSystematic(te::gm::Coord2D coord);

        virtual bool isFixedScale();

        virtual void setFixedScale(bool fixed);

        virtual bool isPlanar();

        virtual te::gm::Envelope getWorldBox();

        virtual std::map<te::gm::Point*, std::string> getTextMapAsObjectInfo();

        virtual bool isLoadedLayer();

        void clear();

        std::vector<std::string> getLayerNames();

        virtual te::gm::Envelope maxLayerExtent();

      protected:

        virtual void recalculateMapBoxMM();

        virtual te::layout::Property getLayerNamesProperty() const;

        virtual te::layout::Property getLayersGenericVariant() const;

        std::vector<std::string> findLayerNames() const;

        virtual void updateMapDisplacementX(double displacementX);

        virtual void updateMapDisplacementY(double displacementY);

        std::list<te::map::AbstractLayerPtr>    m_layers;
        te::gm::Envelope					              m_mapBoxMM;
        double								                  m_mapDisplacementX;
        double								                  m_mapDisplacementY;
        Systematic*							                m_systematic;
        bool								                    m_fixedScale;
        te::gm::Envelope					              m_worldBox;
        bool								                    m_loadedLayer;
        std::vector<std::string>			          m_layerNames;
    };
  }
}

#endif 
