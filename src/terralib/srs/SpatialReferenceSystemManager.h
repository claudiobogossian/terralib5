/*  Copyright (C) 2001-2012 National Institute For Space Research (INPE) - Brazil.
 
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
 \file SpatialReferenceSystemManager.h
 
 \brief A class to manage Coordinate Systems representations.
 */

#ifndef __TERRALIB_SRS_INTERNAL_SPATIAL_REFERENCE_SYSTEM_MANAGER_H
#define __TERRALIB_SRS_INTERNAL_SPATIAL_REFERENCE_SYSTEM_MANAGER_H

#include "Config.h"

#include "../common/Singleton.h"

#include <string>
#include <map>
#include <vector>

namespace te
{
  namespace srs
  {
    class SpatialReferenceSystem;
    
    /*!
     \class SpatialReferenceSystemManager
     
     \brief A class to manage Coordinate Systems representations within TerraLib environment.
     
     A Coordinate System used to describe coordinates are also known as Spatial Reference System.
     It can be represented by either one of these properties: 
       - A pair <id,authority>: a unique numeric id given by a particular authority; 
       - A simple, human-readable, interface-friendly, name;
       - An OGC's WKT description;
       - A PROJ4 text: to support coordinate conversion.
     
     Refer to the <a href="http://www.spatialreference.org">Spatial Reference website</a> for more information about EPSG codes for SRS.
     */
    class TESRSEXPORT SpatialReferenceSystemManager : public te::common::Singleton<SpatialReferenceSystemManager>
    {
      friend class te::common::Singleton<SpatialReferenceSystemManager>;
    
    public:
      
      /* \brief Destructor. */
      ~SpatialReferenceSystemManager();
            
      /*!
       \brief Adds a  <id, authority> to the manager.
       \param name A simple, human-readable, interface-friendly, name;
       \param p4Txt a PROJ4 library description of coordinate system;
       \param wkt An OGC's WKT description for the coordinate system;
       \param id SRS id.
       \param authName The authority responsible for the id. Default "EPSG".
       \exception te::srs::Exception if the coordinate system id is already registered in the manager.
       */      
      void add(const std::string& name, const std::string& p4Txt, const std::string& wkt, unsigned int id, const std::string& authName="EPSG");
      
      /*!
       \brief Returns true is a pair <id, authority> is recognized by the manager.
       \param id The coordinate system identification.
       \param authName The authority responsible for the id. Default "EPSG".
       \return True if the pair <id,authority> is already registered in the manager and false otherwise.
       */
      bool recognizes(unsigned int id, const std::string& authName="EPSG") const;
      
      /*!
       \brief Returns a coordinate system name given an identification.
       \param id The coordinate system identification.
       \param authName The authority responsible for the id. Default "EPSG".
       \return the Coordinate System name if the identification is recognized or and empty string otherwise.
       */
      std::string getName (unsigned int id, const std::string& authName="EPSG") const;
      
      /*!
       \brief Returns a coordinate system PROJ4 description given an identification.
       \param id The coordinate system identification.
       \param authName The authority responsible for the id. Default "EPSG".
       \return The coordinate system PROJ4 description if the identification is recognized or an empty string otherwise.
       */
      std::string getP4Txt(unsigned int id, const std::string& authName="EPSG") const; 
      
      /*!
       \brief Returns a coordinate system WKT description given an id.
       \param id The coordinate system identification.
       \param authName The authority responsible for the id. Default "EPSG".
       \return The coordinate system OGC WKT if the identification is recognized or an empty string otherwise.
       */ 
      std::string getWkt  (unsigned int id, const std::string& authName="EPSG") const;

      /*!
       \brief Returns a coordinate system identification given a name.
       \param name The coordinate system name.
       \return A pair composed by the coordinate system identification and the name of the authority responsible for it.
       \exception te::srs::Exception if the coordinate system name is not found in the manager.
       */ 
      std::pair<std::string,unsigned int> getIdFromName (const std::string& name) const;
      
      /*!
       \brief Returns a coordinate system identification given a PROJ4 description.
       \param name The coordinate system PROJ4 description.
       \return A pair composed by the coordinate system identification and the name of the authority responsible for it.
       \exception te::srs::Exception if the coordinate system PROJ4 description is not found in the manager.
       */
      std::pair<std::string,unsigned int> getIdFromP4Txt(const std::string& p4Txt) const; 
      
      /*!
       \brief Returns a coordinate system identification given a WKT description.
       \param name The coordinate system WKT.
       \return A pair composed by the coordinate system identification and the name of the authority responsible for it.
       \exception te::srs::Exception if the coordinate system WKT is not found in the manager.
       */
      std::pair<std::string,unsigned int> getIdFromWkt  (const std::string& wkt) const;

      /*!
       \brief Returns a pointer to a coordinate system given an identification.
       \param id The coordinate system identification.
       \param authName The authority responsible for the id. Default "EPSG".
       \return A pointer to a coordinate system if the identification is recognized or a null pointer otherwise. Caller is responsible for deleting the returned pointer.
       */
      SpatialReferenceSystem* getSpatialReferenceSystem(unsigned int id, const std::string& authName="EPSG") const;
      
      /*!
       \brief Removes a coordinate system representation from the manager, given its identification.
       \param id The coordinate system identification.
       \param authName The authority responsible for the id. Default "EPSG".
       */
      void remove (unsigned int id, const std::string& authName="EPSG");
      
      /*!
       \brief Removes all coordinate system representations from the manager.
      */
      void clear();
 
      /*!
       \brief Returns an iterator mechanism over the coordinate system identifications in the manager.
       The first iterator of the returned pair points to first coordinate system identification <id,authority> in the manager.
       The second iterator of the returned pair points to the last plus one coordinate system identification <id,authority> in the manager.
        \return a pair of iterators pointing to the first and last coordnate system representation in the manager.
       */     
      std::pair<std::vector<std::pair<std::string,unsigned int> >::const_iterator,
                std::vector<std::pair<std::string,unsigned int> >::const_iterator> getIteratorIds() const;

      /*!
       \brief Returns an iterator mechanism over the coordinate system names in the manager.
       The first iterator of the returned pair points to first coordinate system name in the manager.
       The second iterator of the returned pair points to the last plus one coordinate system name in the manager.
       \return a pair of iterators pointing to the first and last coordnate system representation in the manager.
       */
      std::pair<std::vector<std::string>::const_iterator,
                std::vector<std::string>::const_iterator> getIteratorNames() const;
      
    protected:
      
      /*! \brief Constructor. */
      SpatialReferenceSystemManager();
      
    private:
      
      /*!
       \brief Copy constructor not allowed.
       
       \param rhs The right-hand-side copy that would be used to copy from.
       */
      SpatialReferenceSystemManager(const SpatialReferenceSystemManager& rhs);
      
      /*!
       \brief Assignment operator not allowed.
       
       \param rhs The right-hand-side copy that would be used to copy from.
       
       \return A reference to this object.
       */
      SpatialReferenceSystemManager& operator=(const SpatialReferenceSystemManager& rhs);
      
    private:
      
      std::vector<std::pair<std::string,unsigned int> > m_authIdV;
      std::vector<std::string> m_nameV;
      std::vector<std::string> m_p4txtV;
      std::vector<std::string> m_wktV;
    };
  }
}
#endif // __TERRALIB_SRS_INTERNAL_SPATIAL_REFERENCE_SYSTEM_MANAGER_H
