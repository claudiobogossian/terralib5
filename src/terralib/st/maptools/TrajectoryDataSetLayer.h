/*  Copyright (C) 2010-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/st/maptools/TrajectoryDataSetLayer.h

  \brief A layer with a reference to an TrajectoryDataSet. 
*/

#ifndef __TERRALIB_ST_INTERNAL_TRAJECTORYDATASETLAYER_H
#define __TERRALIB_ST_INTERNAL_TRAJECTORYDATASETLAYER_H

// ST
#include "../Config.h"
#include "AbstractSTDataSetLayer.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace st
  {

    //Forward declarations
    class TrajectoryDataSet;
    class TrajectoryDataSetInfo;

    typedef te::da::DataSetType LayerSchema;

  /*!
      \class TrajectoryDataSetLayer

      \brief A layer with reference to a dataset that contains trajectories.

      \sa AbstractLayer, AbstractSTDataSetLayer
    */
    class TESTEXPORT TrajectoryDataSetLayer : public AbstractSTDataSetLayer
    {
      public:


        /*!
          \brief It initializes a new layer.

          \param parent The parent layer (NULL if it has no parent).
        */
        TrajectoryDataSetLayer(AbstractLayer* parent = 0);

       /*!
          \brief It initializes a new layer.

          \param parent The parent layer (NULL if it has no parent).
          \param info   Infos about the data source and its data set that contains observations.
         
          \note It will take the ownership of the given pointer info
        */
        TrajectoryDataSetLayer(AbstractLayer* parent, TrajectoryDataSetInfo* info);

        /*!
          \brief It initializes a new layer.

          \param id     The layer id.
          \param parent The parent layer (NULL if it has no parent).
          \param info   Infos about the data source and its data set that contains observations.

          \note It will take the ownership of the given pointer info
        */
        TrajectoryDataSetLayer(const std::string& id, AbstractLayer* parent, TrajectoryDataSetInfo* info);

        /*!
          \brief It initializes a new Layer.

          \param id     The layer id.
          \param title  The title is a brief description about the layer.
          \param parent The parent layer (NULL if it has no parent).
          \param info   Infos about the data source and its data set that contains observations.

          \note It will take the ownership of the given pointer info
        */
        TrajectoryDataSetLayer(const std::string& id, const std::string& title, AbstractLayer* parent, TrajectoryDataSetInfo* info);

        /*! \brief Destructor. */
        ~TrajectoryDataSetLayer();

        std::auto_ptr<LayerSchema> getSchema() const;

        /*!
          \brief It returns the layer temporal extent.

          \return The layer temporal extent.
        */
        te::dt::DateTimePeriod* getTemporalExtent() const;

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

        std::auto_ptr<te::st::TrajectoryDataSet> getTrajectoryDataset(te::common::TraverseType travType = te::common::FORWARDONLY) const;

        std::auto_ptr<te::st::TrajectoryDataSet> getTrajectoryDataset(const te::gm::Envelope& e,
                                                                      te::gm::SpatialRelation r = te::gm::INTERSECTS,
                                                                      te::common::TraverseType travType = te::common::FORWARDONLY) const;

        std::auto_ptr<te::st::TrajectoryDataSet> getTrajectoryDataset(const te::gm::Geometry& g,
                                                                      te::gm::SpatialRelation r,
                                                                      te::common::TraverseType travType = te::common::FORWARDONLY) const;

        std::auto_ptr<te::da::DataSet> getData( const te::dt::DateTime& dt, te::dt::TemporalRelation r = te::dt::DURING,
                                                te::common::TraverseType travType = te::common::FORWARDONLY, 
                                                te::common::AccessPolicy rwRole = te::common::RAccess) const;

        std::auto_ptr<te::da::DataSet> getData( const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                                const te::gm::Envelope& e, te::gm::SpatialRelation sr = te::gm::INTERSECTS,
                                                te::common::TraverseType travType = te::common::FORWARDONLY,
                                                te::common::AccessPolicy rwRole = te::common::RAccess) const;

         std::auto_ptr<te::da::DataSet> getData( const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                                 const te::gm::Geometry& g, te::gm::SpatialRelation sr = te::gm::INTERSECTS,
                                                 te::common::TraverseType travType = te::common::FORWARDONLY,
                                                 te::common::AccessPolicy rwRole = te::common::RAccess) const;

        std::auto_ptr<te::st::TrajectoryDataSet> getTrajectoryDataset( const te::dt::DateTime& dt, te::dt::TemporalRelation r = te::dt::DURING,
                                                                       te::common::TraverseType travType = te::common::FORWARDONLY) const;

        std::auto_ptr<te::st::TrajectoryDataSet> getTrajectoryDataset( const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                                                       const te::gm::Envelope& e, te::gm::SpatialRelation sr = te::gm::INTERSECTS,
                                                                       te::common::TraverseType travType = te::common::FORWARDONLY) const;

         std::auto_ptr<te::st::TrajectoryDataSet> getTrajectoryDataset( const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                                                        const te::gm::Geometry& g, te::gm::SpatialRelation sr = te::gm::INTERSECTS,
                                                                        te::common::TraverseType travType = te::common::FORWARDONLY) const;

        bool isValid() const;

        void draw(te::map::Canvas* canvas, const te::gm::Envelope& bbox, int srid);

        /*!
          \brief It returns the layer type: TRAJECTORYDATASETLAYER.

          \return The layer type: TRAJECTORYDATASETLAYER.
        */
        const std::string& getType() const;

        /*!
          \brief

          \return
        */
        const std::string& getDataSourceId() const;

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

        /*!
          \brief Checks if the layer has any childs

          \return true if the layer has any childs, otherwise returns false
        */

        bool hasChilds();

      private:

        boost::ptr_vector<TrajectoryDataSetLayer>  m_layers;        //!, A container the holds any child layers.
        std::auto_ptr<TrajectoryDataSetInfo>       m_info;          //!< Infos about the data source and its data set that contains observations.
        std::string                                m_rendererType;  //!< A pointer to the internal renderer used to paint this layer.
        static const std::string                   sm_type;         //!< A static data member used in the implementation of getType method.
    };

    typedef boost::intrusive_ptr<TrajectoryDataSetLayer> TrajectoryDataSetLayerPtr;

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TRAJECTORYDATASETLAYER_H