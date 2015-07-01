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
#include "../common/Enums.h"
#include "../datatype/Enums.h"
#include "../datatype/Property.h"
#include "../geometry/Enums.h"
#include "Config.h"

// TerraLib 4.x
#include <terralib4/kernel/TeAttribute.h>
#include <terralib4/kernel/TeDatabase.h>

namespace te
{
  namespace da { class DataSetType; }
  namespace dt { class Property; }
  namespace gm { class Envelope; }
  namespace rst { class RasterProperty; }
}

struct TeBox;
class TeDatabaseFactoryParams;
class TeRasterParams;
class TeTable;

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

  te::gm::GeomType Convert2T5GeomType(TeAttrDataType type);

  /*!
    \brief It converts a Terralib 4.x data type to Terralib 5 data type.

    \param type The type to convert.

    \return A TerraLib 5 data type.
  */
  TETERRALIB4EXPORT int Convert2T5(TeAttrDataType type);

  /*!
    \brief It converts a Terralib 4.x data type to Terralib 5 data type.

    \param type The type to convert.

    \return A TerraLib 5 data type.
  */
  TETERRALIB4EXPORT te::gm::GeomType Convert2T5GeomType(TeGeomRep type);

  /*!
    \brief It converts a Terralib 5 data type to Terralib 4.x data type.

    \param type The type to convert.

    \return A TerraLib 4.x data type.
  */
  TeAttrDataType Convert2T4(int type);

  TeAttrDataType Convert2T4GeomType(te::gm::GeomType type);

  int Convert2T5(TeDataType dt);

  std::auto_ptr<te::gm::Envelope> Convert2T5(TeBox box);

  std::auto_ptr<te::da::DataSetType> Convert2T5(TeTable table);

  /*!
    \brief It converts a data source information to a TerraLib 4.x database params.

    \param dsInfo The data source information to be converted to a TerraLib 4.x database params.

    \return A TerraLib 4.x database params.
  */
  std::auto_ptr<TeDatabaseFactoryParams> Convert2T4DatabaseParams(const std::map<std::string, std::string>& dsInfo);

  te::rst::RasterProperty* Convert2T5(TeRasterParams& rparams);

  void CheckDecimalSeparator(std::string& value);

} // end namespace terralib4

#endif  // __TERRALIB_T4_INTERNAL_UTILS_H

