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
 \file Ellipsoid.h
 
 \brief This file contains the structs necessary to model an Ellipsoid.
 */

#ifndef __TERRALIB_SRS_INTERNAL_ELLIPSOID_H
#define __TERRALIB_SRS_INTERNAL_ELLIPSOID_H

// TerraLib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace srs
  {
    /*!
       \class Ellipsoid

       \brief A reference ellipsoid is an approximation of the Earth's surface as a squashed sphere.

       \note The terms "spheroid" and "ellipsoid" are synonymous.

       \ingroup srs

       \todo Verify if it is necessary to keep an <Id,Authority> to this object.
    */
    class TESRSEXPORT Ellipsoid
    {
      
    public:
      
      //! Constructor with parameters
      /*!
       \param name Ellipsoid name (default empty string).
       \param rad  Semi-major axis (default 0.0);
       \param invflat Inverse flattening (default 0.0).
       */
      Ellipsoid(const std::string& name = "", double rad = 0.0, double invflat = 0.0);
      
      //! Returns the Ellipsoid name.
      const std::string& getName() const;
      
      /*!
       \brief Sets the Ellipsoid name.
       \param name Ellipsoid name.
       */
      void setName(const std::string& name);
      
      //! Returns the Ellipsoid radium.
      double getRadium() const;
      
      /*!
       \brief Sets the Ellipsoid radium.
       \param rad Ellipsoid radium.
       */
      void setRadium(const double& rad);
      
      //! Returns the Ellipsoid inverse flattening.
      double getInverseFlattening() const;
      
      /*!
       \brief Sets the Ellipsoid inverse flattening.
       \param invflat  Ellipsoid inverse flattening.
       */
      void setInverseFlattening(const double& invflat);
      
      //! Returns a WKT string that represents the ellipsoid.
      std::string getWKT() const;
      
    private:
      
      std::string m_name;        //!< Ellipsoid name
      double m_radium;           //!< Earth equatorial radius (Meters)
      double m_invFlattening;    //!< Earth flattening
    };
  }
}
#endif // __TERRALIB_SRS_INTERNAL_ELLIPSOID_H
