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
  \file TimeSeriesObservation.h

  \brief This file contains a class to represent a time series observation.
*/

#ifndef __TERRALIB_ST_INTERNAL_TIMESERIESOBSERVATION_H
#define __TERRALIB_ST_INTERNAL_TIMESERIESOBSERVATION_H

//TerraLib
#include "../../../datatype/DateTime.h"
#include "../../../datatype/AbstractData.h"

//ST
#include "../../Config.h"

//STL
#include <vector>

//Boost
#include <boost/shared_ptr.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/mem_fun.hpp>

// Forward declarations
namespace te { namespace dt { class AbstractData; class DateTime; } }

namespace te
{
  namespace st
  {    
    /*!
      \class TimeSeriesObservation.

      \brief A class to represent an observation (time and value) of a time series.

      \sa TimeSeries
    */
    class TESTEXPORT TimeSeriesObservation
    {
       public:
         
         /*! 
            \brief Constructor. 
                    
            \param t       A date time.
            \param v       A value.
                     
            \note It will take the ownership of the input pointers.
         */
         TimeSeriesObservation(te::dt::DateTime* t, te::dt::AbstractData* v);

         /*! \brief It returns the date time. 
            
            \note The caller will not take the ownership of the returned pointer. 
         */
         te::dt::DateTime* getTime() const;

         /*! \brief It sets the date time. 
            
            \note The caller will take the ownership of the returned pointer. 
         */
         void setTime(te::dt::DateTime* t);

         /*! \brief It returns the value. 
            
            \note The caller will not take the ownership of the returned pointer. 
         */
         te::dt::AbstractData* getValue() const; 

         /*! \brief It returns the value as a double.  */
         double getDouble() const;

         /*! \brief It returns the value as a integer. */
         int getInt() const;

          /*! \brief It returns the value as a string. */
         std::string getString() const;

         /*! \brief Operator <. 
             
             It returns TRUE if the internal time is less than the time of the object "other". 
             Otherwise, it returns FALSE.
         */
         bool operator<(const TimeSeriesObservation& other) const;

         /*! 
            \brief Destructor.
         */
         ~TimeSeriesObservation();

       protected:
          
          te::dt::DateTimeShrPtr      m_time;
          te::dt::AbstractDataShrPtr  m_value;
    };

    //typedef TimeSeriesObservationSet
    typedef boost::multi_index_container<
            TimeSeriesObservation,
            boost::multi_index::indexed_by<
                //(0) indexed by time using the operator <
                boost::multi_index::ordered_unique<
                      boost::multi_index::identity<TimeSeriesObservation> >, 
                
                //(1) indexed by value as double
                boost::multi_index::ordered_non_unique< 
                      boost::multi_index::const_mem_fun<TimeSeriesObservation, double, &TimeSeriesObservation::getDouble> >,

                //(2) indexed by value as string
                boost::multi_index::ordered_non_unique< 
                      boost::multi_index::const_mem_fun<TimeSeriesObservation, std::string, &TimeSeriesObservation::getString> >

            > > TimeSeriesObservationSet;

    typedef TimeSeriesObservationSet::nth_index<0>::type TimeSeriesByTime;

    typedef TimeSeriesObservationSet::nth_index<1>::type TimeSeriesByValue;

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TIMESERIESOBSERVATION_H

