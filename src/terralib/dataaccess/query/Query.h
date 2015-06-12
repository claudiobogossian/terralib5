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
  \file terralib/dataaccess/query/Query.h

  \brief The base class for queries.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_QUERY_H
#define __TERRALIB_DATAACCESS_INTERNAL_QUERY_H

// TerraLib
#include "../../common/BaseVisitable.h"
#include "QueryVisitor.h"

namespace te
{
  namespace da
  {
    /*!
      \class Query
      
      \brief A Query is independent from the data source language/dialect.

      \ingroup dataaccess

      \sa Select, Insert, Update, Delete, CreateDataSet, DropDataSet, AlterDataSet, CreateIndex, DropIndex
    */
    class TEDATAACCESSEXPORT Query : public te::common::BaseVisitable<QueryVisitor>
    {
      public:

        TE_DEFINE_VISITABLE

        /*! \brief Default constructor. */
        Query() {}

        /*! \brief Virtual destructor. */
        virtual ~Query() {}

        /*! \brief It creates a new copy of this query. */
        virtual Query* clone() const = 0;
    };

    /*! \brief For use with boost conteiners. */
    inline Query* new_clone(const Query& a)
    {
      return a.clone();
    }

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_QUERY_H

