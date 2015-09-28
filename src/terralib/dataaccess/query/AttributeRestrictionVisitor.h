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
  \file terralib/dataaccess/query/AttributeRestrictionVisitor.h

  \brief A visitor that retrieves attribute restrictions from a Query hierarchy.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_ATTRIBUTERESTRICTIONVISITOR_H
#define __TERRALIB_DATAACCESS_INTERNAL_ATTRIBUTERESTRICTIONVISITOR_H

// TerraLib 
#include "../../geometry/Enums.h"
#include "QueryVisitor.h"

// Boost
#include <boost/noncopyable.hpp>

// STL
#include <map>
#include <string>
#include <vector>

namespace te
{
  namespace da
  {
    /*!
      \struct AttributeRestriction
      
      \brief A struct that represents an attribute restriction.
    */
    struct AttributeRestriction
    {
      /*! \brief Default constructor. */
      AttributeRestriction();

       /*! \brief Destructor. */
      ~AttributeRestriction();

      std::size_t m_index;        //!< Internal index of the attribute restriction.
      std::string m_op;           //!< The attribute restriction operator.
      const Function* m_function; //!< The function that represents the attribute restriction.
    };

    /*!
      \class AttributeRestrictionVisitor
      
      \brief A visitor that retrieves attribute restrictions from a Query hierarchy.
    */
    class TEDATAACCESSEXPORT AttributeRestrictionVisitor : public QueryVisitor, public boost::noncopyable
    {
      public:

        /*! \brief Default constructor. */
        AttributeRestrictionVisitor();

        /*! \brief Virtual destructor. */
        virtual ~AttributeRestrictionVisitor();

        virtual void visit(const Expression& visited) {}
        virtual void visit(const DataSetName& visited) {}
        virtual void visit(const FromItem& visited) {}
        virtual void visit(const Function& visited);
        virtual void visit(const Insert& visited) {}
        virtual void visit(const Join& visited) {}
        virtual void visit(const JoinCondition& visited) {}
        virtual void visit(const JoinConditionOn& visited) {}
        virtual void visit(const JoinConditionUsing& visited) {}
        virtual void visit(const Literal& visited) {}
        virtual void visit(const LiteralBool& visited) {}
        virtual void visit(const LiteralByteArray& visited) {}
        virtual void visit(const LiteralDateTime& visited) {}
        virtual void visit(const LiteralDouble& visited) {}
        virtual void visit(const LiteralEnvelope& visited) {}
        virtual void visit(const LiteralGeom& visited) {}
        virtual void visit(const LiteralInt16& visited) {}
        virtual void visit(const LiteralInt32& visited) {}
        virtual void visit(const LiteralInt64& visited) {}
        virtual void visit(const LiteralString& visited) {}
        virtual void visit(const PropertyName& visited) {}
        virtual void visit(const Query& visited) {}
        virtual void visit(const Select& visited);
        virtual void visit(const SelectExpression& visited) {}
        virtual void visit(const SubSelect& visited) {}
        virtual void visit(const In& visited) {}

        bool hasAttributeRestrictions() const;

        const std::vector<te::da::AttributeRestriction*>& getAttributeRestrictions() const;

      protected:

        virtual void initialize();

        bool isAttributeRestrictionFunction(const Function& f) const;

        std::string getOperator(const Function& f) const;

        void addAttributeRestriction(const Function& f);

      protected:

        std::map<std::string, std::string> m_attrFunctions;            //!< A map of attribute restriction functions.
        std::size_t m_index;                                           //!< An internal visitor index.
        std::vector<te::da::AttributeRestriction*> m_attrRestrictions; //!< The set of attribute restrictions.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_ATTRIBUTERESTRICTIONVISITOR_H
