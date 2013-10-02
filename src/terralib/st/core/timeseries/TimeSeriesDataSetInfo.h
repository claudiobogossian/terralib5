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
  \file TimeSeriesDataSetInfo.h

  \brief This file contains a class called TimeSeriesDataSetInfo
*/

#ifndef __TERRALIB_ST_INTERNAL_TIMESERIESDATASETINFO_H
#define __TERRALIB_ST_INTERNAL_TIMESERIESDATASETINFO_H

//ST
#include "../../Config.h"
#include "../observation/ObservationDataSetInfo.h"

//STL
#include <vector>

namespace te { namespace da { class DataSourceInfo; } }

namespace te
{
  namespace st
  {
    /*!
      \class TimeSeriesDataSetInfo

      \brief A class that contains infos about a DataSet that contains 
            observations of one or more time series.

      Time series: Geometry, Time -> Value.

      This class contains information about a DataSet that
      contains observations of one or more time series. 
      This information includes: 
      (1) the DataSource which it is from and the DataSet name
      (2) the DataSet properties that contain the time and the observed
          attribute values (it can be one or more observed attributes) and the 
          identity of each time series.   
      
      \sa ObservationDataSetInfo  
    */
    class TESTEXPORT TimeSeriesDataSetInfo
    {
      public:
        
        /*! 
          \brief Constructor.
          
          \param dsinfo     Infos about the DataSource. 
          \param dsName     The DataSet name.   
          \param tPropIdx   The property index that contains the times associated to the time series.
          \param vPropIdx   The property index that contains the attribute values associated to the time series.
          \param gPropIdx   The property index that contains the geometry associated to the time series. 
          \param idPropIdx  The property index that contains the identification of each time series. 
          \param id         The identification of the time series that this DataSet contains. 
        */
        TimeSeriesDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                              int tPropIdx, int vPropIdx, int gPropIdx, int idPropIdx, const std::string& id);

        /*! 
          \brief Constructor.
          
          \param dsinfo     Infos about the DataSource. 
          \param dsName     The DataSet name.   
          \param tPropIdx   The property index that contains the times associated to the time series.
          \param vPropIdxs  The property indexes that contain the attribute values associated to the time series.
          \param gPropIdx   The property index that contains the geometry associated to the time series. 
          \param idPropIdx  The property index that contains the identification of each time series. 
          \param id         The identification of the time series that this DataSet contains. 
        */
        TimeSeriesDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                              int tPropIdx, const std::vector<int>& vPropIdxs, int gPropIdx,
                              int idPropIdx, const std::string& id);
        
        /*! 
          \brief Constructor.
          
          \param info       Infos about the DataSource and DataSet that contains the trajectory observations    
          \param idPropIdx  The property index that contains the identification of each trajectory. 
          \param id         The identification of the trajectory hat this DataSet contains.  
        */
        TimeSeriesDataSetInfo(const ObservationDataSetInfo& info, int idPropIdx, const std::string& id); 
                
        /*!
          \brief It returns information about the DataSet that contians the time series observations

          \return A reference to the internal ObservationDataSetInfo.
        */
        const ObservationDataSetInfo& getObservationDataSetInfo() const;
        
        /*!
          \brief It returns information about the DataSource from which the DataSet comes.

          \return A reference to the internal DataSourceInfo.
        */
        const te::da::DataSourceInfo& getDataSourceInfo() const;
        
        /*!
          \brief It returns the indexes of the DataSet properties that contains the times associated to the time series.

          \return The indexes of the DataSet properties that contains the time.
        */
        const std::vector<int>& getTimePropIdxs() const;

        /*!
          \brief It returns the indexes of the DataSet properties that contains the values associated to the time series.

          \return The indexes of the DataSet properties that contains the observed values.
        */
        const std::vector<int>& getValuePropIdxs() const;
        
        /*!
          \brief  It returns the index of the DataSet property that contains the location
                  of the time series.

          \return The index of the DataSet property that contains the location
                  of the time series.
        */
        int getGeomPropIdx() const;

        /*!
          \brief It returns the index of the DataSet property that contains the 
                identification of each time series. 

          \return The index of the DataSet property that contains 
                  the identification of each time series. 
        */
        int getIdPropIdx() const;

        /*!
          \brief It returns the time series id that this DataSet contains

          \return The time series id.
        */
        std::string getId() const;
 
        /*! \brief Virtual destructor. */
        virtual ~TimeSeriesDataSetInfo() { }; 

      protected:

        ObservationDataSetInfo  m_obsDsInfo;    //!< Infos about the DataSource and DataSet that contains the time series observations    
        std::vector<int>        m_vlPropIdxs;   //!< The indexes of the properties that contain the time series values. 
        int                     m_idPropIdx;    //!< The property index that contains the identification of each time series. 
        std::string             m_id;           //!< The time series id that this DataSet contains        
    };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TIMESERIESDATASETINFO_H

