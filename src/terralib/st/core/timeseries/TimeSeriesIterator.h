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
  \file TimeSeriesIterator.h

  \brief This file contains a time series iterator. 
 */

#ifndef __TERRALIB_ST_INTERNAL_TIMESERIESITERATOR_H
#define __TERRALIB_ST_INTERNAL_TIMESERIESITERATOR_H

// ST
#include "../../Config.h"
#include "TimeSeriesObservation.h"

// STL
#include <vector>
#include <memory>

// Boost
#include <boost/shared_ptr.hpp>

// Forward declarations
namespace te { namespace dt { class DateTime; } }
namespace te { namespace gm { class Geometry; } }

namespace te
{
  namespace st
  {
     /*!
      \class TimeSeriesIterator

      \brief A class to traverse the observations of a TimeSeries. 

      This class provides a mechanism to traverse the time series
      observations sequentially, ordered by time.  
      
      An example of use:

      TimeSeriesIterator it = tj.begin();
      while(it!=tj.end())
      {
        DateTime* t = it.getTime();
        double v = it.getDouble(); //or getValue() or getInt or...
        ++it;
      }             
     
      \sa TimeSeries
    */
    class TESTEXPORT TimeSeriesIterator
    {
      public:

        /*! \brief Empty constructor. */
        TimeSeriesIterator(); 

        /*! 
          \brief Constructor.
          
          \param it           A iterator that points to the TimeSeries observations
        */
        TimeSeriesIterator(TimeSeriesObservationSet::const_iterator& it);

        /*! \brief Assignment operator. */        
        const TimeSeriesIterator& operator=(const TimeSeriesIterator& rhs);
        
        /*! \brief Operator == */        
        bool operator==(const TimeSeriesIterator& rhs);

        /*! \brief Operator != */        
        bool operator!=(const TimeSeriesIterator& rhs);

        /*! 
          \brief Prefix operator ++ 
          
          After using this operator, the internal cursor points
          to the next TimeSeries observation.   
          
          The semantics of prefix is this: Increment the value and then fetch it.       
        */        
        TimeSeriesIterator& operator++();

        /*! 
          \brief Postfix operator ++ 
          
          After using this operator, the internal cursor points
          to the next TimeSeries observation.   
          
          The semantics of postfix is: Fetch the value and then increment the original.       
        */        
        TimeSeriesIterator operator++(int aux);

        /*! 
          \brief Prefix operator -- 
          
          After using this operator, the internal cursor points
          to the preceding TimeSeries observation.  

          The semantics of prefix is this: Increment the value and then fetch it.   
        */  
        TimeSeriesIterator& operator--();

        /*! 
          \brief Postfix operator -- 
          
          After using this operator, the internal cursor points
          to the preceding TimeSeries observation.  

          The semantics of postfix is: Fetch the value and then increment the original.      
        */  
        TimeSeriesIterator operator--(int aux);

        /*! 
          \brief Prefix operator * 
          
          It returns the observation (a pair of shared pointers to the time and value) 
          pointed by the internal cursor.       
        */        
        TimeSeriesObservation operator*() const;
        
        /*! 
          \brief It returns the datetime pointed by the internal cursor.

          \return A pointer to the datetime pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        te::dt::DateTime* getTime() const;  

        /*! 
          \brief It returns the attribute value pointed by the internal cursor.

          \return A pointer to the attribute value pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        te::dt::AbstractData* getValue() const;  

        /*! 
          \brief It returns the attribute value as a double pointed by the internal cursor.

          \return A pointer to the attribute value as a double pointed by the internal cursor. 
        */  
        double getDouble() const;

        /*! 
          \brief It returns the attribute value as an integer pointed by the internal cursor.

          \return A pointer to the attribute value as an integer pointed by the internal cursor. 
        */  
        int getInt() const;

        /*! 
          \brief It returns the attribute value as a string pointed by the internal cursor.

          \return A pointer to the attribute value as a string pointed by the internal cursor. 
        */  
        std::string getString() const;
                      
               
        /*! \brief Virtual destructor. */
        virtual ~TimeSeriesIterator();

      private:

       TimeSeriesObservationSet::const_iterator  m_it;    //!< The internal cursor 
     };

     /*!
      \struct TimeSeriesPatch

      \brief A struct to represent a patch or a continuous piece of a time series. 
     */
     struct TimeSeriesPatch
     {
        public:
          TimeSeriesPatch(TimeSeriesIterator b, TimeSeriesIterator e) : m_begin(b), m_end(e) { }

          TimeSeriesIterator begin() const { return m_begin; }

          TimeSeriesIterator end() const { return m_end; }

        private:
          TimeSeriesIterator m_begin; //a pointer to the first observation
          TimeSeriesIterator m_end;   //a pointer to the position after the last observation
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TIMESERIESITERATOR_H


