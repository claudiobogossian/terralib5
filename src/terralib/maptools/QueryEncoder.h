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
  \file terralib/maptools/QueryEncoder.h
  
  \brief A visitor that converts a OGC Filter Expression to TerraLib Expression.
 */

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_QUERYENCODER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_QUERYENCODER_H

// TerraLib
#include "Config.h"
#include "../fe/Visitor.h"

// STL
#include <map>

namespace te
{
// Forward declarations
  namespace da
  {
    class Expression;
  }

  namespace map
  {
    /*!
      \class QueryEncoder
      
      \brief A visitor that converts a OGC Filter Expression to TerraLib Expression.

      \sa Visitor
    */
    class TEMAPEXPORT QueryEncoder : public te::fe::Visitor
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction of a Filter visitor.
         */
        //@{

        /*! \brief Default constructor. */
        QueryEncoder();

        /*! \brief Virtual destructor. */
        ~QueryEncoder();

        //@}

        /** @name Conversion Methods
         *  Methods that can be used to convert a OGC Filter Expression to a TerraLib Expression.
         */
        //@{

        /*!
          \brief It converts the OGC Filter Expression to a TerraLib Expression.

          \param f A valid filter expression.

          \return An equivalent TerraLib expression for the given filter.

          \note The caller of this method will take the ownership of the returned expression.
        */
        te::da::Expression* getExpression(const te::fe::Filter* f);

        //@}

        /** @name Visitor Methods
         *  All concrete visitors must implement these methods.
         */
        //@{

        void visit(const te::fe::AbstractOp& visited);
        void visit(const te::fe::SpatialOp& visited);
        void visit(const te::fe::ComparisonOp& visited);
        void visit(const te::fe::LogicOp& visited);
        void visit(const te::fe::BBOXOp& visited);
        void visit(const te::fe::BinaryComparisonOp& visited);
        void visit(const te::fe::BinaryLogicOp& visited);
        void visit(const te::fe::BinarySpatialOp& visited);
        void visit(const te::fe::DistanceBuffer& visited);
        void visit(const te::fe::PropertyIsBetween& visited);
        void visit(const te::fe::PropertyIsLike& visited);
        void visit(const te::fe::PropertyIsNull& visited);
        void visit(const te::fe::UnaryLogicOp& visited);
        void visit(const te::fe::Expression& visited);
        void visit(const te::fe::BinaryOperator& visited);
        void visit(const te::fe::Function& visited);
        void visit(const te::fe::Literal& visited);
        void visit(const te::fe::PropertyName& visited);

        //@}

      private:

        /*! \brief Static method to initialize the QueryEncoder. */
        static void initialize();

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other object.
        */
        QueryEncoder(const QueryEncoder& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other object.

          \return A reference for this.
        */
        QueryEncoder& operator=(const QueryEncoder& rhs);

        //@}

      private:

        te::da::Expression* m_expression;                      //!< Expression used during conversion.
        static std::map<std::string, std::string> sm_fnameMap; //!< A map that associates Filter Operator names to Query Functions names.

        friend class Module;
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_QUERYENCODER_H
