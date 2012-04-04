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
  \file MovingObjectIterator.h

  \brief This file contains a moving object iterator. 
 */

#ifndef __TERRALIB_ST_INTERNAL_MOVINGOBJECTITERATOR_H
#define __TERRALIB_ST_INTERNAL_MOVINGOBJECTITERATOR_H

// ST
#include "../Config.h"
#include "../observation/ObservationSet.h"

// Forward declarations
namespace te { namespace dt { class DateTime; } }
namespace te { namespace gm { class Geometry; } }

namespace te
{
  namespace st
  {
     /*!
      \class MovingObjectIterator

      \brief A class to represent a moving object iterator.

      This class provides an efficient mechanism to 
      traverse the moving object observations sequentially. 
      
      An example of use:

      MovingObjectIterator it = mo.begin();
      while(it!=mo.end())
      {
        it.getGeometry();
        it.getDateTime();
        ++it;
      }             
     
      \sa MovingObject
    */
    class TESTEXPORT MovingObjectIterator
    {
      public:

        /*! \brief Empty constructor. */
        MovingObjectIterator(); 

        /*! 
          \brief Constructor.
          
          \param it           A iterator that points to the moving object observations
          \param geomPropIdx  The index of the geometry property of the moving object observations  
        */
        MovingObjectIterator(TimeToObservationMap::iterator it, int geomPropIdx = 0);

        
        /*! \brief Assignment operator. */        
        const MovingObjectIterator& operator=(const MovingObjectIterator& rhs);
        
        /*! \brief Operator == */        
        bool operator==(const MovingObjectIterator& rhs);

        /*! \brief Operator != */        
        bool operator!=(const MovingObjectIterator& rhs);

       /*! 
          \brief Prefix operator ++ 
          
          After using this operator, the internal cursor points
          to the next moving object observation.   
          
          The semantics of prefix is this: Increment the value and then fetch it.       
        */        
        MovingObjectIterator& operator++();

        /*! 
          \brief Postfix operator ++ 
          
          After using this operator, the internal cursor points
          to the next moving object observation.   
          
          The semantics of postfix is: Fetch the value and then increment the original.       
        */        
        MovingObjectIterator operator++(int aux);

        /*! 
          \brief Prefix operator -- 
          
          After using this operator, the internal cursor points
          to the preceding moving object observation.  

          The semantics of prefix is this: Increment the value and then fetch it.   
        */  
        MovingObjectIterator& operator--();

        /*! 
          \brief Postfix operator -- 
          
          After using this operator, the internal cursor points
          to the preceding moving object observation.  

          The semantics of postfix is: Fetch the value and then increment the original.      
        */  
        MovingObjectIterator operator--(int aux);

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
        virtual ~MovingObjectIterator() { }  

      private:

        TimeToObservationMap::iterator  m_it;             //!< The internal cursor 
        int                             m_geomPropIdx;    //!< The geometry attribute index of the moving object observation set.
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_MOVINGOBJECTITERATOR_H

