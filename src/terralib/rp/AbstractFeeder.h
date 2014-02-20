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
  \file terralib/rp/AbstractFeeder.h
  \brief Abstract objects feeder.
 */

#ifndef __TERRALIB_RP_INTERNAL_ABSTRACTFEEDER_H
#define __TERRALIB_RP_INTERNAL_ABSTRACTFEEDER_H

#include "Config.h"

#include <boost/noncopyable.hpp>

namespace te
{
  namespace rp
  {
    /*!
      \class AbstractFeeder
      \brief Abstract objects feeder.
      \details Sequential objects feeder to image processing algorithms.
     */
    template< typename ObjType >
    class AbstractFeeder : private boost::noncopyable
    {
      public :
        
        AbstractFeeder() {};
        
        virtual ~AbstractFeeder() {};
        
        /*! 
          \brief Return the current sequence object.
          \return A pointer to the current object or a null pointer if there are no object to return.
        */
        virtual ObjType* getCurrentObj() const = 0; 

        /*! 
          \brief Advances to the next sequence obeject. 
          \return true if the advance was completed or false if the objects sequence end was reached.
          \note This method can invalidate any pointer returned by getCurrentObj.
        */
        virtual bool moveNext() = 0;
        
        /*! 
          \brief Jump to the given object index. 
          \return true if the jump was completed or false on errors.
          \note This method can invalidate any pointer returned by getCurrentObj.
        */
        virtual bool moveTo( const unsigned int index ) = 0;        
        
        /*! 
          \brief Reset the feeder to the first position (subsequent accesses will start from the first sequence obejct).
          \note This method can invalidate any pointer returned by getCurrentObj.
        */        
        virtual void reset() = 0;

        /*! 
          \brief Return the total number of feeder objects.
          \return The total number of feeder objects.
        */
        virtual unsigned int getObjsCount() const = 0;

        /*! 
          \brief Return the index of the current object.
          \return The index of the current object.
        */
        virtual unsigned int getCurrentOffset() const = 0;
    };
  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_ABSTRACTFEEDER_H

