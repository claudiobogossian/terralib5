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
 \file GeographicCoordinateSystem.h
 
 \brief This file contains the structs necessary to represent a GeographicCoordinateSystem.
 */

#ifndef __TERRALIB_SRS_INTERNAL_GEOGRAPHICCOORDINATESYSTEM_H
#define __TERRALIB_SRS_INTERNAL_GEOGRAPHICCOORDINATESYSTEM_H

// TerraLib
#include "Config.h"
#include "SpatialReferenceSystem.h"

// STL
#include <string>

namespace te
{
  namespace srs
  {
    class Datum;

    /*!
      \class GeographicCoordinateSystem

      \brief A Geographic Coordinate System (GEOGCS).

      A Geographic Coordinate System is a coordinate system based on latitude and longitude.

      \ingroup srs
    */
    class TESRSEXPORT GeographicCoordinateSystem: public SpatialReferenceSystem
    {
    public:
      
      /*!
       \brief Constructor with parameters.
       \param name     The geographic coordinate system name.
       \param unitName The name of the angular unit for this GEOGCS.
       \param primem   The meridian used to take longitude measurements from.
       */
      GeographicCoordinateSystem(const std::string& name="", const std::string unitName="degree", double primem=0.0);
      
      //! Destructor
      ~GeographicCoordinateSystem();
      
      /*!
       \brief Sets the associated Datum.
       \param datum Pointer to GEOGCS datum. Do not pass null. Class takes the pointer ownership.
       */
      void setDatum(Datum* datum);
      
      /*!
       \brief Returns the associated Datum.
       \return A pointer to the GEOGCS datum. Class maintains the pointer ownership.
       */
      const Datum* getDatum() const;
      
      //! Returns the meridian used to take longitude measurements from.
      double getPrimem() const;
      
      /*!
       \brief Sets the meridian used to take longitude measurements from.
       \param primen The meridian used to take longitude measurements from.
       */
      void setPrimem(double primen);
      
      std::string getWKT() const;

      bool isGeographic() const;
      
    private:
      
      Datum* m_datum;    //!< Datum.
      double m_primem;  //!< The meridian used to take longitude measurements from.
    };
  }
}
#endif //__TERRALIB_SRS_INTERNAL_GEOGRAPHICCOORDINATESYSTEM_H
