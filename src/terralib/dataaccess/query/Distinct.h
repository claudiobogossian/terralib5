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
  \file terralib/dataaccess/query/Distinct.h

  \brief A class that models a Distinct clause on a query.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DISTINCT_H
#define __TERRALIB_DATAACCESS_INTERNAL_DISTINCT_H

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace da
  {
// Forward declaration
    class Expression;

    /*! \brief A class that models a Distinct clause on a query. */
    typedef boost::ptr_vector<Expression> Distinct;

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DISTINCT_H

