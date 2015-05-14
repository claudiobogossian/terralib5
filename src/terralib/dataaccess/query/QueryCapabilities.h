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
  \file terralib/dataaccess/query/QueryCapabilities.h

  \brief A class that informs the query support of a given data source.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_QUERYCAPABILITIES_H
#define __TERRALIB_DATAACCESS_INTERNAL_QUERYCAPABILITIES_H

// TerraLib
#include "../../geometry/Enums.h"
#include "../Config.h"

// STL
#include <set>
#include <string>

namespace te
{
  namespace da
  {
    /*!
      \class QueryCapabilities

      \brief A class that informs the query support of a given data source.

      \ingroup dataaccess
    */
    class TEDATAACCESSEXPORT QueryCapabilities
    {
      public:

        /*! \brief Constructor. */
        QueryCapabilities();

        /*! \brief Destructor. */
        ~QueryCapabilities();

      public:

        bool supportsSQLDialect() const;

        void setSupportSQLDialect(const bool& support);

        bool supportsSpatialSQLDialect() const;

        void setSupportSpatialSQLDialect(const bool& support);

        bool supportsInsert() const;

        void setSupportInsert(const bool& support);

        bool supportsUpdate() const;

        void setSupportUpdate(const bool& support);

        bool supportsDelete() const;

        void setSupportDelete(const bool& support);

        bool supportsCreate() const;

        void setSupportCreate(const bool& support);

        bool supportsDrop() const;

        void setSupportDrop(const bool& support);

        bool supportsAlter() const;

        void setSupportAlter(const bool& support);

        bool supportsSelect() const;

        void setSupportSelect(const bool& support);

        bool supportsSelectInto() const;

        void setSupportSelectInto(const bool& support);

        void setSupportAll();

        const std::set<std::string>& getSpatialOperators() const;

        void addSpatialOperator(const std::string& op);

        const std::set<std::string>& getSpatialTopologicOperators() const;

        void addSpatialTopologicOperator(const std::string& op);

        const std::set<std::string>& getSpatialMetricOperators() const;

        void addSpatialMetricOperator(const std::string& op);

        const std::set<std::string>& getSpatialNewGeomOperators() const;

        void addSpatialNewGeomOperator(const std::string& op);

        const std::set<std::string>& getLogicalOperators() const;

        void addLogicalOperator(const std::string& op);

        const std::set<std::string>& getComparsionOperators() const;

        void addComparsionOperator(const std::string& op);

        const std::set<std::string>& getArithmeticOperators() const;

        void addArithmeticOperator(const std::string& op);

        const std::set<std::string>& getFunctions() const;

        void addFunction(const std::string& op);

        const std::set<te::gm::GeomType>& getGeometryOperands() const;

        void addGeometryOperand(const te::gm::GeomType& type);

      private:

        bool m_sqlDialect;        //!< A flag that indicates if the data source supports Query API.
        bool m_spatialSQLDialect;  //!< A flag that indicates if the data source supports spatial Query API.
        bool m_insert;            //!< A flag that indicates if the data source supports the INSERT command.
        bool m_update;            //!< A flag that indicates if the data source supports the UPDATE command.
        bool m_delete;            //!< A flag that indicates if the data source supports the DELETE command.
        bool m_create;            //!< A flag that indicates if the data source supports the CREATE command.
        bool m_drop;              //!< A flag that indicates if the data source supports the DROP command.
        bool m_alter;             //!< A flag that indicates if the data source supports the ALTER command.
        bool m_select;            //!< A flag that indicates if the data source supports the SELECT command.
        bool m_selectInto;        //!< A flag that indicates if the data source supports the SELECT INTO command.

        std::set<std::string> m_spatialTopologicOperators;    //!< The names of topological supported operators (touches, crosses, etc).
        std::set<std::string> m_spatialMetricOperators;       //!< The names of spatial metric supported operators (area, lenght, etc).
        std::set<std::string> m_spatialNewGeomOperators;      //!< The names of spatial supported operators that generate new geometries (centroid, buffer, union, intersection,etc).
        std::set<std::string> m_spatialOperators;             //!< The names of other spatial supported operators.

        std::set<std::string> m_logicalOperators;    //!< The names of logical supported operators.
        std::set<std::string> m_comparsionOperators; //!< The names of comparsion supported operators.
        std::set<std::string> m_arithmeticOperators; //!< The names of arithmetic supported operators.
        std::set<std::string> m_functions;           //!< The names of supported functions.
        std::set<te::gm::GeomType> m_geomOperands;   //!< The types of geometry supported operands.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_QUERYCAPABILITIES_H
