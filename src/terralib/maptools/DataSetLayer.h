/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/maptools/DataSetLayer.h

  \brief A layer with reference to a dataset.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_DATASETLAYER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_DATASETLAYER_H

// TerraLib
#include "AbstractLayer.h"

namespace te
{
// Forward declaration
  namespace da { class DataSetType; }

  namespace map
  {
    /*!
      \class DataSetLayer

      \brief A layer with reference to a dataset.

      A DataSetLayer is a reference to a dataset with/without geometric attributes.

      \sa AbstractLayer, QueryLayer, RasterLayer, FolderLayer
    */
    class TEMAPEXPORT DataSetLayer : public AbstractLayer
    {
      public:

        /*!
          \brief It initializes a new layer.

          \param parent The parent layer (NULL if it has no parent).
        */
        DataSetLayer(AbstractLayer* parent = 0);

        /*!
          \brief It initializes a new layer.

          \param id     The layer id.
          \param parent The parent layer (NULL if it has no parent).
        */
        DataSetLayer(const std::string& id, AbstractLayer* parent = 0);

        /*!
          \brief It initializes a new Layer.

          \param id     The layer id.
          \param title  The title is a brief description about the layer.
          \param parent The parent layer (NULL if it has no parent).
        */
        DataSetLayer(const std::string& id, const std::string& title, AbstractLayer* parent = 0);

        /*! \brief Destructor. */
        ~DataSetLayer();

        const LayerSchema* getSchema(const bool full = false) const;

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

        te::da::DataSet* getData(te::da::Expression* restriction,
                                 te::common::TraverseType travType = te::common::FORWARDONLY,
                                 te::common::AccessPolicy rwRole = te::common::RAccess) const;

        te::da::DataSet* getData(const te::da::ObjectIdSet* oids,
                                 te::common::TraverseType travType = te::common::FORWARDONLY,
                                 te::common::AccessPolicy rwRole = te::common::RAccess) const;

        bool isValid() const;

        void draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid);

        /*!
          \brief It returns the layer type: DATASET_LAYER.

          \return The layer type: DATASET_LAYER.
        */
        const std::string& getType() const;

        /*!
          \brief

          \return
        */
        const std::string& getDataSetName() const;

        /*!
          \brief

          \param name
        */
        void setDataSetName(const std::string& name);

        /*!
          \brief

          \return
        */
        const std::string& getDataSourceId() const;

        /*!
          \brief

          \param id
        */
        void setDataSourceId(const std::string& id);

        /*!
          \brief

          \return
        */
        const std::string& getRendererType() const;

        /*!
          \brief

          \param t
        */
        void setRendererType(const std::string& t);

      private:

        std::string m_datasetName;         //!< The dataset name where we will retrieve the layer objects.
        std::string m_datasourceId;        //!< The DataSource associated to this layer.
        std::string m_rendererType;        //!< A pointer to the internal renderer used to paint this layer.

        static const std::string sm_type;  //!< A static data member used in the implementation of getType method.
    };

    typedef boost::intrusive_ptr<DataSetLayer> DataSetLayerPtr;

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_DATASETLAYER_H
