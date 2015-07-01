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
  \file terralib/dataaccess/query/SQLVisitor.h

  \brief A visitor for building an SQL statement from a given Query hierarchy.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_SQLVISITOR_H
#define __TERRALIB_DATAACCESS_INTERNAL_SQLVISITOR_H

// TerraLib
#include "Distinct.h"
#include "Fields.h"
#include "From.h"
#include "GroupBy.h"
#include "OrderBy.h"
#include "QueryVisitor.h"

// STL
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace da
  {
// Forward declarations
    class SQLDialect;

    /*!
      \class SQLVisitor
      
      \brief A visitor for building an SQL statement from a given Query hierarchy.

      Each driver must implement a SQL dialect in order help
      this class to translate the query object to its specific dialect.
    */
    class TEDATAACCESSEXPORT SQLVisitor : public QueryVisitor, public boost::noncopyable
    {
      public:

        //using QueryVisitor::visit;

        /*! \brief Default constructor. */
        SQLVisitor(const SQLDialect& dialect, std::string& sql) : m_dialect(dialect), m_sql(sql) {}

        /*! \brief Virtual destructor. */
        virtual ~SQLVisitor() {}

        virtual void visit(const Expression& visited);
        virtual void visit(const DataSetName& visited);
        virtual void visit(const FromItem& visited);
        virtual void visit(const Function& visited);
        virtual void visit(const Insert& visited);
        virtual void visit(const Join& visited);
        virtual void visit(const JoinCondition& visited);
        virtual void visit(const JoinConditionOn& visited);
        virtual void visit(const JoinConditionUsing& visited);
        virtual void visit(const Literal& visited);
        virtual void visit(const LiteralBool& visited);
        virtual void visit(const LiteralByteArray& visited);
        virtual void visit(const LiteralDateTime& visited);
        virtual void visit(const LiteralDouble& visited);
        virtual void visit(const LiteralEnvelope& visited);
        virtual void visit(const LiteralGeom& visited);
        virtual void visit(const LiteralInt16& visited);
        virtual void visit(const LiteralInt32& visited);
        virtual void visit(const LiteralInt64& visited);
        virtual void visit(const LiteralString& visited);
        virtual void visit(const PropertyName& visited);
        virtual void visit(const Query& visited);
        virtual void visit(const Select& visited);
        virtual void visit(const SubSelect& visited);
        virtual void visit(const In& visited);


// these methods are not true visitors... but let's keep the idea!
        virtual void visitDistinct(const Distinct& visited);
        virtual void visit(const Fields& visited);
        virtual void visit(const From& visited);
        virtual void visit(const GroupBy& visited);
        virtual void visit(const OrderBy& visited);

      protected:

        const SQLDialect& m_dialect;  //!< The function catalog to use when translating the query.
        std::string& m_sql;           //!< The buffer string where the query will be outputed.

    };

  } // end namespace da
}   // end namespace te


#endif  // __TERRALIB_DATAACCESS_INTERNAL_SQLVISITOR_H

