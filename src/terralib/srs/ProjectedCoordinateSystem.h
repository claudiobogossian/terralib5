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
 \file ProjectedCoordinateSystem.h
 
 \brief This file contains the structs necessary to represent a ProjectedCoordinateSystem.
 */

#ifndef __TERRALIB_SRS_INTERNAL_PROJECTEDCOORDINATESYSTEM_H
#define __TERRALIB_SRS_INTERNAL_PROJECTEDCOORDINATESYSTEM_H

// TerraLib
#include "Config.h"
#include "SpatialReferenceSystem.h"

// STL
#include <map>
#include <string>


namespace te
{
  namespace srs
  {
    class GeographicCoordinateSystem;

    /*!
    
      \class ProjectedCoordinateSystem

      \brief A Projected Coordinate System (PROJCS).

      A coordinate reference system that is based on a geographic CRS and then uses a map projection to convert the coordinates to a plane.

      \ingroup srs
     */
    class TESRSEXPORT ProjectedCoordinateSystem : public SpatialReferenceSystem
    {
      
    public:
      
      /*!
       \brief Constructor with parameters.
       \param name       Projected coordinate system name (default empty string).
       \param unitName   The name of the linear unit associated to the PROJCS (default empty string).
       \param geogcs     Pointer to the underlying geographic coordinate system (default null). Class takes pointer ownership.
       \param projName   The name of the map projection (default empty string).
       */
      ProjectedCoordinateSystem(const std::string& name = "",
                                const std::string& unitName = "",
                                GeographicCoordinateSystem* geogcs=0,
                                const std::string& projName = "");
      
      //! Destructor.
      ~ProjectedCoordinateSystem();
      
      /*!
       \brief Sets the underlying Geographic Coordinate Reference System.
       \param geogcs A pointer to Geographic Coordinate Reference System. Do not pass null. Class takes pointer ownership.
       */
      void setGeographicCoordinateSystem(GeographicCoordinateSystem* geogcs);
      
      /*!
       \brief Returns the underlying Geographic Coordinate Reference System.
       \return A pointer to Geographic Coordinate Reference System. Class maintains pointer ownership.
       */
      const GeographicCoordinateSystem* getGeographicCoordinateSystem() const;
      
      /*!
       \brief Sets the map projection name.
       \param projname The map projection name.
       */
      void setProjection(const std::string& projname);
      
      //! Returns the map projection name.
      const std::string& getProjection() const;
      
      /*!
       \brief Sets the projection parameters.
       \param params The projection parameters.
       */
      void setParameters(const std::map<std::string, double>& params);
      
      //! Returns the list of projection parameters.
      const std::map<std::string, double>& getParameters() const;
      
      std::string getWKT() const;
      
      bool isGeographic() const;
      
    private:
      
      GeographicCoordinateSystem*   m_geogcs;
      std::string                   m_projection;
	    std::map<std::string, double> m_params;
    };
  }
}
#endif // __TERRALIB_SRS_INTERNAL_PROJECTEDCOORDINATESYSTEM_H
