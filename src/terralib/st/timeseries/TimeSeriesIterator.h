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
  \file TimeSeriesIterator.h

  \brief This file contains a time series iterator. 
 */

#ifndef __TERRALIB_ST_INTERNAL_TIMESERIESITERATOR_H
#define __TERRALIB_ST_INTERNAL_TIMESERIESITERATOR_H

// ST
#include "../Config.h"
#include "../observation/ObservationSet.h"

// Forward declarations
namespace te { namespace dt { class DateTime; } }

namespace te
{
  namespace st
  {
     /*!
      \class TimeSeriesIterator

      \brief A class to represent a time series iterator.

      This class provides an efficient mechanism to 
      traverse the time series observations sequentially. 
      
      An example of use:

      TimeSeriesIterator it = ts.begin();
      while(it!=ts.end())
      {
        DateTime* t = it.getDateTime();
        AbstractData* value = it.getObservedValue();
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
          
          \param it       A iterator that points to the moving object observations
          \param propIdx  The index of the time series observed property 
        */
        TimeSeriesIterator(TimeToObservationMap::iterator it, int propIdx);
        
        /*! \brief Assignment operator. */        
        const TimeSeriesIterator& operator=(const TimeSeriesIterator& rhs);
        
        /*! \brief Operator == */        
        bool operator==(const TimeSeriesIterator& rhs);

        /*! \brief Operator != */        
        bool operator!=(const TimeSeriesIterator& rhs);

       /*! 
          \brief Prefix operator ++ 
          
          After using this operator, the internal cursor points
          to the next observation of the time series.   
          
          The semantics of prefix is this: Increment the value and then fetch it.       
        */        
        TimeSeriesIterator& operator++();

        /*! 
          \brief Postfix operator ++ 
          
          After using this operator, the internal cursor points
          to the next observation of the time series.   
          
          The semantics of postfix is: Fetch the value and then increment the original.       
        */        
        TimeSeriesIterator operator++(int aux);

        /*! 
          \brief Prefix operator -- 
          
          After using this operator, the internal cursor points
          to the preceding observation of the time series.     

          The semantics of prefix is this: Increment the value and then fetch it.   
        */  
        TimeSeriesIterator& operator--();

        /*! 
          \brief Postfix operator -- 
          
          After using this operator, the internal cursor points
          to the preceding observation of the time series.     

          The semantics of postfix is: Fetch the value and then increment the original.      
        */  
        TimeSeriesIterator operator--(int aux);

        /*! 
          \brief It returns the datetime pointed by the internal cursor.

          \return A pointer to the datetime pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        te::dt::DateTime* getDateTime();  
        
        /*! 
          \brief It returns the observed value pointed by the internal cursor.

          \return A pointer to the observed value pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        te::dt::AbstractData* getObservedValue();      
               
        /*! \brief Virtual destructor. */
        virtual ~TimeSeriesIterator() { }  

      private:

        TimeToObservationMap::iterator  m_it;  //!< The internal cursor 
        int                             m_propIdx;   //!< The observed property index of the time series observation set.
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TIMESERIESITERATOR_H

