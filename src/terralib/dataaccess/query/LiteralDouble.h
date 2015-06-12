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
  \file terralib/dataaccess/query/LiteralDouble.h

  \brief A class that models a literal for double values.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_LITERALDOUBLE_H
#define __TERRALIB_DATAACCESS_INTERNAL_LITERALDOUBLE_H

// TerraLib
#include "Literal.h"

namespace te
{
  namespace da
  {
    /*!
      \class LiteralDouble
      
      \brief A class that models a literal for double values.

      \sa Literal
    */
    class TEDATAACCESSEXPORT LiteralDouble : public Literal
    {
      public:

        TE_DEFINE_VISITABLE

        /*!
          \brief Constructor.

          \param v The double value. The literal will take its ownership
        */
        LiteralDouble(te::dt::AbstractData* v);

        /*!
          \brief Constructor.

          \param v The double value.
        */
        LiteralDouble(const te::dt::AbstractData& v);

        /*!
          \brief Constructor.

          \param v The double value.
        */
        LiteralDouble(const double& v);

        /*! \brief Copy constructor. */
        LiteralDouble(const LiteralDouble& rhs);

        /*! \brief Destructor. */
        ~LiteralDouble();

        /*! Assignment operator.  */
        LiteralDouble& operator=(const LiteralDouble& rhs);

        /*! \brief It creates a new copy of this expression. */
        Expression* clone() const;
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_LITERALDOUBLE_H

