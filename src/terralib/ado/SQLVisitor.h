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
  \file terralib/ado/SQLVisitor.h

  \brief A visitor for building an SQL statement using ADO dialect.
*/

#ifndef __TERRALIB_ADO_INTERNAL_SQLVISITOR_H
#define __TERRALIB_ADO_INTERNAL_SQLVISITOR_H

// TerraLib
#include "../dataaccess/query/SQLVisitor.h"
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

// ADO
#import "msado15.dll" \
  no_namespace rename("EOF", "EndOfFile")

namespace te
{
  namespace ado
  {
    /*!
      \class SQLVisitor
      
      \brief A visitor for building an SQL statement using ADO dialect.
    */
    class TEADOEXPORT SQLVisitor : public te::da::SQLVisitor
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief Default constructor. */
        SQLVisitor(const te::da::SQLDialect& dialect, std::string& sql, _ConnectionPtr conn);

        /*! \brief Destructor. */
        ~SQLVisitor() {}

        //@}

        /** @name Select
         *  All concrete visitors must implement these methods.
         */
        //@{

        void visit(const te::da::Function& visited);
        void visit(const te::da::LiteralByteArray& visited);
        void visit(const te::da::LiteralDateTime& visited);
        void visit(const te::da::LiteralEnvelope& visited);
        void visit(const te::da::LiteralGeom& visited);
        void visit(const te::da::PropertyName& visited);

        //@}

      private:

        _ConnectionPtr m_conn; //!< The PostGIS connection used to escape string!
    };

  } // end namespace ado
}   // end namespace te

#endif  // __TERRALIB_ADO_INTERNAL_SQLVISITOR_H
