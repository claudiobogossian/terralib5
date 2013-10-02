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
  \file PointCoverageDataSetInfo.h

  \brief This file contains a class called PointCoverageDataSetInfo
*/

#ifndef __TERRALIB_ST_INTERNAL_POINTCOVERAGEDATASETINFO_H
#define __TERRALIB_ST_INTERNAL_POINTCOVERAGEDATASETINFO_H

//ST
#include "../../Config.h"
#include "../observation/ObservationDataSetInfo.h"
#include "CoverageDataSetInfo.h"

//Forward declarations
namespace te { namespace dt { class DateTime; } }

namespace te
{
  namespace st
  {
    
    /*!
      \class PointCoverageDataSetInfo

      \brief  A class that contains infos about a DataSource and DataSet 
              that contains observations of a point coverage.

      \sa ObservationDataSetInfo  
    */
    class TESTEXPORT PointCoverageDataSetInfo : public CoverageDataSetInfo
    {
      public:
        
        /*! 
          \brief Constructor.
          
          A user must use this constructor when the DataSet has a property that contain a date and time
          associated to each observation.

          \param dsinfo     Infos about the DataSource. 
          \param dsName     The DataSet name.   
          \param tPropIdx   The property index that contains the times associated to the coverage observations.
          \param ptPropIdx  The property index that contains the points associated to the coverage observations.
          \param vlPropIdx  The property index that contains the values of the coverage. 
          \param time       The date and time associated to the entire point coverage.
        */
        PointCoverageDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                              int tPropIdx, int ptPropIdx, int vlPropIdx, te::dt::DateTime* time);

        /*! 
          \brief Constructor.

          A user must use this constructor when the DataSet has NOT a property that contain a date and time
          associated to each observation.
          
          \param dsinfo     Infos about the DataSource. 
          \param dsName     The DataSet name.   
          \param ptPropIdx  The property index that contains the points associated to the coverage.
          \param vlPropIdx  The property index that contains the values of the coverage.  
          \param time       The date and time associated to the entire point coverage.

          \note It will take the ownership of the given pointers.
        */
        PointCoverageDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                              int ptPropIdx, int vlPropIdx, te::dt::DateTime* time);

        /*! 
          \brief Constructor.

          A user must use this constructor when the DataSet has a property that contain a date and time
          associated to each observation.
          
          \param dsinfo     Infos about the DataSource. 
          \param dsName     The DataSet name.   
          \param tPropIdx   The property indexes that contains the times associated to the coverage observations.
          \param ptPropIdx  The property index that contains the points associated to the coverage observations.
          \param vlPropIdx  The property index that contains the values of the coverage observations.  
          \param time       The date and time associated to the entire point coverage.
        */
        PointCoverageDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                              const std::vector<int>& tPropIdx, int ptPropIdx, const std::vector<int>& vlPropIdx, te::dt::DateTime* time);

        /*! 
          \brief Constructor.

          A user must use this constructor when the DataSet has NOT a property that contain a date and time
          associated to each observation.
          
          \param dsinfo     Infos about the DataSource. 
          \param dsName     The DataSet name.   
          \param ptPropIdx  The property index that contains the points associated to the coverage observations.
          \param vlPropIdx  The property index that contains the values of the coverage observations.  
          \param time       The date and time associated to the entire point coverage.

          \note It will take the ownership of the given pointers.
        */
        PointCoverageDataSetInfo(const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                              int ptPropIdx, const std::vector<int>& vlPropIdx, te::dt::DateTime* time);
        
        /*! 
          \brief Constructor.
          
          \param info       Infos about the DataSource and DataSet that contains the coverage observations    
          \param vlPropIdx  The property index that contains the values of the coverage.  
          \param time       The date and time associated to the entire point coverage.

          \note It will take the ownership of the given pointers.
        */
        PointCoverageDataSetInfo( const ObservationDataSetInfo& info, const std::vector<int>& vlPropIdxs, 
                                  te::dt::DateTime* time);
                
         /*! \name Inhered methods*/
        //@{        
        te::st::CoverageType getCoverageType() const ;
        
        const ObservationDataSetInfo& getObservationDataSetInfo() const;
        
        const te::da::DataSourceInfo& getDataSourceInfo() const;
        
        const std::vector<int>& getTimePropIdxs() const;
        //@}

        /*!
          \brief It returns the property index that contains the points of the coverage. 

          \return The property index that contains the points of the coverage. 
        */
        int getPointPropIdx() const;
        
        /*!
          \brief It returns the property indexes that contains the values of the coverage. 

          \return The property indexes that contains the values of the coverage. 
        */
        const std::vector<int>& getValuePropIdxs() const;
        
        /*!
          \brief It returns the date and time associated to the entire point coverage.

          \return The date and time associated to the point coverage. 
        */
        std::auto_ptr<te::dt::DateTime> getTime() const;

        
        /*! \brief Virtual destructor. */
        virtual ~PointCoverageDataSetInfo(); 

      protected:

        ObservationDataSetInfo          m_obsDsInfo;  //!< Infos about the DataSource and DataSet that contains the point coverage observations    
        std::vector<int>                m_vlPropIdx;  //!< The indexes of the properties that contain the point coverage values. 
        std::auto_ptr<te::dt::DateTime> m_time;    //! The date and time associated to this point coverage, when this information is not in the DataSet
     };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_POINTCOVERAGEDATASETINFO_H

