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
  \file terralib/fe/serialization/xml/Expression.h
   
  \brief Auxiliary classes and functions to serialize filter expressions from a XML document.
*/

#ifndef __TERRALIB_FE_SERIALIZATION_XML_INTERNAL_EXPRESSION_H
#define __TERRALIB_FE_SERIALIZATION_XML_INTERNAL_EXPRESSION_H

// TerraLib
#include "../../../common/Singleton.h"
#include "../../../fe/Visitor.h"
#include "../../Config.h"

// STL
#include <map>
#include <string>

// Boost
#include <boost/function.hpp>

namespace te
{
  namespace xml
  {
    class AbstractWriter;
    class Reader;
  }

  namespace fe
  {
    class Expression;

    namespace serialize
    {
      class TEFEEXPORT Expression : public te::common::Singleton<Expression>, public te::fe::Visitor
      {
        friend class te::common::Singleton<Expression>;

        public:

          typedef boost::function<te::fe::Expression* (te::xml::Reader&)> ExpressionReadFnctType;
          typedef std::map<std::string, ExpressionReadFnctType> ExpressionFnctIdxType;

          void reg(const std::string& expName, const ExpressionReadFnctType& fnct);

          te::fe::Expression* read(te::xml::Reader& reader) const;

          void write(const te::fe::Expression* exp, te::xml::AbstractWriter& writer);

          /** @name Visitor Methods
           *  All concrete visitors must implement these methods.
           */
          //@{

          void visit(const te::fe::BinaryOperator& visited);
          void visit(const te::fe::Function& visited);
          void visit(const te::fe::Literal& visited);
          void visit(const te::fe::PropertyName& visited);

          void visit(const te::fe::AbstractOp& visited)         { /* no need */ }
          void visit(const te::fe::SpatialOp& visited)          { /* no need */ }
          void visit(const te::fe::ComparisonOp& visited)       { /* no need */ }
          void visit(const te::fe::LogicOp& visited)            { /* no need */ }
          void visit(const te::fe::BBOXOp& visited)             { /* no need */ }
          void visit(const te::fe::BinaryComparisonOp& visited) { /* no need */ }
          void visit(const te::fe::BinaryLogicOp& visited)      { /* no need */ }
          void visit(const te::fe::BinarySpatialOp& visited)    { /* no need */ }
          void visit(const te::fe::DistanceBuffer& visited)     { /* no need */ }
          void visit(const te::fe::PropertyIsBetween& visited)  { /* no need */ }
          void visit(const te::fe::PropertyIsLike& visited)     { /* no need */ }
          void visit(const te::fe::PropertyIsNull& visited)     { /* no need */ }
          void visit(const te::fe::UnaryLogicOp& visited)       { /* no need */ }
          void visit(const te::fe::Expression& visited)         { /* no need */ }

          //@}

          ~Expression();

        protected:

          Expression();

        private:

          ExpressionFnctIdxType m_fncts;
          te::xml::AbstractWriter* m_writer;
      };

    } // end namespace serialize
  }   // end namespace fe
}     // end namespace te

#endif  // __TERRALIB_FE_SERIALIZATION_XML_INTERNAL_EXPRESSION_H
