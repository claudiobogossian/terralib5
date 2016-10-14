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
  \file terralib/ws/ogc/wms/WMSLayer.h

  \brief A layer with reference to a WMS Layer.
*/

#ifndef __TERRALIB_WS_OGC_WMSLAYER_H
#define __TERRALIB_WS_OGC_WMSLAYER_H

//TerraLib

#include "../../../../maptools/AbstractLayer.h"
#include "Config.h"

#include "../client/DataTypes.h"

namespace te
{
  namespace ws
  {
    namespace ogc
    {
      namespace wms
      {

      /*!
        \class WMSLayer

        \brief A layer with reference to a WMS Layer.
      */
      class TEOGCWMSDATAACCESSEXPORT WMSLayer : public te::map::AbstractLayer
      {
        
      public:

        /*!
          \brief It initializes a new layer.

          \param parent The parent layer (NULL if it has no parent).
        */
        WMSLayer(te::map::AbstractLayer* parent = 0);

        /*!
          \brief It initializes a new layer.

          \param id     The layer id.
          \param parent The parent layer (NULL if it has no parent).
        */
        WMSLayer(const std::string& id, te::map::AbstractLayer* parent = 0);

        /*!
          \brief It initializes a new Layer.

          \param id     The layer id.
          \param title  The title is a brief description about the layer.
          \param parent The parent layer (NULL if it has no parent).
        */
        WMSLayer(const std::string& id, const std::string& title, te::map::AbstractLayer* parent = 0);

        /*! \brief Destructor. */
        ~WMSLayer();

        virtual std::auto_ptr<te::map::LayerSchema> getSchema() const;

        std::auto_ptr<te::da::DataSet> getData(te::common::TraverseType travType = te::common::FORWARDONLY,
                                               const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

        std::auto_ptr<te::da::DataSet> getData(const std::string& propertyName,
                                               const te::gm::Envelope* e,
                                               te::gm::SpatialRelation r = te::gm::INTERSECTS,
                                               te::common::TraverseType travType = te::common::FORWARDONLY,
                                               const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

        std::auto_ptr<te::da::DataSet> getData(const std::string& propertyName,
                                               const te::gm::Geometry* g,
                                               te::gm::SpatialRelation r,
                                               te::common::TraverseType travType = te::common::FORWARDONLY,
                                               const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

        std::auto_ptr<te::da::DataSet> getData(te::da::Expression* restriction,
                                               te::common::TraverseType travType = te::common::FORWARDONLY,
                                               const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

        std::auto_ptr<te::da::DataSet> getData(const te::da::ObjectIdSet* oids,
                                               te::common::TraverseType travType = te::common::FORWARDONLY,
                                               const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

        bool isValid() const;

        void draw(te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid, const double& scale, bool* cancel);

        const std::string& getType() const;

        const std::string& getDataSourceId() const;

        void setDataSourceId(const std::string& datadourceId);

        const std::string& getRendererType() const;

        void setRendererType(const std::string& rendererType);

        te::ws::ogc::wms::WMSGetMapRequest getRequest() const;

        void setGetMapRequest(const te::ws::ogc::wms::WMSGetMapRequest& getMapRequest);

        /*!
          \brief It gets a WMS GetMap response with the image on a buffer.
                 The request will be based on WMSGetMapRequest setted to this WMSLayer.

          \return WMSGetMapResponse with the image, size and format.
          */
        const te::ws::ogc::wms::WMSGetMapResponse getMap() const;

      private:

        std::string                        m_datasourceId;   //!< The DataSource associated to this layer.
        std::string                        m_rendererType;   //!< A pointer to the internal renderer used to paint this layer.
        te::ws::ogc::wms::WMSGetMapRequest m_getMapRequest;

        mutable std::auto_ptr<te::map::LayerSchema> m_schema; //!< The WMS layer schema.

        static const std::string sm_type;                    //!< A static data member used in the implementation of getType method.
      };

      typedef boost::intrusive_ptr<WMSLayer> WMSLayerPtr;
      }
    }
  }
}

#endif // __TERRALIB_WS_OGC_WMSLAYER_H
