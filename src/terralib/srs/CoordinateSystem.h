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
  \file CoordinateSystem.h

  \brief This file contains the structs necessary to represent a CoordinateSystem.
*/

#ifndef __TERRALIB_SRS_INTERNAL_COORDINATE_SYSTEM_H 
#define __TERRALIB_SRS_INTERNAL_COORDINATE_SYSTEM_H

#include "Config.h"

#include <string>

namespace te
{
	namespace srs
	{
    /*!
     \class CoordinateSystem
     \brief A Coordinate System (CS).
     A coordinate system (CS) is an abstract mathematical concept without any relationship to a physical object.
    */
    class TESRSEXPORT CoordinateSystem
    {
    
      public:
        /*!
         \brief Default constructor.
         \param name Coordinate System nam.
         \param unit Pointer to the unit used for values in this coordinate system (default a null pointer).
        */
        CoordinateSystem(const std::string& name="", const std::string& unitName="");

        //! Virtual destructor.
        virtual ~CoordinateSystem();

        //! Returns the Coordinate System name.
        const std::string& getName() const { return m_name; }

        //! Sets the Coordinate System name.
        void setName(const std::string& name) { m_name = name; }

        //! Returns the name of unit of measure.
        const std::string& getUnitName() const { return m_unitName; }   

        //! Sets the name of the unit of measure.
        /*!
         \brief Sets the name of the unit of measure.
         \param unit the name of the unit of measure.
        */
        void setUnitName(const std::string& unitName) { m_unitName = unitName; }

        /*!
         \brief Assigns a unique identifier for the CS.
         \param id The CS identifier.
         \param authName The authority responsible for the identifier.
        */
        void setId(int id, const std::string& authName);

        /*!
         \brief Returns the coordinate system unique id managed by an authority.
         \param id To return the unique id (output);
         \param authName To return the authority responsible for the id (output).
        */
        void getId(int& id, std::string& authName) const;

        /*!
         \brief Returns a WKT string that represent the CS.
          WKT definition based on document "OpenGIS® Implementation Specification:
          Coordinate Transformation Services Revision 1.00 - pages 18 and 19.
        */
        virtual std::string getWKT() const = 0;

        //! Returns a flag indicating if this is a geographic coordinate system or not.
        virtual bool isGeographic() const = 0;

      protected:
 
        std::string m_name;      //!< Coordinate system name.
        std::string m_unitName;  //!< Name of the unit of measure.

        int m_id;                 //!< CRS unique identifier.
        std::string m_authority;  //!< The authority providing the unique identifier.
    };
  }
}

#endif //__TERRALIB_SRS_INTERNAL_COORDINATE_SYSTEM_H
