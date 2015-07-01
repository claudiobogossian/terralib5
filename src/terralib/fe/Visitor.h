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
  \file Visitor.h
  
  \brief A visitor interface for a Filter expression.
 */

#ifndef __TERRALIB_FE_INTERNAL_VISITOR_H
#define __TERRALIB_FE_INTERNAL_VISITOR_H

// TerraLib
#include "../fe_fw.h"
#include "Config.h"

namespace te
{
  namespace fe
  {
    /*!
      \class Visitor
      
      \brief A visitor interface for a Filter expression.

      The visitor interface can be implemented by database drivers
      in order to translate a Filter object to a SQL statement (mostly the where clause).      

      \sa Visitor, FilterCapabilitiesVisitor
     */
    class TEFEEXPORT Visitor
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction of a Filter visitor.
         */
        //@{

        /*! \brief Default constructor. */
        Visitor() {}

        /*! \brief Virtual destructor. */
        virtual ~Visitor() {}

        //@}

        /** @name Visitor Methods
         *  All concrete visitors must implement these methods.
         */
        //@{

        virtual void visit(const AbstractOp& visited) = 0;
        virtual void visit(const SpatialOp& visited) = 0;
        virtual void visit(const ComparisonOp& visited) = 0;
        virtual void visit(const LogicOp& visited) = 0;
        virtual void visit(const BBOXOp& visited) = 0;
        virtual void visit(const BinaryComparisonOp& visited) = 0;
        virtual void visit(const BinaryLogicOp& visited) = 0;
        virtual void visit(const BinarySpatialOp& visited) = 0;
        virtual void visit(const DistanceBuffer& visited) = 0;
        virtual void visit(const PropertyIsBetween& visited) = 0;
        virtual void visit(const PropertyIsLike& visited) = 0;
        virtual void visit(const PropertyIsNull& visited) = 0;
        virtual void visit(const UnaryLogicOp& visited) = 0;
        virtual void visit(const Expression& visited) = 0;
        virtual void visit(const BinaryOperator& visited) = 0;
        virtual void visit(const Function& visited) = 0;
        virtual void visit(const Literal& visited) = 0;
        virtual void visit(const PropertyName& visited) = 0;

        //@}

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other object.
         */
        Visitor(const Visitor& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other object.

          \return A reference for this.
         */
        Visitor& operator=(const Visitor& rhs);

        //@}
    };

  } // end namespace fe
}   // end namespace te

#endif  // __TERRALIB_FE_INTERNAL_VISITOR_H
