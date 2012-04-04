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
  \file ObservationSetIterator.h

  \brief This file contains an iterator over a set of observations. 
*/

#ifndef __TERRALIB_ST_INTERNAL_OBSERVATIONSETITERATOR_H
#define __TERRALIB_ST_INTERNAL_OBSERVATIONSETITERATOR_H

//ST
#include "../Config.h"
#include "ObservationSet.h"

// Forward declarations
namespace te { namespace dt { class DateTime; class AbstractData;} }

namespace te
{
  namespace st
  {
     // Forward declarations
     class Observation; 

     /*!
      \class ObservationSetIterator

      \brief A class to represent an iterator over a set of observations.

      This class provides an efficient mechanism to 
      traverse a set of observations sequentially. 
      
      An example of use:

      ObservationSetIterator it = obs.begin();
      while(it!=obs.end())
      {
        Observation* ob = it.getObservation();
        DateTime* t = it.getDateTime();
        vector<AbstractData>& values = it.getObservedValues();
        AbstractData* value = it.getObservedValue(idx);
        ++it;
      }             
     
      \sa ObservationSet
    */
    class TESTEXPORT ObservationSetIterator
    {
      public:

        /*! \brief Empty constructor. */
        ObservationSetIterator(); 

        /*! 
          \brief Constructor.
          
          \param it A iterator that points to the observation set
        */
        ObservationSetIterator(TimeToObservationMap::iterator it);
        
        /*! \brief Assignment operator. */        
        const ObservationSetIterator& operator=(const ObservationSetIterator& rhs);
        
        /*! \brief Operator == */        
        bool operator==(const ObservationSetIterator& rhs);

        /*! \brief Operator != */        
        bool operator!=(const ObservationSetIterator& rhs);

        /*! 
          \brief Prefix operator ++ 
          
          After using this operator, the internal cursor points
          to the next observation of the set.   
          
          The semantics of prefix is this: Increment the value and then fetch it.       
        */        
        ObservationSetIterator& operator++();

        /*! 
          \brief Postfix operator ++ 
          
          After using this operator, the internal cursor points
          to the next observation of the set.   
          
          The semantics of postfix is: Fetch the value and then increment the original.       
        */        
        ObservationSetIterator operator++(int aux);

        /*! 
          \brief Prefix operator -- 
          
          After using this operator, the internal cursor points
          to the preceding observation of the set.  

          The semantics of prefix is this: Increment the value and then fetch it.   
        */  
        ObservationSetIterator& operator--();

        /*! 
          \brief Postfix operator -- 
          
          After using this operator, the internal cursor points
          to the preceding observation of the set.  

          The semantics of postfix is: Fetch the value and then increment the original.      
        */  
        ObservationSetIterator operator--(int aux);

        /*! 
          \brief It returns the observation pointed by the internal cursor.

          \return A pointer to the observation pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        Observation* getObservation();

        /*! 
          \brief It returns the phenomenon datetime of the observation pointed by the internal cursor.

          \return A pointer to the phenomenon datetime of the observation pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        te::dt::DateTime* getDateTime();   

        /*! 
          \brief It returns the observed values of the observation pointed by the internal cursor.

          \return A reference to the observed values of the observation pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        std::vector<te::dt::AbstractData*> getObservedValues();

         /*! 
          \brief It returns the idx-th observed value of the observation pointed by the internal cursor.

          \return A pointer to the idx-th observed value of the observation pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        te::dt::AbstractData* getObservedValue(int idx=0);             
               
        /*! \brief Virtual destructor. */
        virtual ~ObservationSetIterator() { }  

      private:

        TimeToObservationMap::iterator  m_it;  //!< The internal cursor 
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_OBSERVATIONSETITERATOR_H

