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
  \file terralib/maptools/QueryLayer.h

  \brief A layer resulting from a query.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_QUERYLAYER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_QUERYLAYER_H

// TerraLib
#include "AbstractLayer.h"

namespace te
{
// Forward declaration
  namespace da { class Select; }

  namespace map
  {
    /*!
      \class QueryLayer

      \brief A layer resulting from a query.

      A QueryLayer is a reference to a dataset that will result from a query to a data source.

      \ingroup map

      \sa AbstractLayer, DataSetLayer, RasterLayer, FolderLayer
    */
    class TEMAPEXPORT QueryLayer : public AbstractLayer
    {
      public:

        /*!
          \brief It initializes a new layer.

          \param parent The parent layer (NULL if it has no parent).
        */
        QueryLayer(AbstractLayer* parent = 0);

        /*!
          \brief It initializes a new layer.

          \param id     The layer id.
          \param parent The parent layer (NULL if it has no parent).
        */
        QueryLayer(const std::string& id, AbstractLayer* parent = 0);

        /*!
          \brief It initializes a new layer.

          \param id     The layer id.
          \param title  The title is a brief description about the layer.
          \param parent The parent layer (NULL if it has no parent).
        */
        QueryLayer(const std::string& id, const std::string& title, AbstractLayer* parent = 0);

        /*! \brief Destructor. */
        ~QueryLayer();

        std::auto_ptr<LayerSchema> getSchema() const;

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

        void draw(Canvas* canvas, const te::gm::Envelope& bbox, int srid);

        /*!
          \brief It returns the layer type: QUERY_LAYER.

          \return The layer type: QUERY_LAYER.
        */
        const std::string& getType() const;

        /*!
          \brief

          \return
        */
        te::da::Select* getQuery() const;

        /*!
        \brief

        \return
        */
      std::string getQueryAsString() const;

        /*!
          \brief

          \param name
        */
        void setQuery(te::da::Select* s);

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

        void computeExtent();

      private:

         std::auto_ptr<te::da::DataSet> getData(te::da::Select* query,
                                                te::common::TraverseType travType = te::common::FORWARDONLY,
                                                const te::common::AccessPolicy accessPolicy = te::common::RAccess) const;

      private:

        std::string m_datasourceId;        //!< The DataSource associated to this Layer.
        std::string m_rendererType;        //!< A pointer to the internal renderer used to paint this layer.
        te::da::Select* m_query;           //!< The dataset name where we will retrieve the layer objects.

        static const std::string sm_type;  //!< A static data member used in the implementation of getType method.
    };

    typedef boost::intrusive_ptr<QueryLayer> QueryLayerPtr;

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_QUERYLAYER_H
