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
  \file TrajectoryIterator.h

  \brief This file contains a trajetory iterator. 
 */

#ifndef __TERRALIB_ST_INTERNAL_TRAJECTORYITERATOR_H
#define __TERRALIB_ST_INTERNAL_TRAJECTORYITERATOR_H

// ST
#include "../../Config.h"
#include "../../Utils.h"
#include "TrajectoryObservation.h"

// STL
#include <map>

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
      \class TrajectoryIterator

      \brief A class to traverse the observations of a trajectory. 

      This class provides a mechanism to traverse the trajectory 
      observations sequentially, ordered by time.  
      
      An example of use:

      TrajectoryIterator it = tj.begin();
      while(it!=tj.end())
      {
        Geometry* g = it.getGeometry();
        DateTime* t = it.getTime();
        ++it;
      }             
     
      \sa Trajectory
    */
    class TESTEXPORT TrajectoryIterator
    {
      public:

        /*! \brief Empty constructor. */
        TrajectoryIterator(); 

        /*! 
          \brief Constructor.
          
          \param it           A iterator that points to the trajectory observations
        */
        TrajectoryIterator(TrajectoryObservationSet::const_iterator& it);

        
        /*! \brief Assignment operator. */        
        const TrajectoryIterator& operator=(const TrajectoryIterator& rhs);
        
        /*! \brief Operator == */        
        bool operator==(const TrajectoryIterator& rhs);

        /*! \brief Operator != */        
        bool operator!=(const TrajectoryIterator& rhs);

       /*! 
          \brief Prefix operator ++ 
          
          After using this operator, the internal cursor points
          to the next trajectory observation.   
          
          The semantics of prefix is this: Increment the value and then fetch it.       
        */        
        TrajectoryIterator& operator++();

        /*! 
          \brief Postfix operator ++ 
          
          After using this operator, the internal cursor points
          to the next trajectory observation.   
          
          The semantics of postfix is: Fetch the value and then increment the original.       
        */        
        TrajectoryIterator operator++(int aux);

        /*! 
          \brief Prefix operator -- 
          
          After using this operator, the internal cursor points
          to the preceding trajectory observation.  

          The semantics of prefix is this: Increment the value and then fetch it.   
        */  
        TrajectoryIterator& operator--();

        /*! 
          \brief Postfix operator -- 
          
          After using this operator, the internal cursor points
          to the preceding trajectory observation.  

          The semantics of postfix is: Fetch the value and then increment the original.      
        */  
        TrajectoryIterator operator--(int aux);

        /*! 
          \brief Prefix operator * 
          
          It returns the item (a pair of shared pointers to the geometry and time) 
          pointed by the internal cursor.       
        */        
        TrajectoryObservation operator*() const;
        
        /*! 
          \brief It returns the geometry pointed by the internal cursor.

          \return A pointer to the geometry pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        te::gm::Geometry* getGeometry() const;

        /*! 
          \brief It returns the datetime pointed by the internal cursor.

          \return A pointer to the datetime pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        te::dt::DateTime* getTime() const;        
               
        /*! \brief Virtual destructor. */
        virtual ~TrajectoryIterator();

      private:

        TrajectoryObservationSet::const_iterator  m_it;    //!< The internal cursor 
     };

     /*!
      \struct TrajectoryPatch

      \brief A struct to represent a patch or a continuous piece of a trajectory. 
     */
     struct TrajectoryPatch
     {
        public:
          TrajectoryPatch(TrajectoryIterator b, TrajectoryIterator e) : m_begin(b), m_end(e) { }

          TrajectoryIterator begin() const { return m_begin; }

          TrajectoryIterator end() const { return m_end; }

        private:
          TrajectoryIterator m_begin; //a pointer to the first observation
          TrajectoryIterator m_end;   //a pointer to the position after the last observation
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TimeSeriesITERATOR_H

