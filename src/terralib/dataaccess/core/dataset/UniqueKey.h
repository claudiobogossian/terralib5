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
  \file terralib/dataaccess/core/dataset/UniqueKey.h

  \brief It describes an unique key (uk) constraint.
*/

#ifndef __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_UNIQUEKEY_H
#define __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_UNIQUEKEY_H

// TerraLib
#include "Constraint.h"

// STL
#include <vector>

namespace te
{
  namespace da
  {
    namespace core
    {
      /*!
        \class UniqueKey
      
        \brief It describes a unique key (pk) constraint.

        \sa Constraint, PrimaryKey, ForeignKey, CheckConstraint
      */
      class TEDATAACCESSEXPORT UniqueKey : public Constraint
      {
        public:

          /*!
            \brief Constructor.

            \param name The unique key constraint name.
          */
          UniqueKey(const std::string& name = "");

          /*!
            \brief Copy constructor.

            \param rhs Right-hand-side instance.
          */
          UniqueKey(const UniqueKey& rhs);

          /*! \brief Destructor. */
          virtual ~UniqueKey() {}

          /*!
            \brief Assignment operator.

            \param rhs Right-hand-side instance.

            \return A reference to this.
          */
          UniqueKey& operator=(const UniqueKey& rhs);

          /*!
            \brief It returns the properties that take part of the unique key.

            \return The properties that take part of the unique key.
          */
          const std::vector<std::size_t>& getProperties() const { return m_properties; }

          /*!
            \brief It returns the properties that take part of the unique key.

            \return The properties that take part of the unique key.
          */
          std::vector<std::size_t>& getProperties() { return m_properties; }

          /*!
            \brief It sets the properties that form the unique key.

            \param properties The properties that form the unique key.
          */
          void setProperties(const std::vector<std::size_t>& properties) { m_properties = properties; }

          /*!
            \brief It adds a property to the list of properties of the unique key.

            \param porpertyPos The property position (possible in a dataset) that will take part of the unique key.
          */
          void add(std::size_t porpertyPos) { m_properties.push_back(porpertyPos); }

          /*!
            \brief It returns the constraint type: PRIMARY_KEY.

            \return The constraint type PRIMARYKEY.
          */
          virtual ConstraintType getType() const { return PRIMARY_KEY; }

          /*!
            \brief It returns a clone of the object.

            \return A clone of the object.
          */
          virtual Constraint* clone();

        private:

          std::vector<std::size_t> m_properties;  //!< The properties that take part of unique key constraint.
      };

    }  // end namespace core
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_UNIQUEKEY_H

