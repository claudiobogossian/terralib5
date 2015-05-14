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
 \file terralib/metadata/iso19115/MD_Metadata.h
 
 \brief 
 */

#ifndef __TERRALIB_METADATA_INTERNAL_ISO19115_MD_METADATA_H
#define __TERRALIB_METADATA_INTERNAL_ISO19115_MD_METADATA_H

// TerraLib
#include "../Config.h"
#include "CodeList.h"
#include "CI_ResponsibleParty.h"
#include "MD_DataIdentification.h"

// STL
#include <string>

// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/noncopyable.hpp>

namespace te
{
  namespace md
  {
    class MD_Distribution;
    
    /*!
     \class MD_Metadata
     
     \brief Root entity which defines metadata for a resource or resources.
     
     \todo Include the members MD_ApplicationSchemaInformation, MD_PortrayalCatalogueReference, MD_ContentInformation MD_MetadataExtensionInformation (NOT IMPLEMENTED YET)
     
     \sa 
     */
    class TEMDEXPORT MD_Metadata : public boost::noncopyable
    {
      public:
      
      /*! 
       \brief Constructor.
       \param contact Pointer to a metadata responsible part or point of contact. Class takes pointer ownership.
       */
      MD_Metadata(CI_ResponsibleParty* contact = 0);
      
      //! Destructor.
      ~MD_Metadata();
      
      /*!
       \brief Sets a unique phrase or string which uniquely identifies the metadata file.
       \param fileid The file identifier. Do not pass empty string.
       This should identify the metadata. 
       Best practice: this should be an uuid.
       */
      void setFileId(const std::string& fileid);
      
      //! Returns the unique phrase or string which uniquely identifies the metadata file.
      const std::string& getFileId() const;
      
      /*!
       \brief Sets URI to the metadata .
       \param mdURI The URI to the metada. Do not pass empty string.
       This is NOT the place to link to the dataset. This should link to the metadata.
       */
      void setMetadataURI(const std::string& mdURI);
      
      //! Returns URI to the metadata 
      const std::string& getMetadataURI() const;
      
      /*!
       \brief Sets the language of the metadata.
       \param lang Language of the metadata. Do not pass empty string.
       The language information should be composed of an ISO639-2/T three-letter language code and an ISO3166-1 three-letter country code.
       */
      void setLanguage(const std::string& lang);
      
      //! Returns metadata language.
      const std::string& getLanguage() const;
       
      //! Sets the charset code used in the metadata
      void setCharset(MD_CharacterSetCode chaset);
      
      //! Returns metadata charset.
      te::md::MD_CharacterSetCode getCharset() const;  
      
      /*!
       \brief Sets the name of the metadata standard/profile used.
       \param sname The name of the metadata standard/profile used. Do not pass null.
      \param sname The version of the metadata standard/profile used. Do not pass empty string.
       */
      void setStandardInfo(const std::string& sname, const std::string& sversion);
      
      //! Returns the name of the metadata standard/profile used.
      const std::string& getStandardName() const;
      
      //! Returns the version of the metadata standard/profile used.
      const std::string& getStandardVersion() const;
      
      /*!
       \brief Adds a dataset identification to the metadata.
       \param mdid Pointer to resource identification. Class takes pointer ownership. Do not pass null.
       */
      void addIdentification(MD_Identification* mdid);
      
      //! Gets the resources identification associated to the metadata.
      const boost::ptr_vector<MD_Identification>& getIdentifications() const;
      
      /*!
       \brief Adds a contact information to the metadata.
       \param mdid Pointer to resource identification. Class takes pointer ownership. Do not pass null.
       */
      void addContact(CI_ResponsibleParty* contact);
      
      //! Gets the metadata contacts.
      const boost::ptr_vector<CI_ResponsibleParty>& getContacts() const;
      
      //! Sets the referency system. 
      void setReferenceSystem(int srid);
      
      //! Returns the referency system.
      int getReferenceSystem() const;
      
      //! Set metadata date stamp.
      void setDateStamp(const boost::gregorian::date& date);
      
      //! Get metadata date stamp.
      const boost::gregorian::date& getDateStamp() const;
      
      /*!
       \brief Sets a distribution information to the metadata.
       \param d Pointer to a distribution info. Class takes pointer ownership. Do not pass null.
       */      
      void setDistribution(MD_Distribution* d);
      
      //! Gets the distribution information.
      const MD_Distribution* getDistribution() const;
      
      /*! 
       \brief Sets the lineage statement.
       General explanation of the data producer’s knowledge of the dataset lineage. This is related to 
       data quality.
       \param stm The lineage statement. Do not pass null.
       */
      void setLineage(const std::string& stm);
      
      //! Gets the lineage statemente.
      const std::string& getLineage() const;
      
    private:
      
      std::string m_fileIdentifier;  
      
      std::string m_metadataURI;
      
      std::string m_language;
      
      MD_CharacterSetCode m_characterSet; 
      
      std::string m_standardName;
      std::string m_standardVersion;     
      
      boost::ptr_vector<CI_ResponsibleParty> m_contact;  
      
      boost::gregorian::date m_dateStamp;
      
      boost::ptr_vector<MD_Identification> m_identificationInfo;
      
      int m_referenceSystemInfo;
      
      MD_Distribution* m_distribution;
      
      std::string m_lineage;
      
    };
  } // end namespace md
}   // end namespace te

#endif  // __TERRALIB_METADATA_INTERNAL_ISO19115_MD_METADATA_H