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
  \file terralib/datatype/StringConverters.h

  \brief A set of function that convert an String type to other types.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_STRINGCONVERTERS_H
#define __TERRALIB_DATATYPE_INTERNAL_STRINGCONVERTERS_H

// TerraLib
#include "Exception.h"

namespace te
{
  namespace dt
  {
// Forward declarations
    class AbstractData;

    /** @name String Methods
      *  Methods related to String conversion.
      */
    //@{

    /*!
      \brief It converts an String data value to a Int16 data value.

      \param d The input data value.

      \return A new data value converted to Int16 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a String type.
    */
    AbstractData* StringToInt16Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an String data value to an unsigned Int16 data value.

      \param d The input data value.

      \return A new data value converted to unsigned Int16 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a String type.
    */
    AbstractData* StringToUInt16Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an String data value to a Int32 data value.

      \param d The input data value.

      \return A new data value converted to Int32 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a String type.
    */
    AbstractData* StringToInt32Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an String data value to an unsigned Int32 data value.

      \param d The input data value.

      \return A new data value converted to unsigned Int32 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a String type.
    */
    AbstractData* StringToUInt32Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an String data value to a Int64 data value.

      \param d The input data value.

      \return A new data value converted to Int64 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a String type.
    */
    AbstractData* StringToInt64Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an String data value to an unsigned Int64 data value.

      \param d The input data value.

      \return A new data value converted to unsigned Int64 data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a String type.
    */
    AbstractData* StringToUInt64Converter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an String data value to a float data value.

      \param d The input data value.

      \return A new data value converted to float data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a String type.
    */
    AbstractData* StringToFloatConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an String data value to a double data value.

      \param d The input data value.

      \return A new data value converted to double data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a String type.
    */
    AbstractData* StringToDoubleConverter(AbstractData* d) throw(Exception);

    /*!
      \brief It converts an String data value to a Numeric data value.

      \param d The input data value.

      \return A new data value converted to Numeric data type. The caller will take the ownership of the returned data.

      \exception Exception It throws an exception if the input abstract data is not a String type.
    */
    AbstractData* StringToNumericConverter(AbstractData* d) throw(Exception);

    /*!
      \brief Auxiliary method that asserts that the given data is an String type.

      \param d The input data value.

      \exception Exception It throws an exception if the input abstract data is not a String type.
    */
    void AssertString(AbstractData* d) throw(Exception);

    //@}

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_STRINGCONVERTERS_H
