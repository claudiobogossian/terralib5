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
  \file Ellipsoid.h

  \brief This file contains the structs necessary to model an Ellipsoid.
*/

#ifndef __TERRALIB_SRS_INTERNAL_ELLIPSOID_H 
#define __TERRALIB_SRS_INTERNAL_ELLIPSOID_H

#include "Config.h"

#include <string>

namespace te
{
	namespace srs
	{
    /*!
     \class Ellipsoid
     \brief A reference ellipsoid is an approximation of the Earth's surface as a squashed sphere.
     \note The terms "spheroid" and "ellipsoid" are synonymous.
     \todo Verify if it is necessary to keep an <Id,Authority> to this object.
    */
    class TESRSEXPORT Ellipsoid
    {
    
    public:

      /*!
        \brief Initializes a new Ellipsoid from parameters.

        \param name Ellipsoid name.
        \param rad  Semi-major axis
        \param invflat Inverse flattening.
      */
      Ellipsoid(const std::string& name = "", double rad = 0.0, double invflat = 0.0);

      /*!
        \brief Returns the Ellipsoid name.
        \return  Ellipsoid name.
       */
      const std::string& getName() const { return m_name; }
      
      /*!
        \brief Sets the Ellipsoid name.        
        \param name Ellipsoid name.
       */
      void setName(const std::string& name) { m_name = name; }

      /*!
        \brief Returns the Ellipsoid radium.
        \return Ellipsoid radium.
       */
      double getRadium() const { return m_radium; }

      /*!
        \brief Sets the Ellipsoid radium.        
        \param rad Ellipsoid radium.
       */
      void setRadium(const double& rad) { m_radium = rad; }

      /*!
        \brief Returns the Ellipsoid inverse flattening.
        \return  Ellipsoid inverse flattening.
       */
      double getInverseFlattening() const { return m_invFlattening; }

      /*!
        \brief Sets the Ellipsoid inverse flattening.
        \param invflat  Ellipsoid inverse flattening.
       */
      void setInverseFlattening(const double& invflat) { m_invFlattening = invflat; }

      //! Returns a WKT string that represent the ellipsoid.
      std::string getWKT() const;

    private:
      
      std::string m_name;        //!< Ellipsoid name
      double m_radium;           //!< Earth equatorial radius (Meters)
      double m_invFlattening;    //!< Earth flattening
    };
  }
}
#endif // __TERRALIB_SRS_INTERNAL_ELLIPSOID_H
