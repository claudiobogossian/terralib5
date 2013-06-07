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
  \file terralib/dataaccess/core/dataset/Index.h

  \brief It describes an index associated to a dataset schema.
*/

#ifndef __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_INDEX_H
#define __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_INDEX_H

// TerraLib
#include "../Config.h"
#include "../Enums.h"

// STL
#include <vector>
#include <string>

namespace te
{
  namespace da
  {
    namespace core
    {
  // Forward declarations
      class DataSetType;

      /*!
        \class Index

        \brief It describes an index associated to a dataset schema.

        \sa DataSetType
      */
      class TEDATAACCESSEXPORT Index
      {
        public:

          /*!
            \brief Constructor.

            \param name The index name.
            \param t    The index type.

            \post If dt is provided, the index will belong to the given DataSetType.
          */
          Index(const std::string& name,
                IndexType t = B_TREE_TYPE);

          /*!
            \brief Constructor.

            \param name       The index name.
            \param t          The index type.
            \param properties The properties that form the index.

            \pre The properties must belong to the same DataSetType as the index.

            \post If dt is provided, the index will belong to the given DataSetType.
          */
          Index(const std::string& name,
                IndexType t,
                const std::vector<std::size_t>& properties);

          /*!
            \brief Copy constructor.

            The new object will not have an associated DataSetType.

            \param rhs Right-hand-side instance.
          */
          Index(const Index& rhs);

          /*! \brief Destructor. */
          ~Index() { }

          /*!
            \brief Assignment operator.

            The new object will not have an assigned DataSetType.

            \param rhs Right-hand-side instance.

            \return A reference to this.
          */
          Index& operator=(const Index& rhs);

          /*!
            \brief It returns the index name.

            \return The index name.
          */
          const std::string& getName() const { return m_name; }

          /*!
            \brief It sets the index name.

            \param name The index name.
          */
          void setName(const std::string& name) { m_name = name; }

          /*!
            \brief It gets the index type.

            \return The index type.
          */
          IndexType getIndexType() const { return m_type; }

          /*!
            \brief It sets the index type.

            \param t The index type.
          */
          void setIndexType(IndexType t) { m_type = t; }

          /*!
            \brief It returns the properties that take part of the index.

            \return The properties that take part of the index.
          */
          const std::vector<std::size_t>& getProperties() const { return m_properties; }

          /*!
            \brief It returns the properties that take part of the index.

            \return The properties that take part of the index.
          */
          std::vector<std::size_t>& getProperties() { return m_properties; }

          /*!
            \brief It adds the property to the list of properties of the index.

            \param propertyPos The property that will take part of the index.
          */
          void add(std::size_t propertyPos) { m_properties.push_back(propertyPos); }

          /*!
            \brief It returns the DataSetType associated to the index.

            \return The DataSetType associated to the index.
          */
          DataSetType* getDataSetType() const { return m_dt; }

          /*!
            \brief It sets the DataSetType associated to the index.

            \param dt The DataSetType to which this index belongs.

            \warning Take care when calling this method. If the index belongs to a DataSetType,
                     remember to detach it from the DataSetType before calling this method.
          */
          void setDataSetType(DataSetType* dt) { m_dt = dt; }

        private:

          std::string m_name;                     //!< The index name.
          std::vector<std::size_t> m_properties;  //!< The list of properties that form the index.
          DataSetType* m_dt;                      //!< The associated dataset.
          IndexType m_type;                       //!< The index type.
      };

    }  // end namespace core
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_INDEX_H
