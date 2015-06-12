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
  \file terralib/datatype/AbstractData.h

  \brief A base class for objects that can be retrieved from the data access module.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_ABSTRACTDATA_H
#define __TERRALIB_DATATYPE_INTERNAL_ABSTRACTDATA_H

// TerraLib
#include "Config.h"

// STL
#include <string>

// Boost
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace dt
  {
    /*!
      \class AbstractData

      \brief A base class for values that can be retrieved from the data access module.

      This class provides the basic extensibility for data types in the
      data access module. Through implementing this abstract class you can add
      new data types to the system and then you can register it in the data source driver.

      \ingroup datatype

      \sa DataType, ByteArray, DateTime, SimpleData, CompositeData

      \todo Add a serialize/deserialize API
    */
    class TEDATATYPEEXPORT AbstractData
    {
      public:

        /*! \brief Constructor. */
        AbstractData() {}

        /*! \brief Virtual destructor. */
        virtual ~AbstractData() {}

        /*!
          \brief It returns a clone of this object.
          
          \return A clone of this object.
        */
        virtual AbstractData* clone() const = 0;

        /*!
          \brief It returns the data type code associated to the data value.

          \return The data type code associated to the data value.
        */
        virtual int getTypeCode() const = 0;

        /*!
          \brief It returns the data value in a string notation.

          \return The data value in a string notation.
        */
        virtual std::string toString() const = 0;
    };

    /*!
      \brief For use with boost conteiners.
    */
    inline AbstractData* new_clone(const AbstractData& a)
    {
      return a.clone();
    }
	
	//Typedef 
    typedef boost::shared_ptr<AbstractData>       AbstractDataShrPtr;

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_ABSTRACTDATA_H

