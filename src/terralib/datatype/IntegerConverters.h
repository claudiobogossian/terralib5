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
  \file terralib/datatype/IntegerConverters.h

  \brief A set of function that convert an Integer type to other types.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_INTEGERCONVERTERS_H
#define __TERRALIB_DATATYPE_INTERNAL_INTEGERCONVERTERS_H

// TerraLib
#include "Exception.h"

namespace te
{
  namespace dt
  {
// Forward declarations
    class AbstractData;

    /** @name Int32 Methods
      *  Methods related to Int32 conversion.
      */
    //@{

    /*!
      \brief It converts an Int32 data value to a char data value.

      \param d The input data value.

      \return A new data value converted to char data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an Int32 type.
    */
    AbstractData* Int32ToCharConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Int32 data value to an unsigned char data value.

      \param d The input data value.

      \return A new data value converted to unsigned char data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an Int32 type.
    */
    AbstractData* Int32ToUCharConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Int32 data value to a Int16 data value.

      \param d The input data value.

      \return A new data value converted to Int16 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an Int32 type.
    */
    AbstractData* Int32ToInt16Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Int32 data value to an unsigned Int16 data value.

      \param d The input data value.

      \return A new data value converted to unsigned Int16 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an Int32 type.
    */
    AbstractData* Int32ToUInt16Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Int32 data value to an unsigned Int32 data value.

      \param d The input data value.

      \return A new data value converted to unsigned Int32 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an Int32 type.
    */
    AbstractData* Int32ToUInt32Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Int32 data value to a Int64 data value.

      \param d The input data value.

      \return A new data value converted to Int64 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an Int32 type.
    */
    AbstractData* Int32ToInt64Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Int32 data value to an unsigned Int64 data value.

      \param d The input data value.

      \return A new data value converted to unsigned Int64 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an Int32 type.
    */
    AbstractData* Int32ToUInt64Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Int32 data value to a boolean data value.

      \param d The input data value.

      \return A new data value converted to boolean data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an Int32 type.
    */
    AbstractData* Int32ToBooleanConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Int32 data value to a float value.

      \param d The input data value.

      \return A new data value converted to float data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an Int32 type.
    */
    AbstractData* Int32ToFloatConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Int32 data value to a double value.

      \param d The input data value.

      \return A new data value converted to double data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an Int32 type.
    */
    AbstractData* Int32ToDoubleConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Int32 data value to Numeric data value.

      \param d The input data value.

      \return A new data value converted to Numeric data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an Int32 type.
    */
    AbstractData* Int32ToNumericConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an Int32 data value to a string data value.

      \param d The input data value.

      \return A new data value converted to string data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an Int32 type.
    */
    AbstractData* Int32ToStringConverter(AbstractData* d) throw(Exception);

    /*!
      \brief Auxiliary method that asserts that the given data is an Int32 type.

      \param d The input data value.

      \exception Exception It throws an exception if the input abstract data is not an Int32 type.
    */
    void AssertInt32(AbstractData* d) throw(Exception);

    //@}

    /** @name UInt32 Methods
      *  Methods related to unsigned Int32 conversion.
      */
    //@{

    /*!
      \brief It converts an unsigned Int32 data value to a char data value.

      \param d The input data value.

      \return A new data value converted to char data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an unsigned Int32 type.
    */
    AbstractData* UInt32ToCharConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an unsigned Int32 data value to an unsigned char data value.

      \param d The input data value.

      \return A new data value converted to unsigned char data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an unsigned Int32 type.
    */
    AbstractData* UInt32ToUCharConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an unsigned Int32 data value to a Int16 data value.

      \param d The input data value.

      \return A new data value converted to Int16 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an unsigned Int32 type.
    */
    AbstractData* UInt32ToInt16Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an unsigned Int32 data value to an unsigned Int16 data value.

      \param d The input data value.

      \return A new data value converted to unsigned Int16 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an unsigned Int32 type.
    */
    AbstractData* UInt32ToUInt16Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an unsigned Int32 data value to an Int32 data value.

      \param d The input data value.

      \return A new data value converted to Int32 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an unsigned Int32 type.
    */
    AbstractData* UInt32ToInt32Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an unsigned Int32 data value to a Int64 data value.

      \param d The input data value.

      \return A new data value converted to Int64 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an unsigned Int32 type.
    */
    AbstractData* UInt32ToInt64Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an unsigned Int32 data value to an unsigned Int64 data value.

      \param d The input data value.

      \return A new data value converted to unsigned Int64 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an unsigned Int32 type.
    */
    AbstractData* UInt32ToUInt64Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an unsigned Int32 data value to a boolean data value.

      \param d The input data value.

      \return A new data value converted to boolean data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an unsigned Int32 type.
    */
    AbstractData* UInt32ToBooleanConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an unsigned Int32 data value to a float value.

      \param d The input data value.

      \return A new data value converted to float data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an unsigned Int32 type.
    */
    AbstractData* UInt32ToFloatConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an unsigned Int32 data value to a double value.

      \param d The input data value.

      \return A new data value converted to double data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an unsigned Int32 type.
    */
    AbstractData* UInt32ToDoubleConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an unsigned Int32 data value to Numeric data value.

      \param d The input data value.

      \return A new data value converted to Numeric data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an unsigned Int32 type.
    */
    AbstractData* UInt32ToNumericConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an unsigned Int32 data value to a string data value.

      \param d The input data value.

      \return A new data value converted to string data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not an unsigned Int32 type.
    */
    AbstractData* UInt32ToStringConverter(AbstractData* d) throw(Exception);

    /*!
      \brief Auxiliary method that asserts that the given data is an unsigned Int32 type.

      \param d The input data value.

      \exception Exception It throws an exception if the input abstract data is not an unsigned Int32 type.
    */
    void AssertUInt32(AbstractData* d) throw(Exception);

    //@}

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_INTEGERCONVERTERS_H
