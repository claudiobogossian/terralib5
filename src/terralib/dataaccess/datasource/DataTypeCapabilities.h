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
  \file terralib/dataaccess/datasource/DataTypeCapabilities.h

  \brief A class that represents the supported data types of a specific data source.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATATYPECAPABILITIES_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATATYPECAPABILITIES_H

// TerraLib
#include "../../datatype/Enums.h"
#include "../Config.h"

// STL
#include <map>

namespace te
{
  namespace da
  {
    /*!
      \class DataTypeCapabilities

      \brief A class that represents the supported data types of a specific data source.
    */
    class TEDATAACCESSEXPORT DataTypeCapabilities
    {
      public:

        /*! \brief Constructor. */
        DataTypeCapabilities();

        /*! \brief Destructor. */
        ~DataTypeCapabilities();

      public:

        bool supportsBit() const;

        void setSupportBit(const bool& support);

        bool supportsChar() const;

        void setSupportChar(const bool& support);

        bool supportsUChar() const;

        void setSupportUChar(const bool& support);

        bool supportsInt16() const;

        void setSupportInt16(const bool& support);

        bool supportsUInt16() const;

        void setSupportUInt16(const bool& support);

        bool supportsInt32() const;

        void setSupportInt32(const bool& support);

        bool supportsUInt32() const;

        void setSupportUInt32(const bool& support);

        bool supportsInt64() const;

        void setSupportInt64(const bool& support);

        bool supportsUInt64() const;

        void setSupportUInt64(const bool& support);

        bool supportsBoolean() const;

        void setSupportBoolean(const bool& support);

        bool supportsFloat() const;

        void setSupportFloat(const bool& support);

        bool supportsDouble() const;

        void setSupportDouble(const bool& support);

        bool supportsNumeric() const;

        void setSupportNumeric(const bool& support);

        bool supportsString() const;

        void setSupportString(const bool& support);

        bool supportsByteArray() const;

        void setSupportByteArray(const bool& support);

        bool supportsGeometry() const;

        void setSupportGeometry(const bool& support);

        bool supportsDateTime() const;

        void setSupportDateTime(const bool& support);

        bool supportsArray() const;

        void setSupportArray(const bool& support);

        bool supportsComposite() const;

        void setSupportComposite(const bool& support);

        bool supportsRaster() const;

        void setSupportRaster(const bool& support);

        bool supportsDataset() const;

        void setSupportDataset(const bool& support);

        bool supportsXML() const;

        void setSupportXML(const bool& support);

        bool supportsCInt16() const;

        void setSupportCInt16(const bool& support);

        bool supportsCInt32() const;

        void setSupportCInt32(const bool& support);

        bool supportsCFloat() const;

        void setSupportCFloat(const bool& support);

        bool supportsCDouble() const;

        void setSupportCDouble(const bool& support);

        bool supportsPolymorphic() const;

        void setSupportPolymorphic(const bool& support);

        bool supports(const int& type) const;

        void setSupport(const int& type, const bool& support);

        void setSupportAll();

        void addHint(const int& type, const int& hint);

        int getHint(const int& type) const;

      private:

        std::map<int, bool> m_types;  //!< A map that indicates the supported data types.
        std::map<int, int> m_hints;   //!< A map that indicates a type hint to a given type.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATATYPECAPABILITIES_H
