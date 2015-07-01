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
  \file terralib/dataaccess/dataset/AttributeConverterManager.h

  \brief A singleton to keep all the registered Attribute Converter.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ATTRIBUTECONVERTERMANAGER_H
#define __TERRALIB_DATAACCESS_INTERNAL_ATTRIBUTECONVERTERMANAGER_H

// TerraLib
#include "../../common/Singleton.h"
#include "AttributeConverters.h"

// STL
#include <map>
#include <string>
#include <vector>

namespace te
{
  namespace da
  {
      /*!
        \class AttributeConverterManager

        \brief A singleton to keep all the registered Attribute Converters.
      */
      class TEDATAACCESSEXPORT AttributeConverterManager : public te::common::Singleton<AttributeConverterManager>
      {
        public:

          void addConverter(const std::string& name, AttributeConverter converter);

          AttributeConverter getConverter(const std::string& name);

          void getConverters(std::vector<std::string>& names) const;

        private:

          std::map<std::string, AttributeConverter> m_converters;
      };

  }   // end namespace da
}     // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ATTRIBUTECONVERTERMANAGER_H
