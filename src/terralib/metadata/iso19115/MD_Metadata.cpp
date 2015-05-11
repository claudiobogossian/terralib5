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
 \file terralib/metadata//iso19115/MD_Metadata.cpp
 */

// TerraLib
#include "../../srs/Config.h"
#include "MD_Distribution.h"
#include "MD_Metadata.h"

// STL
#include <cassert>


te::md::MD_Metadata::MD_Metadata(te::md::CI_ResponsibleParty* contact) :
  m_characterSet(te::md::MD_utf8),
  m_referenceSystemInfo(TE_UNKNOWN_SRS),
  m_distribution(0)
{
  if (contact)
    m_contact.push_back(contact);
}

te::md::MD_Metadata::~MD_Metadata()
{
  if (m_distribution)
    delete m_distribution;
}
     
void 
te::md::MD_Metadata::setFileId(const std::string& fileid)
{
  assert(!fileid.empty());
  
  m_fileIdentifier = fileid;
}


const std::string& 
te::md::MD_Metadata::getFileId() const
{
  return m_fileIdentifier;
}

void 
te::md::MD_Metadata::setMetadataURI(const std::string& mdid)
{
  assert(!mdid.empty());
  
  m_metadataURI = mdid;
}

const std::string& 
te::md::MD_Metadata::getMetadataURI() const
{
  return m_metadataURI;
}

void 
te::md::MD_Metadata::setLanguage(const std::string& lang)
{
  assert(!lang.empty());
  
  m_language = lang;
}

const std::string& 
te::md::MD_Metadata::getLanguage()const
{
  return m_language;
}

void 
te::md::MD_Metadata::setCharset (te::md::MD_CharacterSetCode chaset)
{
  m_characterSet = chaset;
}

te::md::MD_CharacterSetCode
te::md::MD_Metadata::getCharset() const
{
  return m_characterSet;
}

void 
te::md::MD_Metadata::setStandardInfo(const std::string& sname, const std::string& sversion)
{
  assert(!sname.empty());
  assert(!sversion.empty());
  
  m_standardName = sname;
  m_standardVersion = sversion;
}

const std::string&  
te::md::MD_Metadata::getStandardName() const
{
  return m_standardName;
}

const std::string&  
te::md::MD_Metadata::getStandardVersion() const
{
  return m_standardVersion;
}

void
te::md::MD_Metadata::addIdentification(te::md::MD_Identification* mdid)
{
  assert(mdid);
  m_identificationInfo.push_back(mdid);
}

const boost::ptr_vector<te::md::MD_Identification>&
te::md::MD_Metadata:: getIdentifications() const
{
  return m_identificationInfo;
}

void 
te::md::MD_Metadata::addContact(te::md::CI_ResponsibleParty* contact)
{
  assert(contact);
  m_contact.push_back(contact);
}

const boost::ptr_vector<te::md::CI_ResponsibleParty>& 
te::md::MD_Metadata:: getContacts() const
{
  return m_contact;
}

void 
te::md::MD_Metadata::setDateStamp(const boost::gregorian::date& date)
{
  m_dateStamp = date;
}

const boost::gregorian::date& 
te::md::MD_Metadata::getDateStamp() const
{
  return m_dateStamp;
}

void 
te::md::MD_Metadata::setReferenceSystem(int srid)
{
  m_referenceSystemInfo = srid;
}

int
te::md::MD_Metadata::getReferenceSystem() const
{
  return m_referenceSystemInfo;
}

void
te::md::MD_Metadata::setDistribution(te::md::MD_Distribution* d)
{
  assert(d);
  
  if (m_distribution)
    delete m_distribution;
  
  m_distribution = d;
}

const te::md::MD_Distribution* 
te::md::MD_Metadata::getDistribution() const
{
  return m_distribution;
}

void 
te::md::MD_Metadata::setLineage(const std::string& stm)
{
  assert(!stm.empty());
  
  m_lineage = stm;
}

const std::string&  
te::md::MD_Metadata::getLineage() const
{
  return m_lineage;
}

