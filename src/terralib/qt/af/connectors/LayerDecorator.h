/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*! 
  \file LayerDecorator.h

  \brief A connector for abstract layers to communicate with application framework.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_LAYERDECORATOR_H
#define __TERRALIB_QT_AF_INTERNAL_LAYERDECORATOR_H

#include "../Config.h"

// TerraLib
#include "../../../common/Decorator.h"
#include "../../../maptools/AbstractLayer.h"

namespace te
{
  namespace qt
  {
    namespace af
    {
      /*!
        \class LayerDecorator.

        \brief This class will be used to communicate changes on layer to the framework.
      */
      class LayerDecorator : public te::common::Decorator<te::map::AbstractLayer>
      {
        public:

          LayerDecorator(const te::map::AbstractLayerPtr& layer);

          ~LayerDecorator();

          const std::string& getId() const;

          void setId(const std::string& id);

          const std::string& getTitle() const;

          void setTitle(const std::string& title);
        
          te::map::Visibility getVisibility() const;

          void setVisibility(te::map::Visibility v);

          void adjustVisibility();

          const te::gm::Envelope& getExtent() const;

          void setExtent(const te::gm::Envelope& mbr);

          int getSRID() const;

          void setSRID(int srid);

          void select(te::da::ObjectIdSet* oids);

          void deselect(const te::da::ObjectIdSet* oids);

          const te::da::ObjectIdSet* getSelected() const;

          void clearSelected();

          const te::map::LayerSchema* getSchema() const;

          te::da::DataSet* getData(te::common::TraverseType travType = te::common::FORWARDONLY, 
                                         te::common::AccessPolicy rwRole = te::common::RAccess) const;
          
          te::da::DataSet* getData(const te::gm::Envelope& e,
                                         te::gm::SpatialRelation r = te::gm::INTERSECTS,
                                         te::common::TraverseType travType = te::common::FORWARDONLY,
                                         te::common::AccessPolicy rwRole = te::common::RAccess) const;
          
          te::da::DataSet* getData(const te::dt::Property& p,
                                         const te::gm::Envelope& e,
                                         te::gm::SpatialRelation r = te::gm::INTERSECTS,
                                         te::common::TraverseType travType = te::common::FORWARDONLY,
                                         te::common::AccessPolicy rwRole = te::common::RAccess) const;
          
          te::da::DataSet* getData(const te::gm::Geometry& g,
                                         te::gm::SpatialRelation r = te::gm::INTERSECTS,
                                         te::common::TraverseType travType = te::common::FORWARDONLY, 
                                         te::common::AccessPolicy rwRole = te::common::RAccess) const;

          te::da::DataSet* getData(const te::dt::Property& p,
                                         const te::gm::Geometry& g,
                                         te::gm::SpatialRelation r,
                                         te::common::TraverseType travType = te::common::FORWARDONLY,
                                         te::common::AccessPolicy rwRole = te::common::RAccess) const;

          te::da::DataSet* getData(const te::da::ObjectIdSet* oids,
                                         te::common::TraverseType travType = te::common::FORWARDONLY,
                                         te::common::AccessPolicy rwRole = te::common::RAccess) const;
          
          const std::string& getType() const;

          bool isValid() const;

          void draw(te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid);
      };

      typedef boost::intrusive_ptr<LayerDecorator> LayerDecoratorPtr;

    }
  }
}

#endif //__TERRALIB_QT_AF_INTERNAL_LAYERDECORATOR_H
