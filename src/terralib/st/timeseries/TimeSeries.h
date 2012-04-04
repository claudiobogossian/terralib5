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
  \file TimeSeries.h

  \brief This file contains a class to represent a time series.
 */

#ifndef __TERRALIB_ST_INTERNAL_TIMESERIES_H
#define __TERRALIB_ST_INTERNAL_TIMESERIES_H

// TerraLib
#include "../../datatype/Enums.h"

//STL
#include <vector>

// DyGeo
#include "../Config.h"

// Forward declarations
namespace te { namespace dt { class AbstractData; class DateTime; class Property;} }

namespace te
{
  namespace st
  {
    // Forward declarations
    class ObservationSet;
    class AbstractInterpolator;
    class TimeSeriesIterator;

    /*!
      \class Time Series

      \brief A class to represent time series.

      A time series represents a variation of a property over time, where 
      this property is of a textual or numerical type (T -> A). 
      It is composed of a set of observations and of an interpolator able
      to estimate values in non-observed times.

      \TODO operations: +, -, *, max, min, etc...

      \sa ObservationSet AbstractInterpolator
    */
    class TESTEXPORT TimeSeries
    {
      public:

        /*! \name TimeSeries Constructors */
        //@{
        
        /*! \brief Constructor. */
        TimeSeries(); 

        /*! 
          \brief Constructor. 
          
          \param obs      A pointer to time series observations.
          \param propIdx  The observation set property index that will be taken as the time series property.

          \note It will take the ownership of the input pointer.
        */
        TimeSeries(ObservationSet* obs, int propIdx = 0);

        /*! 
          \brief Constructor. 
          
          \param obs      A pointer to time series observations.
          \param propIdx  The observation set property index that will be taken as the time series property.
          \param interp   A pointer to an interpolator.

          \note It will take the ownership of the first input pointer (ObservationSet* ).
          \note It will NOT take the ownership of the last input pointer (AbstractInterpolator*).
        */
        TimeSeries(ObservationSet* obs, int propIdx, AbstractInterpolator* interp);

        /*! 
          \brief Constructor. 
          
          \param prop     The observed property.  
        
          \note It will take the ownership of the input pointer.
        */
        TimeSeries(te::dt::Property* prop);
        //@}

        /*!
          \brief It returns a time series iterator that points to its first observation.

          \return A time series iterator.        
        */
        TimeSeriesIterator begin();

        /*!
          \brief It returns a time series iterator that points to the end of its observation set.

          \return A time series iterator.        
        */
        TimeSeriesIterator end();

        /*!
          \brief It sets the time series observations.

          \param obs      A pointer to the time series observations.
          \param attrIdx  The observation set property index that will be taken as the time series property.

          \note It will take the ownership of the input pointer.
         */
        void setObservationSet(ObservationSet* obs, int attrIdx = 0);
        
        /*!
          \brief It adds an observation to time series.

          \param phTime   The phenomenon time. 
          \param obsValue The observed value.
         
          \note It will take the ownership of the input pointer.
        */
        void addObservation(te::dt::DateTime* phTime, te::dt::AbstractData* obsValue);

        /*!
          \brief It returns the size of the time series observation set.

          \return The observation set size of the time series.
        */
        std::size_t size() const;        

        /*!
          \brief It returns the value associated to a given date and time.

          \param t  A date and time.
           
          \return   A pointer to the value associated to the given date and time.

          \note The caller will not take the ownership of the returned pointer.
        */
        te::dt::AbstractData* getValue(te::dt::DateTime* t);

        /*!
          \brief It returns the interpolator associated to the time series

          \return A pointer the interpolator associated to the time series
        */
        AbstractInterpolator* getInterpolator() const;

        /*!
          \brief It sets the interpolator associated to the time series

          \param interp A interpolator which is able to estimate values

          \note It will NOT take the ownership of the input pointer.
        */
        void setInterpolator(AbstractInterpolator* interp);

         /*!
          \brief It returns the time series property.

          \return The time series property.

          \note The caller will not take the ownership of the returned pointer.
        */
        te::dt::Property* getProperty();

        
        /*! \brief Virtual destructor. */
        virtual ~TimeSeries(); 

      private:

        ObservationSet*          m_observations;    //!< The time series observations 
        int                      m_propIdx;         //!< The time series property index 
        AbstractInterpolator*    m_interpolator;    //!< The interpolators used to estimate non-observed times
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TIMESERIES_H

