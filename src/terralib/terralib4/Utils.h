/*  Copyright (C) 2009-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/terralib4/Utils.h

  \brief Utilitary functions for dealing with TerraLib 5 and 4.x conversion.
*/

#ifndef __TERRALIB_T4_INTERNAL_UTILS_H
#define __TERRALIB_T4_INTERNAL_UTILS_H

// STL
#include <map>
#include <memory>
#include <string>

// Terralib 5
#include "../datatype/Property.h"

// TerraLib 4.x
#include <terralib/kernel/TeAttribute.h>

namespace te
{
  namespace dt{ class Property; }
  namespace gm{ class Envelope; }
}

class TeBox;
class TeDatabaseFactoryParams;

namespace terralib4
{
  /*!
    \brief It creates a valid TerraLib 5 property given a valid TerraLib 4.x attribute representation.

    \param attRep A TerraLib 3 data type.

    \return The TerraLib 5 equivalent property.

    \exception Exception It throws an exception if a type cannot be conveted to TerraLib 5 datatype system.

    \note The returned PropertyType will not have an identification number, it is the caller
          responsability to set one.
  */
  std::auto_ptr<te::dt::Property> Convert2T5(const TeAttributeRep& attRep);

  /*!
    \brief It converts a Terralib 4.x data type to Terralib 5 data type.

    \param type The type to convert.

    \return A TerraLib 5 data type.
  */
  int Convert2T5(TeAttrDataType type);

  std::auto_ptr<te::gm::Envelope> Convert2T5(TeBox box);

  /*!
    \brief It converts a data source information to a TerraLib 4.x database params.

    \param dsInfo The data source information to be converted to a TerraLib 4.x database params.

    \return A TerraLib 4.x database params.
  */
  std::auto_ptr<TeDatabaseFactoryParams> Convert2T4DatabaseParams(const std::map<std::string, std::string>& dsInfo);

} // end namespace terralib4

#endif  // __TERRALIB_T4_INTERNAL_UTILS_H

