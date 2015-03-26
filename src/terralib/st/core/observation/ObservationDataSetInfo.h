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
  \file ObservationDataSetInfo.h

  \brief This file contains a class called ObservationDataSetInfo
*/

#ifndef __TERRALIB_ST_INTERNAL_OBSERVATIONDATASETINFO_H
#define __TERRALIB_ST_INTERNAL_OBSERVATIONDATASETINFO_H

//ST
#include "../../Config.h"

//TerraLib
#include "../../../dataaccess/datasource/DataSourceInfo.h"

//STL
#include <string>
#include <vector>

namespace te { namespace da { class DataSourceInfo; } }

namespace te
{
  namespace st
  {
     /*!
      \class ObservationDataSetInfo

      \brief A class that contains infos about a DataSet that contains observations.

      This class contains information about a DataSet that
      contains observations. This information includes: 
      (1) the DataSource which it is from
      (2) the DataSet name 
      (3) the DataSet properties that contains the phenomenon time, the result time, etc.  
      
      An observation is an act associated with a discrete position in time, 
      in space or in space and time through which a number, term or other value 
      is assigned to a phenomenon. So, given a DataSet that contains observations, 
      we need to know some extra information about it, such as what columns contain 
      the phenomenon time and the observed properties.      

      \sa DataSourceInfo  
    */
    class TESTEXPORT ObservationDataSetInfo
    {
      public:
                   
        /*! 
          \brief Constructor.
          
          \param dsinfo       Infos about the DataSource. 
          \param dsName       The DataSet name. 
          \param phTimeIdx    The index of the DataSet property that contains the phenomenon time.
          \param obsPropIdx   The index of the DataSet property that is observed.
          \param gPropIdx     The index of the property that contains the spatial information.

        */
        ObservationDataSetInfo( const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                                int phTimePropIdx, int obsPropIdx, int gPropIdx = -1);  

        /*! 
          \brief Constructor.
          
          \param dsinfo       Infos about the DataSource. 
          \param dsName       The DataSet name. 
          \param phTimeIdx    The indexes of DataSet properties that contain the phenomenon time.
          \param obsPropIdx   The indexes of DataSete properties that are observed.
          \param gPropIdx     The index of the property that contains the spatial information.

        */
        ObservationDataSetInfo( const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                                const std::vector<int>& phTimeIdx, 
                                const std::vector<int>& obsPropIdx, int gPropIdx = -1);
                                
        /*! 
          \brief Constructor.
          
          \param dsinfo     Infos about the DataSource. 
          \param dsName     The DataSet name. 
          \param phTimeIdx    The indexes of DataSetType properties that contains the phenomen time.
          \param obsPropIdx   The indexes of DataSetType property that are observed.
          \param valTimeIdx   The indexes of DataSetType properties that contains the valid time.
          \param resTimeIdx   The index of DataSetType property that contains the result time.
          \param gPropIdx     The index of the property that contains the spatial information.
        */
        ObservationDataSetInfo( const te::da::DataSourceInfo& dsinfo, const std::string& dsName, 
                                const std::vector<int>& phTimeIdx, const std::vector<int>& obsPropIdx, 
                                const std::vector<int>& valTimeIdx, int resTimeIdx, int gPropIdx = -1);

        /*!
          \brief It returns the information about the DataSource

          \return A reference to the internal DataSourceInfo.
        */
        const te::da::DataSourceInfo& getDataSourceInfo() const;

         /*!
          \brief It returns the DataSet name

          \return The dataSet name
        */
        std::string getDataSetName() const;

        /*!
          \brief It sets the DataSet name

          \param name The dataSet name
        */
        void setDataSetName(const std::string& name);
        
        /*!
          \brief It returns the indexes of the DataSetType properties that contains the phenomenon time.

          \return The indexes of the DataSetType properties that contains the phenomenon time.
        */
        const std::vector<int>& getTimePropIdxs() const;

        /*!
          \brief It returns the index of the DataSet property that contains the phenomenon beginning time.

          \return The index of the DataSet property that contains the phenomenon beginning time.
        */
        int getBeginTimePropIdx() const;

        /*!
          \brief It returns the index of the DataSet property that contains the phenomenon end time.

          \return The index of the DataSet property that contains the phenomenon end time.

          \note It returns -1 when there is no a phenomenon end time.
        */
        int getEndTimePropIdx() const;

