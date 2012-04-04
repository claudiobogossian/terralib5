/*  Copyright (C) 2010-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/datatype/AbstractDataConverter.h

  \brief A base class for data type conversion operation.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_ABSTRACTDATACONVERTER_H
#define __TERRALIB_DATATYPE_INTERNAL_ABSTRACTDATACONVERTER_H

// TerraLib
#include "Config.h"
#include "Exception.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace dt
  {
// Forward declaration
    class AbstractData;

    /*!
      \class AbstractDataConverter

      \brief A base class for data type conversion operation.

      This is a helper class that can be used
      to guide applications to convert data types
      to the right types.

      In the data access module each data source
      can publish its list of conversion operations
      and hence be used by applications to guide
      the process of data conversion.

      \sa AbstractData, DataConverterManager
    */
    class TEDATATYPEEXPORT AbstractDataConverter : public boost::noncopyable
    {
      public:

        /*! \brief Constructor. */
        AbstractDataConverter() { }

        /*! \brief Virtual destructor. */
        virtual ~AbstractDataConverter() { }

        /*!
          \brief It converts the input data value (d) to a new one having a new data type.

          \param d The input data value.

          \return A new value converted to a new data type. The caller will take the ownership of the returned data.

          \exception Exception It throws an exception if the input abstract data is not of the right type.
        */
        virtual AbstractData* convert(const AbstractData* d) const throw(Exception) = 0;

        /*!
          \brief It returns the data type code of input data.

          \return The data type code of input data.
        */
        virtual int getInputDataTypeCode() const = 0;

        /*!
          \brief It returns the data type code of output data.

          \return The data type code of output data.
        */
        virtual int getOutputDataTypeCode() const = 0;
    };

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_ABSTRACTDATACONVERTER_H

