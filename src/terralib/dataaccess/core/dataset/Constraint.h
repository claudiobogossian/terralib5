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
  \file terralib/dataaccess/core/dataset/Constraint.h

  \brief A base class for constraints.
*/

#ifndef __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_CONSTRAINT_H
#define __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_CONSTRAINT_H

// TerraLib
#include "../Config.h"
#include "../Enums.h"

// STL
#include <string>

namespace te
{
  namespace da
  {
    namespace core
    {
// Forward declaration
      class DataSetType;

      /*!
        \class Constraint

        \brief A base class for constraints.

        \sa PrimaryKey, ForeignKey, UniqueKey, CheckConstraint
      */
      class TEDATAACCESSEXPORT Constraint
      {
        public:

          /*!
            \brief Constructor.

            \param name The constraint name.
          */
          Constraint(const std::string& name = "");

          /*!
            \brief Copy constructor not allowed.

            \param rhs Right-hand-side instance.
          */
          Constraint(const Constraint& rhs);

          /*! \brief Virtual destructor. */
          virtual ~Constraint() {}

          /*!
            \brief Assignment operator not allowed.

            \param rhs Right-hand-side instance.

            \return A reference to this object.
          */
          Constraint& operator=(const Constraint& rhs);

          /*!
            \brief It returns the dataset schema associated to the constraint.

            \return The dataset schema associated to the constraint.
          */
          DataSetType* getDataSetType() const { return m_dt; }

          /*!
            \brief It sets the DataSetType associated to the constraint.

            \param dt The dataset schema associated to this constraint.
          */
          void setDataSetType(DataSetType* dt) { m_dt = dt; }

          /*!
            \brief It returns the constraint name.

            \return The constraint name.
          */
          virtual const std::string& getName() const { return m_name; }

          /*!
            \brief It sets the constraint name.

            \param name The constraint name.
          */
          virtual void setName(const std::string& name) { m_name = name; }

          /*!
            \brief It returns the constraint type.
          
            \return The constraint type.

            \note Each child has to implement this method and return its specialized type.
          */
          virtual ConstraintType getType() const = 0;

          /*!
            \brief It returns a clone of the object.

            The new object will not have an associated DataSetType.

            \return A clone of the object.
          */
          virtual Constraint* clone() = 0;

        private:

          std::string m_name;   //!< The constraint name.
          DataSetType* m_dt;    //!< The associated dataset schema.
      };

    }  //end namespace core
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_CONSTRAINT_H
