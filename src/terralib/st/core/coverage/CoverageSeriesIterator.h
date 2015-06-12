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
  \file CoverageSeriesIterator.h

  \brief  This file contains an iterator that is able to traverse all observations
          of a coverage series.  
*/

#ifndef __TERRALIB_ST_INTERNAL_COVERAGESERIESITERATOR_H
#define __TERRALIB_ST_INTERNAL_COVERAGESERIESITERATOR_H

// ST
#include "../../Config.h"
#include "CoverageSeriesObservation.h"

// STL
#include <map>

// Boost
#include <boost/shared_ptr.hpp>

// Forward declarations
namespace te { namespace dt { class DateTime; } }

namespace te
{
  namespace st
  {
    //Forward declaration
    class Coverage;
    
     /*!
      \class CoverageSeriesIterator

      \brief A class to traverse the observations of a CoverageSeries. 

      This class provides a mechanism to traverse the observations of
      a CoverageSeries (time and coverage) ordered by time.  
      
      An example of use:

      CoverageSeriesIterator it = cvs.begin();
      while(it!=cvs.end())
      {
        DateTime* t = it.getTime();
        Coverage* c = it.getCoverage(); 
        ++it;
      }             
     
      \sa CoverageSeries
    */
    class TESTEXPORT CoverageSeriesIterator
    {
      public:

        /*! \brief Empty constructor. */
        CoverageSeriesIterator(); 

        /*! 
          \brief Constructor.
          
          \param it           A iterator that points to the point coverage observations
        */
        CoverageSeriesIterator(const CoverageSeriesObservationSet::const_iterator& it);

        /*! \brief Assignment operator. */        
        const CoverageSeriesIterator& operator=(const CoverageSeriesIterator& rhs);
        
        /*! \brief Operator == */        
        bool operator==(const CoverageSeriesIterator& rhs);

        /*! \brief Operator != */        
        bool operator!=(const CoverageSeriesIterator& rhs);

        /*! 
          \brief Prefix operator ++ 
          
          After using this operator, the internal cursor points
          to the next coverage series observation.   
          
          The semantics of prefix is this: Increment the value and then fetch it.       
        */        
        CoverageSeriesIterator& operator++();

        /*! 
          \brief Postfix operator ++ 
          
          After using this operator, the internal cursor points
          to the next coverage series observation.   
          
          The semantics of postfix is: Fetch the value and then increment the original.       
        */        
        CoverageSeriesIterator operator++(int aux);

        /*! 
          \brief Prefix operator -- 
          
          After using this operator, the internal cursor points
          to the preceding coverage series observation.  

          The semantics of prefix is this: Increment the value and then fetch it.   
        */  
        CoverageSeriesIterator& operator--();

        /*! 
          \brief Postfix operator -- 
          
          After using this operator, the internal cursor points
          to the preceding coverage series observation.  

          The semantics of postfix is: Fetch the value and then increment the original.      
        */  
        CoverageSeriesIterator operator--(int aux);

        /*! 
          \brief Prefix operator * 
          
          It returns the observation (a pair of shared pointers to DateTime and Coverage) 
          pointed by the internal cursor.       
        */        
        CoverageSeriesObservation operator*() const;
        
        /*! 
          \brief It returns the time pointed by the internal cursor.

          \return A pointer to the time pointed by the internal cursor. 

          \note The caller will NOT take the ownership of the returned pointer.
        */  
        te::dt::DateTime* getTime() const;  

        /*! 
          \brief It returns the coverage pointed by the internal cursor.

          \return A pointer to the coverage pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        te::st::Coverage* getCoverage() const;
        
        /*! \brief Virtual destructor. */
        virtual ~CoverageSeriesIterator();

      private:

       CoverageSeriesObservationSet::const_iterator  m_it;    //!< The internal cursor 
     };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_COVERAGESERIESITERATOR_H


