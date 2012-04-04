/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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

#ifndef __TERRALIB_SRS_INTERNAL_PROJECTED_COORDINATE_SYSTEM_H 
#define __TERRALIB_SRS_INTERNAL_PROJECTED_COORDINATE_SYSTEM_H

#include "Config.h"
#include "GeographicCoordinateSystem.h"
#include "Datum.h"

#include "../common/UnitOfMeasure.h"

#include <string>
#include <map>

namespace te
{
	namespace srs
	{
    /*!
    \brief A Projected Coordinate System (PROJCS).
      A coordinate reference system that is based on a geographic CRS and then 
      uses a map projection to convert the coordinates to a plane.
    */
    class TESRSEXPORT ProjectedCoordinateSystem : public CoordinateSystem
    {
    
    public:

      //! Default constructor.
      ProjectedCoordinateSystem();

      /*!
        \brief Constructor from parameters.

        \param name   Projected coordinate system name.
        \param unit   The name of the linear unit associated to the PROJCS 
        (default value is "METER").
      */
      ProjectedCoordinateSystem(const std::string& name, 
                                const std::string& unitName="METER");
   
      //! Destructor.
      ~ProjectedCoordinateSystem();

      /*!
        \brief Sets the the Geographic Coordinate Reference System.
        \param geogcs The Geographic Coordinate Reference System.
        \note This object will take the ownership of the pointer.
       */
      void setGeographicCoordinateSystem(GeographicCoordinateSystem* geogcs) 
        { m_geogcs = geogcs; }      
      
      /*!
        \brief It returns the Geographic Coordinate Reference System.
        \return The Geographic Coordinate Reference System.
       */
      const GeographicCoordinateSystem* getGeographicCoordinateSystem() const 
        { return m_geogcs; }

      /*!
        \brief Sets the name of a projection from geographic coordinates to 
               projected coordinates.
        \param proj The name of a projection.
       */
      void setProjection(const std::string& proj) { m_projection = proj; } 

      /* 
        \brief Returns the name of a projection from geographic coordinates to 
               projected coordinates.
      */
      const std::string& getProjection() const { return m_projection; } 

      /*!
        \brief Sets the projection parameters.
        \param params The projection parameters.
       */
      void setParameters(const std::map<std::string, double>& params) 
      { m_params = params; }

      //! Access (for reading only) the set of parameters.
      const std::map<std::string, double>& getParameters() const 
      { return m_params; }
      
      //! Returns a WKT string that represent the PROJCS.
      std::string getWKT() const;

      bool isGeographic() const
      { return false; }

    private:

      GeographicCoordinateSystem*   m_geogcs;	      
      std::string                   m_projection;   
	    std::map<std::string, double> m_params;	  
    };
  }
}
#endif // __TERRALIB_SRS_INTERNAL_PROJECTED_COORDINATE_SYSTEM_H
