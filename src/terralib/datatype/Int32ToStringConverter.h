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
  \file terralib/datatype/Int32ToStringConverter.h

  \brief A converter from Int32 data values to String.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_INT32TOSTRINGCONVERTER_H
#define __TERRALIB_DATATYPE_INTERNAL_INT32TOSTRINGCONVERTER_H

// TerraLib
#include "AbstractDataConverter.h"

namespace te
{
  namespace dt
  {
    /*!
      \class Int32ToStringConverter

      \brief A converter from Int32 data values to String.

      \sa AbstractDataConverter, DataConverterManager
    */
    class TEDATATYPEEXPORT Int32ToStringConverter : public AbstractDataConverter
    {
      public:

        /*! \brief Constructor. */
        Int32ToStringConverter() { }

        /*! \brief Virtual destructor. */
        ~Int32ToStringConverter() { }

        /*!
          \brief It converts an Int32 data value to a string data value.

          \param d The input data value.

          \return A new data value converted to string data type. The caller will take the ownership of the returned data.

          \exception Exception It throws an exception if the input abstract data is not a Int32 type.
        */
        AbstractData* convert(const AbstractData* d) const throw(te::dt::Exception);

        /*!
          \brief It returns the data type code of input data: INT32_TYPE.

          \return The data type code of input data: INT32_TYPE.
        */
        int getInputDataTypeCode() const;

        /*!
          \brief It returns the data type code of output data: STRING_TYPE.

          \return The data type code of output data: STRING_TYPE.
        */
        int getOutputDataTypeCode() const;
    };

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_INT32TOSTRINGCONVERTER_H

