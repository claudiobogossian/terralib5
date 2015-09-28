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
  \file terralib/dataaccess/query/SelectExpression.h

   \brief A Select can be used as a source of information in another query.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_SELECTEXPRESSION_H
#define __TERRALIB_DATAACCESS_INTERNAL_SELECTEXPRESSION_H

// TerraLib
#include "Expression.h"

// STL
#include <memory>

namespace te
{
  namespace da
  {
// Forward declarations
    class Select;

    /*!
      \class SelectExpression
      
      \brief A Select can be used as a source of information in another query.

      \sa FromItem, DataSetName, DataSetType, DataSet, FromFunctionCall, Join 
    */
    class TEDATAACCESSEXPORT SelectExpression : public Expression
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param s The real Select to be associated to this object.

          \note The SelectExpression will take the ownership of the Select.
        */
        SelectExpression(Select* s);

        /*!
          \brief Constructor.

          \param s The real Select to be associated to this object.
        */
        SelectExpression(const Select& s);

        /*! \brief Copy constructor. */
        SelectExpression(const SelectExpression& rhs);

        /*! \brief Destructor. */
        ~SelectExpression() {}

        /*! Assignment operator.  */
        SelectExpression& operator=(const SelectExpression& rhs);

        /*! \brief It creates a new copy of this Expression. */
        Expression* clone() const;

        /*!
          \brief It returns the associated select expression.

          \return The associated select expression.
        */
        Select* getSelect() const;

        /*!
          \brief It sets the real SelectExpression associated to this object.

          \param s The Select to be associated to the Expression object.

          \note The SelectExpression will take the ownership of the Select.
        */
        void setSelect(Select* s);

      private:

        std::auto_ptr<Select> m_select;  //!< The associated SelectExpression.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_SELECTEXPRESSION_H

