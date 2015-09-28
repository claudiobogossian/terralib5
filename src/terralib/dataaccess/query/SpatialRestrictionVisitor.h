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
  \file terralib/dataaccess/query/SpatialRestrictionVisitor.h

  \brief A visitor that retrieves spatial restrictions from a Query hierarchy.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_SPATIALRESTRICTIONVISITOR_H
#define __TERRALIB_DATAACCESS_INTERNAL_SPATIALRESTRICTIONVISITOR_H

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
// Forward declaration
  namespace gm
  {
    class Geometry;
  }

  namespace da
  {
    /*!
      \struct SpatialRestriction
      
      \brief A struct that represents a spatial restriction.
    */
    struct SpatialRestriction
    {
      /*! \brief Default constructor. */
      SpatialRestriction();

       /*! \brief Destructor. */
      ~SpatialRestriction();

      std::size_t m_index;              //!< Internal index of the spatial restriction.
      te::gm::SpatialRelation m_type;   //!< The spatial restriction type.
      te::gm::Geometry* m_geometry;     //!< The geometry of the spatial restriction.
      std::string m_pname;              //!< The property name of the spatial restriction.
      bool m_isFromEnvelope;            //!< A flag that indicates if the spatial restriction uses an envelope.
      const Function* m_function;       //!< The function that represents the spatial restriction.
    };

    /*!
      \class SpatialRestrictionVisitor
      
      \brief A visitor for retrieves spatial restrictions from a Query hierarchy.
    */
    class TEDATAACCESSEXPORT SpatialRestrictionVisitor : public QueryVisitor, public boost::noncopyable
    {
      public:

        /*! \brief Default constructor. */
        SpatialRestrictionVisitor();

        /*! \brief Virtual destructor. */
        virtual ~SpatialRestrictionVisitor();

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

        bool hasSpatialRestrictions() const;

        const std::vector<te::da::SpatialRestriction*>& getSpatialRestrictions() const;

      protected:

        virtual void initialize();

        bool isSpatialRestrictionFunction(const Function& f) const;

        te::gm::SpatialRelation getSpatialRelation(const Function& f) const;

        bool isFromEnvelope(const Function& f) const;

        te::gm::Geometry* getGeometryRestriction(const Function& f) const;

        std::string getPropertyName(const Function& f) const;

        void addSpatialRestriction(const Function& f);

      protected:

        std::map<std::string, te::gm::SpatialRelation> m_spatialFunctions; //!< A map of spatial restriction functions.
        std::size_t m_index;                                               //!< An internal visitor index.
        std::vector<te::da::SpatialRestriction*> m_spatialRestrictions;    //!< The set of spatial restrictions.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_SPATIALRESTRICTIONVISITOR_H
