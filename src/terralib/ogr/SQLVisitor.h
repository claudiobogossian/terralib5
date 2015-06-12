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
  \file terralib/ogr/SQLVisitor.h

  \brief A visitor for building an SQL statement using OGR dialect.
*/

#ifndef __TERRALIB_OGR_INTERNAL_SQLVISITOR_H
#define __TERRALIB_OGR_INTERNAL_SQLVISITOR_H

// TerraLib
#include "../dataaccess/query/SQLVisitor.h"
#include "Config.h"

namespace te
{
  namespace gm
  {
    // Forward declaration
    class Envelope;
  }

  namespace ogr
  {
    class TEOGREXPORT SQLVisitor : public te::da::SQLVisitor
    {
      public:
        /*! \brief Default constructor. */
        SQLVisitor(const te::da::SQLDialect& dialect, std::string& sql);

        /*! \brief Destructor. */
        ~SQLVisitor() {}

        void visit(const te::da::DataSetName& visited);

        void visit(const te::da::LiteralEnvelope& visited);

        void visit(const te::da::PropertyName& visited);

        void visit(const te::da::Select& visited);

        void visit(const te::da::Join& visited);

        void visit(const te::da::JoinConditionOn& visited);

        void visitDistinct(const te::da::Distinct& visited);

        te::gm::Envelope* getMBR();

      protected:

        te::gm::Envelope* m_bbox;
    };
  }
}


#endif //__TERRALIB_OGR_INTERNAL_SQLVISITOR_H
