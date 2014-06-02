/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/datatype/DataConverterManager.h

  \brief A singleton for managing the data type converter available in the system.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_DATACONVERTERMANAGER_H
#define __TERRALIB_DATATYPE_INTERNAL_DATACONVERTERMANAGER_H

// TerraLib
#include "../common/Singleton.h"
#include "Config.h"
#include "DataTypeConverter.h"
#include "Exception.h"

// STL
#include <map>

namespace te
{
  namespace dt
  {
    /*!
      \class DataConverterManager

      \brief A singleton for managing the data type converter available in the system.

      \ingroup datatype

      \sa DataTypeConverter
    */
    class TEDATATYPEEXPORT DataConverterManager : public te::common::Singleton<DataConverterManager>
    {
      friend class te::common::Singleton<DataConverterManager>;

      public:

        /*!
          \brief It adds a converter to the list of known data type mappings.

          \param src The source data type of the converter.
          \param dst The destination data type of the converter.
          \param conv The data type converter.

          \exception Exception It throws an exception if a converter for the same source and destination data types already exists.
        */
        void add(int src, int dst, DataTypeConverter conv) throw(Exception);

         /*!
          \brief It returns a data type converter for the given source and destination data types.

          \param src The source data type of the converter.
          \param dst The destination data type of the converter.

          \return A data type converter for the given source and destination data types. or NULL if none is found.

          \exception Exception It throws an exception if none is found.
        */
        const DataTypeConverter& get(int src, int dst) throw(Exception);

        /*!
          \brief It returns a data type converter for the given source and destination data types

          \param typeMap The type of converter: source-data-type -> destination-data-type.

          \return A data type converter for the given map.

          \exception Exception It throws an exception if none is found.
        */
        const DataTypeConverter& get(const std::pair<int, int>& typeMap) throw(Exception);
  
        /*! \brief Clear the container. */
        void clear();

      protected:

        /*! \brief Constructor. */
        DataConverterManager();

        /*! \brief Destructor. */
        ~DataConverterManager();

      private:

        std::map<std::pair<int, int>, DataTypeConverter> m_convMap;  //!< A map: (source-data-type, target-data-type) -> data type converter
    };

    inline const DataTypeConverter& DataConverterManager::get(int src, int dst) throw(Exception)
    {
      std::pair<int, int> mapping(src, dst);
      return get(mapping);
    }

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_DATACONVERTERMANAGER_H
