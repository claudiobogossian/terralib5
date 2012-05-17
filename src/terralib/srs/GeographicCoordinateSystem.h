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
  \file GeographicCoordinateSystem.h

  \brief This file contains the structs necessary to represent a GeographicCoordinateSystem.
*/

#ifndef __TERRALIB_SRS_INTERNAL_GEOGRAPHIC_COORDINATE_SYSTEM_H 
#define __TERRALIB_SRS_INTERNAL_GEOGRAPHIC_COORDINATE_SYSTEM_H

#include "Config.h"
#include "CoordinateSystem.h"
#include "Datum.h"

#include "../common/UnitOfMeasure.h"

#include <string>

namespace te
{
	namespace srs
	{
    /*!
     \class GeographicCoordinateSystem
     \brief A Geographic Coordinate System (GEOGCS).
      A Geographic Coordinate System is a coordinate system based on latitude and longitude.
    */
    class TESRSEXPORT GeographicCoordinateSystem: public CoordinateSystem
    {
      public:

        //! Default constructor.
        GeographicCoordinateSystem();

        /*!
          \brief Constructor from parameters.
          
          \param name The geographic coordinate system name.
          \param datum The horizontal datum which corresponds to the procedure used to measure positions on the surface of the Earth.
          \param primem The meridian used to take longitude measurements from.
          \param angUnit The name of the angular unit for this GEOGCS (default value is "DEGREE").
        */
        GeographicCoordinateSystem(const std::string& name, const Datum& datum, 
                                   double primem=0.0, const std::string& unitName="DEGREE");

        //! Returns the associated Datum.
        const te::srs::Datum& getDatum() const { return m_datum; }

        /*!
          \brief Sets the associated Datum.
          \param datum The Datum that will be associated to the Geographic Coordinate System.
         */
        void setDatum(const Datum& datum) { m_datum = datum; }

        //! Returns the meridian used to take longitude measurements from.
        double getPrimem() const { return m_primem; } 

        /*!
          \brief Sets the meridian used to take longitude measurements from.
          \param primem The meridian used to take longitude measurements from.
         */
        void setPrimem(const double& primem) { m_primem = primem; }

        //! Returns a WKT string that represent the GCRS.
        std::string getWKT() const;

        bool isGeographic() const
        { return true; }

      private:

        Datum m_datum;    //!< Datum.
        double m_primem;  //!< The meridian used to take longitude measurements from.
    };
  }
}
#endif //__TERRALIB_SRS_INTERNAL_GEOGRAPHIC_COORDINATE_SYSTEM_H
