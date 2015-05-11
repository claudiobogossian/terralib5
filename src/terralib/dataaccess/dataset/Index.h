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
  \file terralib/dataaccess/dataset/Index.h

  \brief It describes an index associated to a DataSetType.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_INDEX_H
#define __TERRALIB_DATAACCESS_INTERNAL_INDEX_H

// TerraLib
#include "../Config.h"
#include "../Enums.h"

// STL
#include <vector>
#include <string>

namespace te
{
// Forward declarations
  namespace dt { class Property; }

  namespace da
  {
// Forward declarations
    class DataSetType;   

    /*!
      \class Index

      \brief It describes an index associated to a DataSetType.

      \sa DataSetType, PrimaryKey, ForeignKey, CheckConstraint, UniqueKey
    */
    class TEDATAACCESSEXPORT Index
    {
      public:

        /*!
          \brief Constructor.

          \param dt The DataSetType associated to this index.
          \param id The index identifier.

          \post If dt is provided, the index will belong to the given DataSetType.
          \post By default the index type is BTreeType.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        Index(DataSetType* parent = 0, unsigned int id = 0);

        /*!
          \brief Constructor.

          \param name The index name.
          \param t    The index type.
          \param dt   The DataSetType associated to this index.
          \param id   The index identifier.

          \post If dt is provided, the index will belong to the given DataSetType.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        Index(const std::string& name,
              IndexType t = B_TREE_TYPE,
              DataSetType* dt = 0,
              unsigned int id = 0);

        /*!
          \brief Constructor.

          \param name       The index name.
          \param t          The index type.
          \param properties The properties that form the index.
          \param dt         The DataSetType associated to this index.
          \param id         The index identifier.

          \pre The properties must belong to the same DataSetType as the index.

          \post If dt is provided, the index will belong to the given DataSetType.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        Index(const std::string& name,
              IndexType t,
              const std::vector<te::dt::Property*>& properties,
              DataSetType* dt = 0,
              unsigned int id = 0);

        /*!
          \brief Copy constructor.

          The new object will not have an associated DataSetType.

          \param rhs Right-hand-side instance.
        */
        Index(const Index& rhs);

        /*! \brief Destructor. */
        ~Index() {}

        /*!
          \brief Assignment operator.

          The new object will not have an assigned DataSetType.

          \param rhs Right-hand-side instance.

          \return A reference to this.
        */
        Index& operator=(const Index& rhs);

        /*!
          \brief It returns the index identifier.

          \return A number that identifies the index.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        unsigned int getId() const { return m_id; }

        /*!
          \brief It sets the DataSetType identifier.

          \param id A unique number that identifies the DataSetType in its DataSource.

          \warning The identifier value (id) may be used by data source implementations. So, don't rely on its value!
        */
        void setId(unsigned int id) { m_id = id; }

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
        const std::vector<te::dt::Property*>& getProperties() const { return m_properties; }

        /*!
          \brief It sets the properties that take part of the index.

          \param properties The properties that take part of the index.
        */
        void setProperties(const std::vector<te::dt::Property*>& properties) { m_properties = properties; }

        /*!
          \brief It adds the property to the list of properties of the index.

          \param p The property that will take part of the index.
        */
        void add(te::dt::Property* p) { m_properties.push_back(p); }

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

        /*!
          \brief It verifies if Property is associated to the index.

          \param p The Property to be verified.

          \return True if Property is associated to the index, false otherwise.
        */
        bool has(te::dt::Property* p);

        /*!
          \brief It changes the reference to property p to pp.

          \param p  A property that takes part of the index.
          \param pp The property that will take p place.

          \note If the property p is not in the idx attribute list this method does nothing.
        */
        void replace(te::dt::Property* p, te::dt::Property* pp);

        /*!
          \brief It returns a clone of the object.

          The new object will not have an associated schema.

          \return A clone of the object.
        */
        Index* clone();

  private:

        unsigned int m_id;                            //!< An identification number for the index.
        IndexType m_type;                             //!< The index type.
        DataSetType* m_dt;                            //!< The parent DataSetType.
        std::string m_name;                           //!< The index name.
        std::vector<te::dt::Property*> m_properties;  //!< The list of properties that form the index.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_INDEX_H

