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
  \file terralib/datatype/BooleanConverters.h

  \brief A set of function that convert an Boolean type to other types.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_BOOLEANCONVERTERS_H
#define __TERRALIB_DATATYPE_INTERNAL_BOOLEANCONVERTERS_H

// TerraLib
#include "Exception.h"

namespace te
{
  namespace dt
  {
// Forward declarations
    class AbstractData;

    /** @name Boolean Methods
      *  Methods related to Boolean conversion.
      */
    //@{

    /*!
      \brief It converts an Boolean data value to a char data value.

      \param d The input data value.

      \return A new data value converted to char data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a Boolean type.
    */
    AbstractData* BooleanToCharConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Boolean data value to an unsigned char data value.

      \param d The input data value.

      \return A new data value converted to unsigned char data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a Boolean type.
    */
    AbstractData* BooleanToUCharConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Boolean data value to a Int16 data value.

      \param d The input data value.

      \return A new data value converted to Int16 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a Boolean type.
    */
    AbstractData* BooleanToInt16Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Boolean data value to an unsigned Int16 data value.

      \param d The input data value.

      \return A new data value converted to unsigned Int16 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a Boolean type.
    */
    AbstractData* BooleanToUInt16Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Boolean data value to a Int32 data value.

      \param d The input data value.

      \return A new data value converted to Int32 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a Boolean type.
    */
    AbstractData* BooleanToInt32Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Boolean data value to an unsigned Int32 data value.

      \param d The input data value.

      \return A new data value converted to unsigned Int32 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a Boolean type.
    */
    AbstractData* BooleanToUInt32Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Boolean data value to a Int64 data value.

      \param d The input data value.

      \return A new data value converted to Int64 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a Boolean type.
    */
    AbstractData* BooleanToInt64Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Boolean data value to an unsigned Int64 data value.

      \param d The input data value.

      \return A new data value converted to unsigned Int64 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a Boolean type.
    */
    AbstractData* BooleanToUInt64Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Boolean data value to a float data value.

      \param d The input data value.

      \return A new data value converted to float data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a Boolean type.
    */
    AbstractData* BooleanToFloatConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Boolean data value to a double data value.

      \param d The input data value.

      \return A new data value converted to double data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a Boolean type.
    */
    AbstractData* BooleanToDoubleConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Boolean data value to a Numeric data value.

      \param d The input data value.

      \return A new data value converted to Numeric data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a Boolean type.
    */
    AbstractData* BooleanToNumericConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Boolean data value to a string data value.

      \param d The input data value.

      \return A new data value converted to string data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a Boolean type.
    */
    AbstractData* BooleanToStringConverter(AbstractData* d) throw(Exception);

    /*!
      \brief Auxiliary method that asserts that the given data is an Boolean type.

      \param d The input data value.

      \exception Exception It throws an exception if the input abstract data is not a Boolean type.
    */
    void AssertBoolean(AbstractData* d) throw(Exception);

    //@}

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_BOOLEANCONVERTERS_H
