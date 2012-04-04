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
  \file terralib/datatype/DataConverterManager.h

  \brief A singleton for managing the data type converter available in the system.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_DATACONVERTERMANAGER_H
#define __TERRALIB_DATATYPE_INTERNAL_DATACONVERTERMANAGER_H

// TerraLib
#include "../common/Singleton.h"
#include "Config.h"
#include "Exception.h"

// STL
#include <map>

namespace te
{
  namespace dt
  {
// Forward declaration
    class AbstractDataConverter;

    /*!
      \class DataConverterManager

      \brief A singleton for managing the data type converter available in the system.      

      \sa AbstractDataConverter

      \todo Make the internal representation a set instead of a map.
    */
    class TEDATATYPEEXPORT DataConverterManager : public te::common::Singleton<DataConverterManager>
    {
      friend class te::common::Singleton<DataConverterManager>;

      public:

        /*!
          \brief It adds the converter to the list of known data type mappings.

          \param conv A data type converter. The manager will take its ownership.

          \exception Exception It throws an exception if a converter for the same destination and target data types already exists.
        */
        void add(AbstractDataConverter* conv) throw(Exception);

        /*!
          \brief It returns a data type converter for the given map or NULL if none is found.

          \param typeMap The type of converter: from-data-type -> destination-data-type.

          \return A data type converter for the given map or NULL if none is found.
        */
        AbstractDataConverter* get(const std::pair<int, int>& typeMap);

        /*!
          \brief It returns a data type converter for the given map or NULL if none is found.

          \param src The source data type of the converter.
          \param dst The target data type of the conversion.

          \return A data type converter for the given map or NULL if none is found.
        */
        AbstractDataConverter* get(int src, int dst);

        /*!
          \brief It returns the list of available converters.

          \return The list of available converters.
        */
        const std::map<std::pair<int, int>, AbstractDataConverter*>& getConverters() const;

        /*!
          \brief It returns the list of available converters.

          \return The list of available converters.
        */
        std::map<std::pair<int, int>, AbstractDataConverter*>& getConverters();        

      protected:

        /*! \brief Constructor. */
        DataConverterManager();

        /*! \brief Destructor. */
        ~DataConverterManager();

      private:

        std::map<std::pair<int, int>, AbstractDataConverter*> m_convMap;  //!< A map: (src-data-type, destination-data-type) -> data converter
    };

    inline AbstractDataConverter* DataConverterManager::get(int src, int dst)
    {
      std::pair<int, int> mapping(src, dst);
      return get(mapping);
    }

    inline const std::map<std::pair<int, int>, AbstractDataConverter*>& DataConverterManager::getConverters() const
    {
      return m_convMap;
    }

    inline std::map<std::pair<int, int>, AbstractDataConverter*>& DataConverterManager::getConverters()
    {
      return m_convMap;
    }    

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_DATACONVERTERMANAGER_H

