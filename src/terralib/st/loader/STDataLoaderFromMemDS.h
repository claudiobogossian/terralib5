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
  \file STDataLoaderFromMemDS.h

  \brief  This file contains a class responsible for loading spatiotemporal data 
          from a ST in-memory DataSource which contains an internal chache of 
          in-memory DataSets indexed by space and time. 
*/

#ifndef __TERRALIB_ST_INTERNAL_STDATALOADERFROMMEMDS_H
#define __TERRALIB_ST_INTERNAL_STDATALOADERFROMMEMDS_H

//ST
#include "../Config.h"
#include "STDataLoaderImpl.h"

//TerraLib
#include "../../dataaccess/datasource/DataSource.h"

//Forward declarations
namespace te { namespace da { class DataSourceInfo; } } 
namespace te { namespace stmem { class DataSet; } } 

namespace te
{
  namespace st
  {    
    /*!
      \class STDataLoaderFromMemDS

      \brief  A class responsible for loading spatiotemporal data 
          from a ST in-memory DataSource which contains an internal chache of 
          in-memory DataSets indexed by space and time.  

      All infos about the DataSets that contains observations, such as ObservationDataSetInfo,
      TrajectoryDataSetInfo, etc, contains information about the original DataSource and
      DataSet.  

      //TO DO: verificar o nome do DataSet: do jeito que está não pode ter nome repetido!
    */
    class TESTEXPORT STDataLoaderFromMemDS : public STDataLoaderImpl
    {
      public:

        /*! \brief Constructor*/
        STDataLoaderFromMemDS();

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

        std::auto_ptr<te::dt::DateTimePeriod> getTemporalExtent(const ObservationDataSetInfo& info);

        std::auto_ptr<te::dt::DateTimePeriod> getTemporalExtent(const TrajectoryDataSetInfo& info);
        
        std::auto_ptr<te::dt::DateTimePeriod> getTemporalExtent(const TimeSeriesDataSetInfo& info);

        te::gm::Envelope getSpatialExtent(const ObservationDataSetInfo& info);

        te::gm::Envelope getSpatialExtent(const TrajectoryDataSetInfo& info);
        //@}
    
        virtual ~STDataLoaderFromMemDS();     
    
      protected:

        /*! \brief It loads DataSet from the original DataSource and put it in the In-Memory DataSource
        
            \note It can through an Exception. 
        */
        void loadDataSet( const te::da::DataSourceInfo& info, const std::string dsname, 
                          const std::string& begTimePropName, const std::string& endTimePropName, 
                          const std::string& gmPropName);

        /*! \brief It builds an ObservationDataSet from a given DataSet */
        std::auto_ptr<ObservationDataSet> buildDataSet(te::stmem::DataSet* ds, const ObservationDataSetInfo& info);

        /*! \brief It builds an TrajectoryDataSet from a given DataSet */
        std::auto_ptr<TrajectoryDataSet> buildDataSet(te::stmem::DataSet* ds, const TrajectoryDataSetInfo& info);

        /*! \brief It builds an TimeSeriesDataSet from a given DataSet */
        std::auto_ptr<TimeSeriesDataSet> buildDataSet(te::stmem::DataSet* ds, const TimeSeriesDataSetInfo& info);
      
      private:

        te::da::DataSourcePtr m_ds; //! A shared pointer to the ST In-Mem Data Source

    };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_STDATALOADERFROMMEMDS_H



