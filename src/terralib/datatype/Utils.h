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
  \file terralib/datatype/Utils.h

  \brief Utilitary function for data type module.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_UTILS_H
#define __TERRALIB_DATATYPE_INTERNAL_UTILS_H

// TerraLib
#include "Config.h"

// STL
#include <vector>
#include <string>

namespace te
{
  namespace dt
  {
// Forward declaration
    class CompositeProperty;
    class Property;

    /*!
      \brief It creates a list with the properties position in the composite property.

      \param properties The properties to find their position in the composite.
      \param cp         The composite property that contains the properties.
      \param poslist    An output list with the properties position.

      \pre The composite property cp must contains all informed properties.

      \note The function will append the positions into the poslist. It doesn't clear previous data.
    */
    TEDATATYPEEXPORT void GetPropertiesPosition(const std::vector<Property*>& properties,
                                                const CompositeProperty* cp,
                                                std::vector<std::size_t>& poslist);

    /*!
      \brief It convert a string to a DataType.

      \param dataType String to be converted.
      
      \return DataType integer
    */
    TEDATATYPEEXPORT int Convert2Terralib(const std::string& dataType);

    /*!
      \brief Function used to convert from a int (Data Type Enum) to a string

      \param dataType     Integer value that defines the data type

      \return A string with the data type name.
    */
    TEDATATYPEEXPORT std::string ConvertDataTypeToString(const int& dataType);

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_UTILS_H

