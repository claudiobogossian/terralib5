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
          \param tPropName   The property name that contains the times associated to the time series.
          \param vPropName   The property name that contains the attribute values associated to the time series.
          \param gPropName   The property name that contains the geometry associated to the time series. 
          \param idPropName  The property name that contains the identification of each time series. 
          \param id         The identification of the time series that this DataSet contains. 
        */
        TimeSeriesDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                              const std::string& tPropName, const std::string& vPropName, 
                              const std::string& gPropName, const std::string& idPropName, 
                              const std::string& id);
        /*! 
          \brief Constructor.
          \param dsinfo     Infos about the DataSource. 
          \param dsName     The DataSet name.   
          \param tPropName   The property name that contains the times associated to the time series.
          \param vPropName   The property name that contains the attribute values associated to the time series.
          \param gPropName   The property name that contains the geometry associated to the time series. 
          \param idPropName  The property name that contains the identification of each time series. 
          \param id         The identification of the time series that this DataSet contains. 
        */
        TimeSeriesDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                              const std::string& tPropName, const std::vector<std::string>& vPropNames, 
                              const std::string& gPropName, const std::string& idPropName, 
                              const std::string& id);
        
        /*! 
          \brief Constructor.
          
          \param info         Infos about the DataSource and DataSet that contains the time series observations    
          \param valPropNames  The property names that contains the time series values.  
        */
        TimeSeriesDataSetInfo(const ObservationDataSetInfo& info, const std::vector<std::string>& valPropNames); 
                
        /*!
          \brief It returns information about the DataSet that contains the time series observations

          \return A reference to the internal ObservationDataSetInfo.
        */
        const ObservationDataSetInfo& getObservationDataSetInfo() const;
        
        /*!
          \brief It returns the names of the DataSet properties that contains the values associated to the time series.

          \return The names of the DataSet properties that contains the observed values.
        */
        const std::vector<std::string>& getValuePropNames() const;
        
        /*! \brief Virtual destructor. */
        virtual ~TimeSeriesDataSetInfo(); 

      protected:

        ObservationDataSetInfo    m_obsDsInfo;    //!< Infos about the DataSource and DataSet that contains the time series observations    
        std::vector<std::string>  m_vlPropNames;   //!< The names of the properties that contain the time series values. 
    };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TIMESERIESDATASETINFO_H

