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
  \file TrajectoryIterator.h

  \brief This file contains a trajectory iterator. 
 */

#ifndef __TERRALIB_ST_INTERNAL_TRAJECTORYITERATOR_H
#define __TERRALIB_ST_INTERNAL_TRAJECTORYITERATOR_H

// ST
#include "../Config.h"
#include "MovingObjectIterator.h"

// Forward declarations
namespace te { namespace dt { class DateTime; } }
namespace te { namespace gm { class Geometry; } }

namespace te
{
  namespace st
  {
     /*!
      \class TrajectoryIterator

      \brief A class to represent a trajectory iterator.

      This class provides an efficient mechanism to 
      traverse the trajectory observations sequentially. 
      
      An example of use:

      TrajectoryIterator it = trj.begin();
      while(it!=trj.end())
      {
        it.getGeometry();
        it.getDateTime();
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
          
          \param it   A moving object iterator that points to its observations.
        */
        TrajectoryIterator(MovingObjectIterator it);

        
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
          \brief It returns the geometry pointed by the internal cursor.

          \return A pointer to the geometry pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        te::gm::Geometry* getGeometry();

        /*! 
          \brief It returns the datetime pointed by the internal cursor.

          \return A pointer to the datetime pointed by the internal cursor. 
          
          \note The caller will NOT take the ownership of the returned pointer.    
        */  
        te::dt::DateTime* getDateTime();        
               
        /*! \brief Virtual destructor. */
        virtual ~TrajectoryIterator() { }  

      private:

        MovingObjectIterator     m_it;  //!< The internal cursor
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TRAJECTORYITERATOR_H

