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
  \file terralib/datatype/DataType.h

  \brief It stores information about a data type.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_DATATYPE_H
#define __TERRALIB_DATATYPE_INTERNAL_DATATYPE_H

// TerraLib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace dt
  {
    /*!
      \class DataType

      \brief It stores information about a data type.

      TerraLib keeps information about all available data types
      in the system in a singleton called DataTypeManager.
      In this singleton you will find all supported data types.
      
      Some of the data type codes are reserved for the built-in data
      types of TerraLib (they can be seen in defines).

      \ingroup datatype

      \sa DataTypeManager, AbstractData
    */
    class TEDATATYPEEXPORT DataType
    {
      public:

        /*! \brief Default constructor. */
        DataType() {}

        /*!
          \brief Constructor.

          \param name        The data type name.
          \param description Data type description.
        */
        DataType(const std::string& name, const std::string& description);

        /*! \brief Destructor. */
        ~DataType() {}

        /*!
          \brief It returns the data type id.

          \return The data type id.
        */
        int getId() const;

        /*!
          \brief It returns the data type name.

          \return The data type name.
        */
        const std::string& getName() const;

        /*!
          \brief It returns the data type description.

          \return The data type description.
        */
        const std::string& getDescription() const;

        /*!
          \brief It compares two data types by their ids.

          \return True, if the id of this data type is less than the data type in the right-hand-side.
        */
        bool operator<(const DataType& rhs) const;

      private:

        /*!
          \brief Constructor.

          \param id           The data type id assigned by TerraLib.
          \param name        The data type name.
          \param description Data type description.
        */
        DataType(int id, const std::string& name, const std::string& description);

        /*!
          \brief This method can be called only by DataTypeManager.

          \param id The type code.
        */
        void setId(int id);

      private:
        
        int m_id;                   //!< Data type id assigned by TerraLib.
        std::string m_name;         //!< Data type name.
        std::string m_description;  //!< Data type description.

      friend class DataTypeManager; 
    };

    inline DataType::DataType(const std::string& name, const std::string& description)
      : m_id(0), m_name(name), m_description(description)
    {
    }

    inline int DataType::getId() const
    {
      return m_id;
    }

    inline const std::string& DataType::getName() const
    {
      return m_name;
    }

    inline bool DataType::operator<(const DataType& rhs) const
    {
      return m_id < rhs.m_id;
    }

    inline DataType::DataType(int id, const std::string& name, const std::string& description)
      : m_id(id), m_name(name), m_description(description)
    {
    }
    
    inline void DataType::setId(int id)
    {
      m_id = id;
    }

  } // end namespace dt
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_DATATYPE_H