        /*!
          \brief It sets the indexes of the DataSet properties that contains the phenomenon time.

          \param idxs The indexes of the DataSet properties that contains the phenomenon time.
        */
        void setTimePropIdxs(const std::vector<int>& idxs);

        /*!
          \brief It sets the index of the DataSet property that contains the phenomenon beginning time.

          \param idx The index of the DataSet property that contains the phenomenon beginning time.
        */
        void setBeginTimePropIdx(int idx);

        /*!
          \brief It sets the index of the DataSet property that contains the phenomenon end time.

          \param idx The index of the DataSet property that contains the phenomenon end time.
        */
        void setEndTimePropIdx(int idx);

        /*!
          \brief It returns the indexes of the DataSetType properties that are observed over time. 

          \return The indexes of the DataSetType properties that are observed over time. 
        */
        const std::vector<int>& getObsPropIdxs() const;

        /*!
          \brief It sets the indexes of the DataSetType properties that are observed over time. 

          \param props The indexes of the DataSetType properties that are observed over time.
          \param gPropIdx The index of the observed property that contains the spatial information. 
        */
        void setObsPropIdxs(const std::vector<int>& props, int gPropIdx = -1);
                
        /*!
          \brief It returns the index of the property that is a geometry property.

          \return The index of the property that is a geometry property.
        */
        int getGeomPropIdx() const;

        /*!
          \brief It returns the index of the DataSet property that contains the valid beginning time.

          \return The index of the DataSet property that contains the valid beginning time.

          \note It returns -1 when there is no a valid beginning time.
        */
        int getVlBeginTimePropIdx() const;

        /*!
          \brief It returns the index of the DataSet property that contains the valid end time.

          \return The index of the DataSet property that contains the valid end time.

          \note It returns -1 when there is no a valid end time.
        */
        int getVlEndTimePropIdx() const;    
        
        /*!
          \brief It returns the indexes of the DataSet properties that contains the valid time.

          \return The indexes of the DataSet properties that contains the valid time.
        */
        const std::vector<int>& getVlTimePropIdxs() const;

        /*!
          \brief It sets the index of the DataSet property that contains the valid beginning time.

          \param idx The index of the DataSet property that contains the valid beginning time.
        */
        void setVlBeginTimePropIdx(int idx);

        /*!
          \brief It sets the index of the DataSet property that contains the valid end time.

          \param idx The index of the DataSet property that contains the valid end time.
        */
        void setVlEndTimePropIdx(int idx);
        
        /*!
          \brief It sets the indexes of the DataSet properties that contains the valid time.

          \param idxs The indexes of the DataSet properties that contains the valid time.
        */
        void setVlTimePropIdxs(const std::vector<int>& idxs);

        /*!
          \brief It returns the index of the DataSet property that contains the result time.

          \return The index of the DataSet properties that contains the result time.
        */
        int getRsTimePropIdx() const;

        /*!
          \brief It sets the index of the DataSet property that contains the result time.

          \param idx The index of the DataSet property that contains the result time.
        */
        void setRsTimePropIdx(int idx);   

        /*!
          \brief It returns an id (as a string) that identifies the DataSet.

           The id is composed of : DataSourceId + DataSetName

          \return An id that identifies the DataSet 
        */
        std::string getId() const;
                
        /*! \brief Virtual destructor. */
        virtual ~ObservationDataSetInfo(); 

      protected:

        te::da::DataSourceInfo   m_dsInfo;              //!< Information about the DataSource from which the DataSet come.
        std::string              m_dsName;              //!< The DataSet name
        std::vector<int>         m_phTimePropIdx;       //!< The indexes of DataSet properties that contain the phenomenon time.
        std::vector<int>         m_observedPropIdx;     //!< The indexes of DataSet properties that are observed. 
        int                      m_geomPropIdx;         //!< The index of the property that contains the spatial information.
        std::vector<int>         m_validTimePropIdx;    //!< The indexes of DataSet properties that contain the valid time. 
        int                      m_resultTimePropIdx;   //!< The index of DataSet property that contains the result time.         
    };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_OBSERVATIONDATASETINFO_H

