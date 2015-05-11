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
 \file SpatialReferenceSystem.h
 
 \brief This file contains the structs necessary to represent a Spatial Reference System.
 */

#ifndef __TERRALIB_SRS_INTERNAL_SPATIALREFERENCESYSTEM_H
#define __TERRALIB_SRS_INTERNAL_SPATIALREFERENCESYSTEM_H

// TerraLib
#include "Config.h"

// STL
#include <memory>
#include <string>

namespace te
{
  namespace srs
  {
    /*!
      \class SpatialReferenceSystem
      \brief A Spatial Reference System, also known as a Coordinate System.

      A spatial reference system (SRS) or coordinate reference system (CRS) is a coordinate-based local,
      regional or global system used to locate geographical entities.

      \ingroup srs
    */
    class TESRSEXPORT SpatialReferenceSystem
    {
      
    public:
      /*!
       \brief Default constructor.
       \param name The SRS name. Default empty string.
       \param unitName The SRS unit name.  Default empty string;
       */
      SpatialReferenceSystem(const std::string& name="", const std::string& unitName="");
      
      //! Virtual destructor.
      virtual ~SpatialReferenceSystem();
      
      /*!
       \brief Sets the SRS name.
       \param name the SRS name.
       */
      void setName(const std::string& name);
      
      //! Returns the SRS name.
      const std::string& getName() const;
      
      /*!
       \brief Sets the SRS unit name.
       \param name the SRS unit name.
       */
      void setUnitName(const std::string& name);
      
      //! Returns the SRS unit name.
      const std::string& getUnitName() const;
      
      /*!
       \brief Sets the SRS integer identification.
       
       Spatial reference systems can be referred to using a SRID integer,
       given by an authority, such as the EPSG codes defined by the International
       Association of Oil and Gas Producers.
       
       \param id The SRS integer identifier.
       \param authName The authority responsible for the identifier. Default "EPSG".
       */
      void setSRID(int id, const std::string& authName="EPSG");
      
      /*!
       \brief Returns SRS integer identification.
       \param id To return the unique id (output);
       \param authName To return the authority responsible for the id (output).
       */
      void getSRID(int& id, std::string& authName) const;
      
      /*!
       \brief Returns an OGC's WKT representation for the SRS.
       
       The WKT definition based on document "OpenGIS Implementation Specification: Coordinate Transformation Services Revision 1.00 - pages 18 and 19.
       */
      virtual std::string getWKT() const = 0;
      
      //! Returns true is a geographic coordinate system and false otherwise.
      virtual bool isGeographic() const = 0;
      
    protected:
      
      std::string m_name;
      std::string m_unitName;
      
      std::pair<unsigned int,std::string> m_srid;
    };
    
    /*!
     \typedef SpatialReferenceSystemPtr an auto pointer to a SpatialReferenceSystem.
     */
    typedef std::auto_ptr<SpatialReferenceSystem> SpatialReferenceSystemPtr;
  }
}

#endif //__TERRALIB_SRS_INTERNAL_SPATIALREFERENCESYSTEM_H
