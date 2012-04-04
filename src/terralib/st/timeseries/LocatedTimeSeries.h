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
  \file LocatedTimeSeries.h

  \brief This file contains a class to represent a located time series
 */

#ifndef __TERRALIB_ST_INTERNAL_LOCATEDTIMESERIES_H
#define __TERRALIB_ST_INTERNAL_LOCATEDTIMESERIES_H

//ST
#include "../Config.h"

//STL
#include <map>
#include <string>
#include <vector>

// Forward declarations
namespace te { namespace gm { class Geometry; } }

namespace te
{
  namespace st
  {
    // Forward declarations
    class TimeSeries;

    /*!
      \class LocatedTimeSeries

      \brief A class to represent a located time series.
      
      A located time series is a set of time series associated to 
      a spatial location.

      \sa TimeSeries Geometry
    */
    class TESTEXPORT LocatedTimeSeries
    {
      public:

        /*! \brief Constructor. */
        LocatedTimeSeries(); 

        /*! 
          \brief Constructor. 
          
          \param timeSeries   A time series associated to the spatial location.
          \param spLocation   The spatial location.
          \param id           An identification.

          \note It will take the ownership of the input pointers.
        */
        LocatedTimeSeries(TimeSeries* timeSeries, te::gm::Geometry* spLocation, int id = -1);

        /*! 
          \brief Constructor. 
          
          \param timeSeries   A set of time series associated to the spatial location.
          \param spLocation   The spatial location.
          \param id           An identification.

          \note It will take the ownership of the input pointers.
        */
        LocatedTimeSeries(const std::vector<TimeSeries*>& timeSeries, te::gm::Geometry* spLocation, int id = -1);

        /*!
          \brief It returns the set of time series associated to the spatial location.

          \return The time series set.

          \note The caller will NOT take the ownership of the returned pointers. 
        */
        std::vector<TimeSeries*>& getTimeSeriesSet();

        /*!
          \brief It returns idx-th time series associated to the spatial location.

          \param idx  The idx-th time series.
          
          \return The idx-th time series.

          \note The caller will NOT take the ownership of the returned pointers. 
        */
        TimeSeries* getTimeSeries(int idx = 0) const;

        /*!
          \brief It returns the time series with a given property name.

          \param name  The time series property name.
          
          \return The time series with a given property name.

          \note The caller will NOT take the ownership of the returned pointers. 
        */
        TimeSeries* getTimeSeries(const std::string& name) const;

        /*!
          \brief It sets the time series.

          \param ts The time series set.

          \note It will take the ownership of the input pointers.
        */
        void setTimeSeries(const std::vector<TimeSeries*>& ts);

        /*!
          \brief It adds a time series.

          \param ts The time series to be added.

          \note It will take the ownership of the input pointer.
        */
        void addTimeSeries(TimeSeries* ts);

        /*!
          \brief It returns its spatial location.

          \return A pointer to its spatial location.
          
          \note The caller will NOT take the ownership of the returned pointer.
        */
        te::gm::Geometry* getSpatialLocation();
        
        /*!
          \brief It sets its spatial location.

          \param spLocation Its spatial location.

          \note It will take the ownership of the input pointer.
        */
        void setSpatialLocation(te::gm::Geometry* spLocation);   
        
        /*!
          \brief It returns its identifier.

          \return Its identifier.
        */
        int getId();
        
        /*!
          \brief It sets its identifier.

          \param id Its identifier.
        */
        void setId(int id);   

        /*!
          \brief It adds an observation to a specific time series.

          \param propName The time series property name 
          \param phTime   The phenomenon time. 
          \param obsValue The observed value.
         
          \note It will take the ownership of the input pointer.
        */
        void addObservation(const std::string& propName, te::dt::DateTime* phTime, te::dt::AbstractData* obsValue);

        /*! \brief Virtual destructor. */
        virtual ~LocatedTimeSeries(); 

      private:

        std::vector<TimeSeries*>  m_timeSeries;   //!< A vector of time series collected by the geosensor
        te::gm::Geometry*         m_spLocation;   //!< Its spatial location       
        int                       m_Id;           //!< Its identification
    };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_LOCATEDTIMESERIES_H

