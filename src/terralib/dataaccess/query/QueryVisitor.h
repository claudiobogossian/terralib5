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
  \file terralib/dataaccess/query/QueryVisitor.h

  \brief A visitor interface for the Query hierarchy.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_QUERYVISITOR_H
#define __TERRALIB_DATAACCESS_INTERNAL_QUERYVISITOR_H

// TerraLib
#include "../Config.h"
#include "../query_fw.h"

namespace te
{
  namespace da
  {
    /*!
      \class QueryVisitor
      
      \brief A visitor interface for the Query hierarchy.

      The visitor interface can be implemented by data access drivers
      in order to translate a Query object to its dialect (SELECT, INSERT, UPDATE, DELETE, DROP, CREATE, ALTER).

      \ingroup dataaccess
    */
    class TEDATAACCESSEXPORT QueryVisitor
    {
      public:

        /*! \brief Default constructor. */
        QueryVisitor() {}

        /*! \brief Virtual destructor. */
        virtual ~QueryVisitor() {}

        virtual void visit(const Expression& visited) = 0;
        virtual void visit(const DataSetName& visited) = 0;
        //virtual void visit(const FromFunctionCall& visited) = 0;
        virtual void visit(const FromItem& visited) = 0;
        virtual void visit(const Function& visited) = 0;
        virtual void visit(const Insert& visited) = 0;
        virtual void visit(const Join& visited) = 0;
        virtual void visit(const JoinCondition& visited) = 0;
        virtual void visit(const JoinConditionOn& visited) = 0;
        virtual void visit(const JoinConditionUsing& visited) = 0;
        virtual void visit(const Literal& visited) = 0;
        virtual void visit(const LiteralBool& visited) = 0;
        virtual void visit(const LiteralByteArray& visited) = 0;
        virtual void visit(const LiteralDateTime& visited) = 0;
        virtual void visit(const LiteralDouble& visited) = 0;
        virtual void visit(const LiteralEnvelope& visited) = 0;
        virtual void visit(const LiteralGeom& visited) = 0;
        virtual void visit(const LiteralInt16& visited) = 0;
        virtual void visit(const LiteralInt32& visited) = 0;
        virtual void visit(const LiteralInt64& visited) = 0;
        virtual void visit(const LiteralString& visited) = 0;
        virtual void visit(const PropertyName& visited) = 0;
        virtual void visit(const Query& visited) = 0;
        virtual void visit(const Select& visited) = 0;
        virtual void visit(const SelectExpression& visited) = 0;
        virtual void visit(const SubSelect& visited) = 0;
        virtual void visit(const In& visited) = 0;
    };

  } // end namespace da
}   // end namespace te


#endif  // __TERRALIB_DATAACCESS_INTERNAL_QUERYVISITOR_H

