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
 \file SpatialReferenceSystemManager.h
 
 \brief A class to manage Coordinate Systems representations.
 */

#ifndef __TERRALIB_SRS_INTERNAL_SPATIALREFERENCESYSTEMMANAGER_H
#define __TERRALIB_SRS_INTERNAL_SPATIALREFERENCESYSTEMMANAGER_H

// TerraLib
#include "../common/Singleton.h"
#include "../common/UnitOfMeasure.h"
#include "Config.h"
#include "SpatialReferenceSystem.h"

// STL
#include <map>
#include <string>
#include <vector>
#include <memory>

// Boost
#include <boost/lexical_cast.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>

namespace te
{
  namespace srs
  {
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

      \ingroup srs
    */
    class TESRSEXPORT SpatialReferenceSystemManager : public te::common::Singleton<SpatialReferenceSystemManager>
    {
      friend class te::common::Singleton<SpatialReferenceSystemManager>;
      
    private:
      
      /*! This is a private struct to handle the set of SRS descriptions in the manager.
       It is not visible outside this class. */
      struct srs_desc
      {
        srs_desc(const std::string& name, unsigned int auth_id, const std::string& auth_name, const std::string& p4txt, const std::string& wkt);
        
        std::string srid() const;
        
        std::string m_name;
        unsigned int m_auth_id;
        std::string m_auth_name;
        std::string m_p4txt;
        std::string m_wkt;
      };
      
      /*! A mult-index container with the following indexes:
       1) an unique index by <auth_name, id>;
       2) a non-unique index by name;
       3) a non-unique index by p4txt;
       4) a non-unique index by wkt;
       5) a non-unique index by auth_name;
       */
      typedef boost::multi_index_container<
      srs_desc,
      boost::multi_index::indexed_by<
      boost::multi_index::ordered_unique<BOOST_MULTI_INDEX_CONST_MEM_FUN(srs_desc,std::string,srid)>,
      boost::multi_index::ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(srs_desc,std::string,m_name)>,
      boost::multi_index::ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(srs_desc,std::string,m_p4txt)>,
      boost::multi_index::ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(srs_desc,std::string,m_wkt)>,
      boost::multi_index::ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(srs_desc,std::string,m_auth_name)>
      >
      > srs_set;
      
    private:
      
      srs_set m_set;
      
    public:
      
      //! An iterator by SRS <id,authority>
      typedef boost::multi_index::nth_index<srs_set,0>::type::iterator iterator;
      
      //! Destructor.
      ~SpatialReferenceSystemManager();
      
      /*!
       \brief Inializes the manager from a JSON file containing instances of SRSs
       
       This methods reads the file "TE_JSON_FILES_LOCATION/srs.json" for SRSs definitions and insert them on the manager if it is empty.
       \exception te::srs::Exception if the JSON file is not well formed.
       */
      void init();

      /*!
       \brief Inializes the manager from a JSON file containing instances of SRSs.

       \param fileName Name of the JSON file.
       \exception te::srs::Exception if the JSON file is not well formed.
       */
      void init(const std::string& fileName);
      
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
       \param p4Txt The coordinate system PROJ4 description.
       \return A pair composed by the coordinate system identification and the name of the authority responsible for it.
       \exception te::srs::Exception if the coordinate system PROJ4 description is not found in the manager.
       */
      std::pair<std::string,unsigned int> getIdFromP4Txt(const std::string& p4Txt) const;
      
      /*!
       \brief Returns a coordinate system identification given a WKT description.
       \param wkt The coordinate system WKT.
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
      SpatialReferenceSystemPtr getSpatialReferenceSystem(unsigned int id, const std::string& authName="EPSG") const;
      
      /*!
       \brief Removes a coordinate system representation from the manager, given its identification.
       \param id The coordinate system identification.
       \param authName The authority responsible for the id. Default "EPSG".
       */
      void remove (unsigned int id, const std::string& authName="EPSG");
      
      //! Removes all coordinate system representations from the manager.
      void clear();
      
      //! Returns the number of objects in the manager.
      size_t size() const;
      
      /*!
       \brief Returns an iterator mechanism over the coordinate system descriptions in the manager.
       
       The first iterator of the returned pair points to first coordinate system description.
       The second iterator of the returned pair points to the last plus one ccoordinate system description.
       \return a pair of iterators pointing to the first and last coordnate system representation in the manager.
       */
      std::pair<te::srs::SpatialReferenceSystemManager::iterator,te::srs::SpatialReferenceSystemManager::iterator> getIterators() const;
      
      /*!
       \brief Returns the unit of measure for a SRS with a given id.
       \return A pointer to the unit of measure or a null pointer if it the SRS could not be founded.
       */
      te::common::UnitOfMeasurePtr getUnit(unsigned int id, const std::string& authName="EPSG");
      
      /*!
       \brief Checks if a SRS with a given id refers to a geographic spatial reference system.
       \return True if the SRS with a given id refers to a geographic spatial reference system or false if not or not founded in the Manager.
       */
      bool isGeographic(unsigned int id, const std::string& authName="EPSG");
      
      /*!
       \brief Returns a SRID, not yet used, to identify an SRS created by an user. 
       \return a string represent a SRID, not yet used, to identify an SRS created by an user.
      */
      std::string getNewUserDefinedSRID();
      
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
      
    };
  }
}
#endif // __TERRALIB_SRS_INTERNAL_SPATIALREFERENCESYSTEMMANAGER_H
