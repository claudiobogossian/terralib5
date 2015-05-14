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
  \file PointCoverageIterator.h

  \brief  This file contains an iterator that is able to traverse all observations
          of a point coverage.  
*/

#ifndef __TERRALIB_ST_INTERNAL_POINTCOVERAGEITERATOR_H
#define __TERRALIB_ST_INTERNAL_POINTCOVERAGEITERATOR_H

// ST
#include "../../Config.h"
#include "PointCoverageObservation.h"

// STL
#include <map>

// Boost
#include <boost/shared_ptr.hpp>

// Forward declarations
namespace te { namespace dt { class DateTime; } }
namespace te { namespace gm { class Point; } }

namespace te
{
  namespace st
  {
     /*!
      \class PointCoverageIterator

      \brief A class to traverse the observations of a PointCoverage. 

      This class provides a mechanism to traverse the point coverage
      observations sequentially.  
      
      An example of use:

      PointCoverageIterator it = cv.begin();
      while(it!=cv.end())
      {
        Point& l = it.getLocation();
        double v = it.getDouble(i); //or getValue() or getInt or...
        ++it;
      }             
     
      \sa PointCoverage
    */
    class TESTEXPORT PointCoverageIterator
    {
      public:

        /*! \brief Empty constructor. */
        PointCoverageIterator(); 

        /*! 
          \brief Constructor.
          
          \param it           A iterator that points to the point coverage observations
        */
        PointCoverageIterator(const PointCoverageObservationSet::const_iterator& it);

        /*! \brief Assignment operator. */        
        const PointCoverageIterator& operator=(const PointCoverageIterator& rhs);
        
        /*! \brief Operator == */        
        bool operator==(const PointCoverageIterator& rhs);

        /*! \brief Operator != */        
        bool operator!=(const PointCoverageIterator& rhs);

        /*! 
          \brief Prefix operator ++ 
          
          After using this operator, the internal cursor points
          to the next point coverage observation.   
          
          The semantics of prefix is this: Increment the value and then fetch it.       
        */        
        PointCoverageIterator& operator++();

        /*! 
          \brief Postfix operator ++ 
          
          After using this operator, the internal cursor points
          to the next point coverage observation.   
          
          The semantics of postfix is: Fetch the value and then increment the original.       
        */        
        PointCoverageIterator operator++(int aux);

        /*! 
          \brief Prefix operator -- 
          
          After using this operator, the internal cursor points
          to the preceding point coverage observation.  

          The semantics of prefix is this: Increment the value and then fetch it.   
        */  
        PointCoverageIterator& operator--();

        /*! 
          \brief Postfix operator -- 
          
          After using this operator, the internal cursor points
          to the preceding point coverage observation.  

          The semantics of postfix is: Fetch the value and then increment the original.      
        */  
        PointCoverageIterator operator--(int aux);

        /*! 
          \brief Prefix operator * 
          
          It returns the item (a shared pointer to a pair of a point and a vector of values) 
          pointed by the internal cursor.       
        */        
        PointCoverageObservation operator*() const;
        
        /*! 
          \brief It returns the location pointed by the internal cursor.

          \return A reference to the location pointed by the internal cursor. 
        */  
        te::gm::Point& getLocation() const;  

        /*! 
          \brief It returns the i-th attribute value pointed by the internal cursor.

          \param i The index of the property to be returned.

          \return A pointer to the attribute value pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        te::dt::AbstractData* getValue(int i) const;
        
        /*! 
          \brief It returns the first attribute value pointed by the internal cursor.

          \return A pointer to the first attribute value pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        te::dt::AbstractData* getValue() const;  

        /*! 
          \brief It returns the i-th attribute value as a double pointed by the internal cursor.

          \param i The index of the property to be returned.
          
          \return The attribute value as a double pointed by the internal cursor. 
        */  
        double getDouble(int i) const;
        
        /*! 
          \brief It returns the first attribute value as a double pointed by the internal cursor.

          \return The attribute value as a double pointed by the internal cursor. 
        */  
        double getDouble() const;

        /*! 
          \brief It returns the i-th attribute value as an integer pointed by the internal cursor.

          \param i The index of the property to be returned.

          \return The attribute value as an integer pointed by the internal cursor. 
        */  
        int getInt(int i) const;
        
        /*! 
          \brief It returns the first attribute value as an integer pointed by the internal cursor.

          \return The first attribute value as an integer pointed by the internal cursor. 
        */  
        int getInt() const;
       
        /*! \brief Virtual destructor. */
        virtual ~PointCoverageIterator();

      private:

       PointCoverageObservationSet::const_iterator  m_it;    //!< The internal cursor 
     };
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_POINTCOVERAGEITERATOR_H


