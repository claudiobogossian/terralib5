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
  \file terralib/dataaccess/core/dataset/CheckConstraint.h

  \brief A class that describes a check constraint.
*/

#ifndef __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_CHECKCONSTRAINT_H
#define __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_CHECKCONSTRAINT_H

// TerraLib
#include "AbstractConstraint.h"

namespace te
{
  namespace da
  {
    namespace core
    {
      /*!
        \class CheckConstraint

        \brief A class that describes a check constraint.

        \sa Constraint, PrimaryKey, UniqueKey, ForeignKey
      */
      class TEDATAACCESSEXPORT CheckConstraint : public AbstractConstraint
      {
        public:

          /*!
            \brief Constructor.

            \param name       The check constraint name.
            \param expression The check constraint expression.
          */
          CheckConstraint(const std::string& name = "",
                          const std::string& expression =  "");

          /*!
            \brief Copy constructor not allowed.

            \param rhs Right-hand-side instance.
          */
          CheckConstraint(const CheckConstraint& rhs);

          /*! \brief Destructor. */
          ~CheckConstraint() {}

          /*!
            \brief Assignment operator.

            \param rhs Right-hand-side instance.

            \return A reference to this.
          */
          CheckConstraint& operator=(const CheckConstraint& rhs);

          /*!
            \brief It returns the check constraint expression.

            \return The check constraint expression.
          */
          const std::string& getExpression() const { return m_expression; }

          /*!
            \brief It sets the check constraint expression.

            \param e The check constraint expression.
          */
          void setExpression(const std::string& e) { m_expression = e; } 

          /*!
            \brief It returns the constraint type: CHECK.
          
            \return The constraint type CHECK.
          */
          ConstraintType getType() const { return CHECK; }

          /*!
            \brief It returns a clone of the object.

            The new object will not have an associated DataSetType.

            \return A clone of the object.
          */
          AbstractConstraint* clone();
      
        private:

          std::string m_expression; //!< The check constraint expression.
      };

    }  //end namespace core
  }    // end namespace da
}      // end namespace te

#endif  // __TERRALIB_DATAACCESS_CORE_DATASET_INTERNAL_CHECKCONSTRAINT_H
