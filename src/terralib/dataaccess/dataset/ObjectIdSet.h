/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/dataset/ObjectIdSet.h

  \brief This class represents a set of unique ids created in the same context. i.e. from the same data set.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_OBJECTIDSET_H
#define __TERRALIB_DATAACCESS_INTERNAL_OBJECTIDSET_H

// TerraLib
#include "../Config.h"
#include "ObjectId.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

// STL
#include <vector>

namespace te
{
  namespace da
  {
// Forward declaration
    class DataSet;
    class DataSetType;
    class Select;

    /*!
      \class ObjectIdSet

      \brief This class represents a set of unique ids created in the same context. i.e. from the same data set.

      \sa DataSet, DataSetType, ObjectId
    */
    class TEDATAACCESSEXPORT ObjectIdSet
    {
      public:

        /*!
          \brief Constructor.

          \param type The type of the data set that generates the unique ids.

          \note This class will NOT take the ownership of the given pointer.
        */
        ObjectIdSet(const DataSetType* type);

        /*! \brief Destructor. */
        ~ObjectIdSet();
      
        /*!
          \brief It returns the type of the data set that generated the unique ids.

          \return The type of the data set that generated the unique ids.
        */
        const DataSetType* getType() const;

        /*!
          \brief It adds a propety that will be used to generate the unique ids.

          \param name A propety name that will be used to generate the unique ids.
        */
        void addProperty(const std::string& name);

        /*!
          \brief It adds a propety that will be used to generate the unique ids.

          \param i A propety index that will be used to generate the unique ids.
        */
        void addProperty(std::size_t i);

        /*!
          \brief It sets the properties that will be used to generate the unique ids.

          \param names A vector with the propety names that will be used to generate the unique ids.
        */
        void setProperties(const std::vector<std::string>& names);

        /*!
          \brief It sets the properties that will be used to generate the unique ids.

          \param indexes A vector with the propety indexes that will be used to generate the unique ids.
        */
        void setProperties(const std::vector<std::size_t>& indexes);

        /*!
          \brief It adds an object id to this object id set.

          \param The object id that will be added.

          \note This class will take the ownership of the given pointer.
        */
        void add(ObjectId* oid);

        /*!
          \brief It returns the query that can be used to retrieve the data set that contains the all indentified elements.

          \return The query that can be used to retrieve the data set that contains the all indentified elements.

          \note The caller will take the ownership of the given pointer.
        */
        Select* getQuery() const;

        /*!
          \brief It returns the query that can be used to retrieve the data set that contains the i-th indentified element.

          \return The query that can be used to retrieve the data set that contains the i-th indentified element.

          \note The caller will take the ownership of the given pointer.
        */
        Select* getQuery(std::size_t i) const;

        /*! \brief It clears this object id set. */
        void clear();

        /*! \brief It returns true if the unique ids were generated from primary key properties. Otherwise, it returns false. */
        bool isFromPrimaryKey() const;

        /*! It sets if the unique id were generated from primary key properties. */
        void setIsFromPrimaryKey(bool value);

        /*! \brief It returns true if the unique id were generated from unique key properties. Otherwise, it returns false. */
        bool isFromUniqueKeys() const;

        /*! It sets if the unique id were generated from unique key properties. */
        void setIsFromUniqueKeys(bool value);

        /*! It returns the object id set size. */
        std::size_t size() const;

      private:

        const DataSetType* m_type;          //!< A pointer to the type of the data set that generates the unique ids.
        std::vector<std::size_t> m_indexes; //!< A vector with the property indexes used to generate de unique ids.
        boost::ptr_vector<ObjectId> m_oids; //!< The set of unique ids.
        bool m_isFromPk;                    //!< A flag that indicates if the unique ids was generated from primary key properties.
        bool m_isFromUk;                    //!< A flag that indicates if the unique ids was generated from unique key properties.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_OBJECTIDSET_H
