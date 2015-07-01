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
  \file terralib/ado/Globals.h
   
  \brief An static class with global definitions.
*/

#ifndef __TERRALIB_ADO_INTERNAL_GLOBALS_H
#define __TERRALIB_ADO_INTERNAL_GLOBALS_H

// TerraLib
#include "../common/Static.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
  // Forward declaration
  namespace da
  { 
    class DataSourceCapabilities;
    class SQLDialect;
  }

  namespace ado
  {
    /*!
      \class Globals

      \brief An static class with global definitions.
    */
    class TEADOEXPORT Globals : public te::common::Static
    {
      public:

        static const std::string sm_driverIdentifier;  //!< The ADO driver identifier.

        static te::da::SQLDialect* sm_queryDialect;                //!< The query dialect supported by ADO driver.
        static te::da::DataSourceCapabilities* sm_capabilities;    //!< The query dialect supported by ADO driver.

        static const std::string sm_geometryTypeName;
        static const std::string sm_pointTypeName;
        static const std::string sm_lineStringTypeName;
        static const std::string sm_polygonTypeName;
        static const std::string sm_multiPointTypeName;
        static const std::string sm_multiLineStringTypeName;
        static const std::string sm_multiPolygonTypeName;
        static const std::string sm_curvePolygonTypeName;
        static const std::string sm_polyhedralSurfaceTypeName;
        static const std::string sm_geometryZTypeName;
        static const std::string sm_pointZTypeName;
        static const std::string sm_lineStringZTypeName;
        static const std::string sm_polygonZTypeName;
        static const std::string sm_multiPointZTypeName;
        static const std::string sm_multiLineStringZTypeName;
        static const std::string sm_multiPolygonZTypeName;
        static const std::string sm_curvePolygonZTypeName;
        static const std::string sm_polyhedralSurfaceZTypeName;
        static const std::string sm_geometryMTypeName;
        static const std::string sm_pointMTypeName;
        static const std::string sm_lineStringMTypeName;
        static const std::string sm_polygonMTypeName;
        static const std::string sm_multiPointMTypeName;
        static const std::string sm_multiLineStringMTypeName;
        static const std::string sm_multiPolygonMTypeName;
        static const std::string sm_curvePolygonMTypeName;
        static const std::string sm_polyhedralSurfaceMTypeName;
        static const std::string sm_geometryZMTypeName;
        static const std::string sm_pointZMTypeName;
        static const std::string sm_lineStringZMTypeName;
        static const std::string sm_polygonZMTypeName;
        static const std::string sm_multiPointZMTypeName;
        static const std::string sm_multiLineStringZMTypeName;
        static const std::string sm_multiPolygonZMTypeName;
        static const std::string sm_curvePolygonZMTypeName;
        static const std::string sm_polyhedralSurfaceZMTypeName;
        
    };

  } // end namespace ado
}   // end namespace te

#endif  // __TERRALIB_ADO_INTERNAL_GLOBALS_H

