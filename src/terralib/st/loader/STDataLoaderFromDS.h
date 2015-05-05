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
  \file STDataLoaderFromDS.h

  \brief  This file contains a class responsible for loading spatiotemporal data 
          from a DataSource using query capabilities. 
*/

#ifndef __TERRALIB_ST_INTERNAL_STDATALOADERFROMDS_H
#define __TERRALIB_ST_INTERNAL_STDATALOADERFROMDS_H

//ST
#include "../Config.h"
#include "STDataLoaderImpl.h"

namespace te { namespace da { class DataSet; class DataSourceInfo; } }

namespace te
{
  namespace st
  {    
    /*!
      \class STDataLoaderFromDS

      \brief  A class responsible for loading spatiotemporal data 
              from a DataSource using query capabilities. 
    */
    class TESTEXPORT STDataLoaderFromDS : public STDataLoaderImpl
    {
      public:

        STDataLoaderFromDS();

         /*! \name Inhered methods*/
        //@{
        
        std::auto_ptr<ObservationDataSet> getDataSet(const ObservationDataSetInfo& info, 
                                 te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<ObservationDataSet> getDataSet(const ObservationDataSetInfo& info, const te::gm::Envelope& e,
                                 te::gm::SpatialRelation r = te::gm::INTERSECTS,
                                 te::common::TraverseType travType = te::common::FORWARDONLY);
        
        std::auto_ptr<ObservationDataSet> getDataSet(const ObservationDataSetInfo& info, const te::gm::Geometry& geom, 
                                 te::gm::SpatialRelation r = te::gm::INTERSECTS,
                                 te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<ObservationDataSet> getDataSet(const ObservationDataSetInfo& info, 
                                 const te::dt::DateTime& dt, te::dt::TemporalRelation r = te::dt::DURING,
                                 te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<ObservationDataSet> getDataSet(const ObservationDataSetInfo& info,
                                                     const te::dt::DateTime& dt, 
                                                     te::dt::TemporalRelation tr,
                                                     const te::gm::Envelope& e, 
                                                     te::gm::SpatialRelation sr = te::gm::INTERSECTS,
                                                     te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<ObservationDataSet> getDataSet(const ObservationDataSetInfo& info, 
                                 const te::gm::Geometry& geom, te::gm::SpatialRelation sr,
                                 const te::dt::DateTime& dt, te::dt::TemporalRelation tr = te::dt::DURING,
                                 te::common::TraverseType travType = te::common::FORWARDONLY);

        void getInfo( const TrajectoryDataSetInfo& input, std::vector<TrajectoryDataSetInfo>& output);
        
        std::auto_ptr<TrajectoryDataSet> getDataSet(const TrajectoryDataSetInfo& info, 
                              te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<TrajectoryDataSet> getDataSet(const TrajectoryDataSetInfo& info,  
                             const te::gm::Geometry& geom, te::gm::SpatialRelation r,
                             te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<TrajectoryDataSet> getDataSet(const TrajectoryDataSetInfo& info, 
                                const te::gm::Envelope& e, te::gm::SpatialRelation r,
                                te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<TrajectoryDataSet> getDataSet(const TrajectoryDataSetInfo& info, 
                                 const te::dt::DateTime& dt, te::dt::TemporalRelation r = te::dt::DURING,
                                 te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<TrajectoryDataSet> getDataSet(const TrajectoryDataSetInfo& info,
                                                    const te::dt::DateTime& dt, 
                                                    te::dt::TemporalRelation tr,
                                                    const te::gm::Envelope& e, 
                                                    te::gm::SpatialRelation sr = te::gm::INTERSECTS,
                                                    te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<TrajectoryDataSet> getDataSet(const TrajectoryDataSetInfo& info, 
                                 const te::gm::Geometry& geom, te::gm::SpatialRelation sr,
                                 const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                 te::common::TraverseType travType = te::common::FORWARDONLY);
       
        void getInfo( const TimeSeriesDataSetInfo& input, std::vector<TimeSeriesDataSetInfo>& output);
        
        std::auto_ptr<TimeSeriesDataSet> getDataSet(const TimeSeriesDataSetInfo& info, 
                              te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<TimeSeriesDataSet> getDataSet(const TimeSeriesDataSetInfo& info, 
                                 const te::dt::DateTime& dt, te::dt::TemporalRelation r = te::dt::DURING,
                                 te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<TimeSeriesDataSet> getDataSet(const TimeSeriesDataSetInfo& info, const te::gm::Envelope& e,
                                 te::gm::SpatialRelation r = te::gm::INTERSECTS,
                                 te::common::TraverseType travType = te::common::FORWARDONLY);
        
        std::auto_ptr<TimeSeriesDataSet> getDataSet(const TimeSeriesDataSetInfo& info, const te::gm::Geometry& geom, 
                                 te::gm::SpatialRelation r = te::gm::INTERSECTS,
                                 te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<TimeSeriesDataSet> getDataSet(const TimeSeriesDataSetInfo& info,
                                                    const te::dt::DateTime& dt, 
                                                    te::dt::TemporalRelation tr,
                                                    const te::gm::Envelope& e, 
                                                    te::gm::SpatialRelation sr = te::gm::INTERSECTS,
                                                    te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<TimeSeriesDataSet> getDataSet(const TimeSeriesDataSetInfo& info, 
                                 const te::gm::Geometry& geom, te::gm::SpatialRelation sr,
                                 const te::dt::DateTime& dt, te::dt::TemporalRelation tr,
                                 te::common::TraverseType travType = te::common::FORWARDONLY);

        std::auto_ptr<te::dt::DateTimePeriod> getTemporalExtent(const ObservationDataSetInfo& info);

        std::auto_ptr<te::dt::DateTimePeriod> getTemporalExtent(const TrajectoryDataSetInfo& info);
        
        std::auto_ptr<te::dt::DateTimePeriod> getTemporalExtent(const TimeSeriesDataSetInfo& info);

        te::gm::Envelope getSpatialExtent(const ObservationDataSetInfo& info);

        te::gm::Envelope getSpatialExtent(const TrajectoryDataSetInfo& info);
        //@}

    protected:

       /*! \brief It builds an ObservationDataSet from a given DataSet */
        std::auto_ptr<ObservationDataSet> buildDataSet(te::da::DataSet* ds, const ObservationDataSetInfo& info);

        /*! \brief It builds an TrajectoryDataSet from a given DataSet */
        std::auto_ptr<TrajectoryDataSet> buildDataSet(te::da::DataSet* ds, const TrajectoryDataSetInfo& info);

        /*! \brief It builds an TimeSeriesDataSet from a given DataSet */
        std::auto_ptr<TimeSeriesDataSet> buildDataSet(te::da::DataSet* ds, const TimeSeriesDataSetInfo& info);

        virtual ~STDataLoaderFromDS();     
    };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_STDATALOADERFROMDS_H



