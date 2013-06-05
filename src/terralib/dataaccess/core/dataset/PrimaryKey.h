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
  \file terralib/dataaccess/core/dataset/PrimaryKey.h

  \brief It describes a primary key (pk) constraint.
*/

#ifndef __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_PRIMARYKEY_H
#define __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_PRIMARYKEY_H

// TerraLib
#include "UniqueKey.h"

namespace te
{
  namespace da
  {
    namespace core
    {
      /*!
        \class PrimaryKey

        \brief It describes a primary key (pk) constraint.

        \sa Constraint, UniqueKey, ForeignKey, CheckConstraint
      */
      class TEDATAACCESSEXPORT PrimaryKey : public UniqueKey
      {
        public:

          /*!
            \brief Constructor.

            \param name The primary key constraint name.
          */
          PrimaryKey(const std::string& name = "");

          /*!
            \brief Copy constructor.

            \param rhs Right-hand-side instance.
          */
          PrimaryKey(const PrimaryKey& rhs);

          /*! \brief Destructor. */
          ~PrimaryKey() {}

          /*!
            \brief Assignment operator.

            \param rhs Right-hand-side instance.

            \return A reference to this.
          */
          PrimaryKey& operator=(const PrimaryKey& rhs);

          /*!
            \brief It returns the constraint type: PRIMARY_KEY.

            \return The constraint type PRIMARYKEY.
          */
          ConstraintType getType() const { return PRIMARY_KEY; }

          /*!
            \brief It returns a clone of the object.

            \return A clone of the object.
          */
          Constraint* clone();
      };

    }  // end namespace core
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_PRIMARYKEY_H

