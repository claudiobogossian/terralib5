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
  \file terralib/datatype/DataTypeManager.h

  \brief A singleton for managing all data types in the system.
*/

#ifndef __TERRALIB_DATATYPE_INTERNAL_DATATYPEMANAGER_H
#define __TERRALIB_DATATYPE_INTERNAL_DATATYPEMANAGER_H

// TerraLib
#include "../common/Singleton.h"
#include "DataType.h"
#include "Exception.h"

// STL
#include <set>

namespace te
{
  namespace dt
  {
    /*!
      \class DataType

      \brief A singleton for managing all data types in the system.

      TerraLib keeps information about all available data types
      in this singleton. Here, you can find all supported data types.

      Basic constraints for data types:
      <ul>
      <li>No two data types may have the same name</li>
      <li>The id of a data type will be dynamically generated by the manager</li>
      <li>Data type names must be in capital letters although it can contains numbers and other symbols</li>
      </ul>

      \sa DataType

      \todo Remove the static data member and use the type code!
     */
    class TEDATATYPEEXPORT DataTypeManager : public te::common::Singleton<DataTypeManager>
    {
      friend class te::common::Singleton<DataTypeManager>;

      public:

        /*!
          \brief It adds a new data type to the system.

          \param dt The data type to be added.

          \return The id associated to the new data type on success (a number greater than 0).

          \exception Exception It throws an exception if a data type with the same name already exists in the system.

          \note Not thread-safe.
        */
        int add(const DataType& dt) throw(te::dt::Exception);

        /*!
          \brief It adds a new data type to the system.

          \param name        The new data type name.
          \param description The data type description.

          \return The id associated to the new data type on success (a number greater than 0).

          \exception Exception It throws an exception if a data type with the same name already exists in the system.

          \note Not thread-safe.
        */
        int add(const std::string& name, const std::string& description) throw(te::dt::Exception);

        /*!
          \brief It removes the data type.

          \param dt The data type.
         */
        void remove(const DataType* dt);

        /*!
          \brief It finds a data type having the given id.

          \param id The data type id.

          \return The data type having the id or NULL if none is found.
        */
        const DataType* find(int id) const;

        /*!
          \brief It finds a data type having the given name.

          \param name The data type name.

          \return The data type having the name or NULL if none is found.
        */
        const DataType* find(const std::string& name) const;

      protected:

        /*! \brief Constructor for singletons is protected. */
        DataTypeManager();

        /*! \brief Destructor for singletons is protected. */
        ~DataTypeManager();

      private:

        /*!
          \struct NameComparer

          \brief A functor for comparing a pair of pointers to data types by name.
        */
        struct NameComparer
        {
          bool operator()(const DataType* f, const DataType* s) const
          {
            return f->getName() < s->getName();
          }
        };

        /*!
          \struct IdComparer

          \brief A functor for comparing a pair of pointers to data types by id.
        */
        struct IdComparer
        {
          bool operator()(const DataType* f, const DataType* s) const
          {
            return f->getId() < s->getId();
          }
        };
        
        std::set<DataType*, IdComparer> m_types;      //!< The set of data types ordered by id.
        std::set<DataType*, NameComparer> m_nameIdx;  //!< The set of data types ordered by name.

        static int sm_lastId;                 //!< The id value of the last inserted data type. 
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATATYPE_INTERNAL_DATATYPEMANAGER_H

