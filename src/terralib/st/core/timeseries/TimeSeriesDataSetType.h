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
  \file TimeSeriesDataSetType.h

  \brief This file contains a class called TimeSeriesDataSetType 
*/

#ifndef __TERRALIB_ST_INTERNAL_TIMESERIESDATASETTYPE_H
#define __TERRALIB_ST_INTERNAL_TIMESERIESDATASETTYPE_H

//ST
#include "../observation/ObservationDataSetType.h"

//STL
#include <string>
#include <vector>

namespace te
{
  namespace st
  {
    /*!
      \class TimeSeriesDataSetType

      \brief  A class that contains complemental information 
              about a DataSet that holds observations of a time series. 

      A time series represents the variation of the values of a property over time. 

      \sa ObservationDataSet ObservationDataSetType
    */
    class TESTEXPORT TimeSeriesDataSetType
    {
      public:

        /*! 
          \brief Constructor. 
          
          \param tPropIdx   The index of the property that contains the times of a time series. 
          \param vPropIdx   The index of the property that contains the values of the time series.
          \param gPropIdx   The index of the property that contains the location of the time series.
          \param idPropIdx  The index of the property that contains the identity of the time series. 
          \param id         The trajectory id. 
        */
        TimeSeriesDataSetType(int tPropIdx, int vPropIdx, int gPropIdx, int idPropIdx, 
                              const std::string& id); 
        
        /*! 
          \brief Constructor. 
          
          \param tPropIdxs       The indexes of the properties that contains the beginning and final times of a time series.
          \param vPropIdxs       The indexes of the properties that contains the values of a time series.
          \param gPropIdx        The index of the property that contains the geometries of a time series. 
          \param idPropIdx       The index of the property that contains the identity of the time series. 
          \param id              The trajectory id. 
        */
        TimeSeriesDataSetType(const std::vector<int>& tPropIdxs, const std::vector<int>& vPropIdxs, 
                              int gPropIdx, int idPropIdx, const std::string& id); 
                
        /*! 
          \brief Constructor. 
          
          \param type         The ObservationSetType   
          \param vPropIdxs    The indexes of the properties that contains the values of a time series.
          \param idPropIdx    The property index that contains the time series identification. 
          \param id           The time series id. 

          \note idPropIdx equal to "-1" means that the DataSet contains observations of only one time series.
        */
        TimeSeriesDataSetType(const ObservationDataSetType& type, const std::vector<int>& vPropIdxs,
                              int idPropIdx, const std::string& id); 
               
        /*!
          \brief It returns the observation data set type.

          \return The observation set type.
        */
        const ObservationDataSetType& getType() const;
        
        /*!
          \brief It returns the property index that contains the times associated to the time series. 

          \return The property index that contains the times associated to the time series. 
        */
        int getTimePropIdx() const;

        /*!
          \brief It returns the property index that contains the begin time associated to the time series. 

          \return The property index that contains the begin times associated to the time series. 
        */
        int getBeginTimePropIdx() const;

        /*!
          \brief It returns the property index that contains the final time associated to the time series. 

          \return The property index that contains the end time associated to the time series. 
        */
        int getEndTimePropIdx() const;
        
        /*!
          \brief It returns the indexes of the properties that contains the values associated to the time series.

          \return The indexes of the properties that contains the observed values.
        */
        const std::vector<int>& getValuePropIdx() const;
        
        /*!
          \brief It returns the property index that contains the time series location. 

          \return The property index that contains the time series location.
        */
        int getGeomPropIdx() const;
        
        /*!
          \brief It returns the property index that contains the time series identifications.

          \return The property index that contains the time series identifications.
        */
        int getIdPropIdx() const;

        /*!
          \brief It sets the property index that contains the time series identifications.

          \param idx The property index that contains the time series identifications.
        */
        void setIdPropIdx(int idx);

        /*!
          \brief It returns the time series id.

          \return The time series id.
        */
        std::string getId() const;

        /*!
          \brief It sets the time series id.

          \param id The time series id.
        */
        void setId(const std::string& id);
                
        /*! \brief Virtual destructor. */
        virtual ~TimeSeriesDataSetType(); 

      protected:

        ObservationDataSetType  m_obsType;      //!< The internal observation data set type
        std::vector<int>        m_vlPropIdxs;   //!< The indexes of the properties that contain the time series values. 
        int                     m_idPropIdx;    //!< The property index that contains the identification of each time series. 
        std::string             m_id;           //!< The time series id         
     };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TIMESERIESDATASETTYPE_H

