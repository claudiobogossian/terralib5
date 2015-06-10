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
 \file terralib/metadata/serialization/xml/MD_Metadata.cpp
 
 \brief Support for MD_Metadata serialization.
 */

// TerraLib
#include "../../../common/Translator.h"
#include "../../../metadata/iso19115/CI_Citation.h"
#include "../../../metadata/iso19115/CI_ResponsibleParty.h"
#include "../../../metadata/iso19115/MD_Format.h"
#include "../../../metadata/iso19115/MD_Metadata.h"
#include "../../../metadata/iso19115/MD_Distribution.h"
#include "../../../srs/Config.h"
#include "../../../xml/AbstractWriter.h"
#include "../../../xml/Reader.h"
#include "MD_Metadata.h"

// STL
#include <cassert>

// Boost
#include <boost/foreach.hpp>
#include <boost/format.hpp>


void te::md::Save(const te::md::MD_Metadata* md, te::xml::AbstractWriter& writer)
{
  writer.writeStartDocument("UTF-8", "no");
  writer.writeStartElement("gmd:MD_Metadata");
  
  writer.writeAttribute("xmlns:gco", "http://www.isotc211.org/2005/gco");
  writer.writeAttribute("xmlns:gmd", "http://www.isotc211.org/2005/gmd");
  writer.writeAttribute("xmlns:gml", "http://www.opengis.net/gml");
  writer.writeAttribute("xmlns:gts", "http://www.isotc211.org/2005/gts");
  writer.writeAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
  
  // metadata file identifier - core optional element
  if (!md->getFileId().empty())
  {
    writer.writeStartElement("gmd:fileIdentifier");
    writer.writeStartElement("gco:CharacterString");
    writer.writeValue(md->getFileId());
    writer.writeEndElement("gco:CharacterString");
    writer.writeEndElement("gmd:fileIdentifier");
  }
  
  // metadata language - core conditional element, if not defined by encoding
  writer.writeStartElement("gmd:language");  
  if (!md->getLanguage().empty())
  {
    writer.writeStartElement("gco:CharacterString");
    writer.writeValue(md->getLanguage());
    writer.writeEndElement("gco:CharacterString");    
  }
  else 
    writer.writeAttribute("gco:nilReason", "unknown"); 
  writer.writeEndElement("gmd:language");
  
  // metadata character set - core conditional element
  // Here we have a problema: how to map codes to their name values?!
  writer.writeStartElement("gmd:characterSet");
  writer.writeStartElement("gmd:MD_CharacterSetCode");
  writer.writeAttribute("codeList","http://www.isotc211.org/2005/resources/codeList.xml#MD_CharacterSetCode");
  writer.writeAttribute("codeSpace",md->getCharset());
  //writer.writeValue(md->getCharset());
  //writer.writeEndElement("gmd:MD_CharacterSetCode");
  writer.writeEndElement("gmd:MD_CharacterSetCode");
  writer.writeEndElement("gmd:characterSet");  
  
  // these are not core elements, this is the default
  writer.writeStartElement("gmd:parentIdentifier");
  writer.writeAttribute("gco:nilReason","inapplicable");
  writer.writeEndElement("gmd:parentIdentifier");
  
	writer.writeStartElement("gmd:hierarchyLevel");
  writer.writeStartElement("gmd:MD_ScopeCode");
  writer.writeAttribute("codeList","http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#MD_ScopeCode");
  writer.writeAttribute("codeListValue","dataset");
  writer.writeAttribute("codeSpace","005");
  writer.writeValue("dataset");
  writer.writeEndElement("gmd:MD_ScopeCode");
  writer.writeEndElement("gmd:hierarchyLevel");
  
  writer.writeStartElement("gmd:hierarchyLevelName");
  writer.writeAttribute("gco:nilReason","inapplicable");
  writer.writeEndElement("gmd:hierarchyLevelName");
  
  // metadata point of contact is core and mandatory element
  writer.writeStartElement("gmd:contact");
  const boost::ptr_vector<te::md::CI_ResponsibleParty>& mycontacts = md->getContacts();
  if (!mycontacts.empty()) 
  {
    for (boost::ptr_vector<te::md::CI_ResponsibleParty>::const_iterator it = mycontacts.begin();
         it != mycontacts.end(); ++it)
      te::md::Save(*it,writer);
  }
  else 
    writer.writeAttribute("gco:nilReason", "unknown");
  writer.writeEndElement("gmd:contact");

  // metadata date stamp - core mandatory element
  writer.writeStartElement("gmd:dateStamp");
  if (!md->getDateStamp().is_not_a_date())
  {
    writer.writeStartElement("gco:Date");
    writer.writeValue(boost::gregorian::to_iso_extended_string(md->getDateStamp()));
    writer.writeEndElement("gco:Date");
  }
  else 
    writer.writeAttribute("gco:nilReason", "unknown");
  writer.writeEndElement("gmd:dateStamp");
  
  // metadata standard name - core optional element
  if (!md->getStandardName().empty())
  {
    writer.writeStartElement("gmd:metadataStandardName");
    writer.writeStartElement("gco:CharacterString");
    writer.writeValue(md->getStandardName());
    writer.writeEndElement("gco:CharacterString");
    writer.writeEndElement("gmd:metadataStandardName");
  }
  
  // metadata standard version - core optional element
  if (!md->getStandardName().empty())
  {
    writer.writeStartElement("gmd:metadataStandardVersion");
    writer.writeStartElement("gco:CharacterString");
    writer.writeValue(md->getStandardVersion());
    writer.writeEndElement("gco:CharacterString");
    writer.writeEndElement("gmd:metadataStandardVersion");
  }

  // because of the context, this is the link to the metadata file, not the dataset file. It is not core.
  if (!md->getMetadataURI().empty())
  {
    writer.writeStartElement("gmd:dataSetURI");
    writer.writeStartElement("gco:CharacterString");
    writer.writeValue(md->getMetadataURI());
    writer.writeEndElement("gco:CharacterString");
    writer.writeEndElement("gmd:dataSetURI");
  }
  
  // reference system - core optional SRS
  // for now... lets suppose we are dealing with EPSG codes only.
  if (md->getReferenceSystem() != TE_UNKNOWN_SRS)
  {
    writer.writeStartElement("gmd:referenceSystemInfo");
    writer.writeStartElement("gmd:MD_ReferenceSystem");
    writer.writeStartElement("gmd:referenceSystemIdentifier");
    writer.writeStartElement("gmd:RS_Identifier");
    std::string scode;
    scode= boost::str(boost::format("urn:ogc:def:crs:EPSG::%d") % md->getReferenceSystem());
    writer.writeStartElement("gmd:code");
    writer.writeValue(scode);
    writer.writeEndElement("gmd:code");
    writer.writeEndElement("gmd:RS_Identifier");
    writer.writeEndElement("gmd:referenceSystemIdentifier");
    writer.writeEndElement("gmd:MD_ReferenceSystem");
    writer.writeEndElement("gmd:referenceSystemInfo");
  }
  
  // this is core and mandatory element from at least one dataset.
  writer.writeStartElement("gmd:identificationInfo");
  const boost::ptr_vector<te::md::MD_Identification>& ids = md->getIdentifications();
  if (!ids.empty()) 
  {
    for (boost::ptr_vector<te::md::MD_Identification>::const_iterator itd = ids.begin();
         itd != ids.end(); ++itd)
      te::md::Save(*itd,writer);
  }
  else 
    writer.writeAttribute("gco:nilReason", "unknown");
  writer.writeEndElement("gmd:identificationInfo"); 
  
  const te::md::MD_Distribution* dist = md->getDistribution();
  if (dist)
  {
    writer.writeStartElement("gmd:distributionInfo");
    writer.writeStartElement("gmd:MD_Distribution");
    
    const boost::ptr_vector<te::md::MD_Format>& fmts = dist->getFormats();
    if (!fmts.empty())
    {
      writer.writeStartElement("gmd:distributionFormat");
      for (boost::ptr_vector<te::md::MD_Format>::const_iterator itf = fmts.begin(); itf != fmts.end(); ++itf)
      {
        writer.writeStartElement("gmd:MD_Format");
        
        // distribution format name - core optional element
        writer.writeStartElement("gmd:name");
        if (!itf->m_name.empty())
        {
          writer.writeStartElement("gco:CharacterString");
          writer.writeValue(itf->m_name);
          writer.writeEndElement("gco:CharacterString");
        }
        else
          writer.writeAttribute("gco:nilReason", "unknown");
        writer.writeEndElement("gmd:name");
        
        // distribution format version - core optional element
        writer.writeStartElement("gmd:version");
        if (!itf->m_version.empty())
        {
          writer.writeStartElement("gco:CharacterString");
          writer.writeValue(itf->m_version);
          writer.writeEndElement("gco:CharacterString");
        }
        else
          writer.writeAttribute("gco:nilReason", "inapplicable");
        writer.writeEndElement("gmd:version");
        
        writer.writeEndElement("gmd:MD_Format");
      }
      writer.writeEndElement("gmd:distributionFormat");
    }
    if (!dist->getURL().empty())
    {
      writer.writeStartElement("gmd:transferOptions");
      writer.writeStartElement("gmd:MD_DigitalTransferOptions");
      writer.writeStartElement("gmd:onLine");
      writer.writeStartElement("gmd:CI_OnlineResource");
      writer.writeStartElement("gmd:linkage");
      writer.writeStartElement("gmd:URL");
      writer.writeValue(dist->getURL());
      writer.writeEndElement("gmd:URL");
      writer.writeEndElement("gmd:linkage");
      writer.writeEndElement("gmd:CI_OnlineResource");
      writer.writeEndElement("gmd:onLine");
      writer.writeEndElement("gmd:MD_DigitalTransferOptions");
      writer.writeEndElement("gmd:transferOptions");
    }
    writer.writeEndElement("gmd:MD_Distribution");
    writer.writeEndElement("gmd:distributionInfo");
  }
  
  if (!md->getLineage().empty())
  {
    writer.writeStartElement("gmd:DQ_DataQuality"); 
    
    writer.writeStartElement("gmd:scope");
    writer.writeStartElement("gmd:DQ_Scope");
    writer.writeStartElement("gmd:level");
    writer.writeStartElement("gmd:MD_ScopeCode");
    writer.writeAttribute("codeList","http://www.isotc211.org/2005/resources/Codelist/gmxCodelists.xml#MD_ScopeCode");
    writer.writeAttribute("codeListValue","dataset");
    writer.writeAttribute("codeSpace","005");
    writer.writeValue("dataset");
    writer.writeEndElement("gmd:MD_ScopeCode");
    writer.writeEndElement("gmd:level");
    writer.writeEndElement("gmd:DQ_Scope");
    writer.writeEndElement("gmd:scope");
    
    writer.writeStartElement("gmd:lineage");
    writer.writeStartElement("gmd:LI_Lineage");
    writer.writeStartElement("gmd:statement");
    writer.writeStartElement("gco:CharacterString");
    writer.writeValue(md->getLineage());
    writer.writeEndElement("gco:CharacterString");
    writer.writeEndElement("gmd:statement");
    writer.writeEndElement("gmd:LI_Lineage");
    writer.writeEndElement("gmd:lineage");
    
    writer.writeEndElement("gmd:DQ_DataQuality");
  }
  
  writer.writeEndElement("gmd:MD_Metadata");
}

void te::md::Save(const te::md::CI_ResponsibleParty& ctc, te::xml::AbstractWriter& writer)
{
  writer.writeStartElement("gmd:CI_ResponsibleParty");

  writer.writeStartElement("gmd:individualName");
  if (!ctc.getIndividualName().empty())
  {
    writer.writeStartElement("gco:CharacterString");
    writer.writeValue(ctc.getIndividualName());
    writer.writeEndElement("gco:CharacterString");    
  }
  else 
    writer.writeAttribute("gco:nilReason", "inapplicable");
  writer.writeEndElement("gmd:individualName");

  writer.writeStartElement("gmd:organisationName");
  if (!ctc.getOrganizationName().empty())
  {
    writer.writeStartElement("gco:CharacterString");
    writer.writeValue(ctc.getOrganizationName());
    writer.writeEndElement("gco:CharacterString");
  }
  else 
    writer.writeAttribute("gco:nilReason", "inapplicable");
  writer.writeEndElement("gmd:organisationName");
  
  writer.writeStartElement("gmd:positionName");
  if (!ctc.getPositionName().empty())
  {
    writer.writeStartElement("gco:CharacterString");
    writer.writeValue(ctc.getPositionName());
    writer.writeEndElement("gco:CharacterString");
  }
  else 
    writer.writeAttribute("gco:nilReason", "inapplicable");
  writer.writeEndElement("gmd:positionName");
  
  writer.writeStartElement("gmd:role");
  writer.writeStartElement("gmd:CI_RoleCode");
  writer.writeAttribute("codeList","http://www.isotc211.org/2005/resources/codeList.xml#CI_RoleCode");
  writer.writeAttribute("codeListValue",ctc.getRoleCode());
  writer.writeValue(ctc.getRoleCode());
  writer.writeEndElement("gmd:CI_RoleCode");
  writer.writeEndElement("gmd:role"); 
  
  writer.writeEndElement("gmd:CI_ResponsibleParty");
}

void te::md::Save(const te::md::MD_Identification& id, te::xml::AbstractWriter& writer)
{  
  // assuming that it is a data identification
  writer.writeStartElement("gmd:MD_DataIdentification");
  
  // citation has some core and mandatory elements
  te::md::Save(id.getCitation(),writer);
  
  // abstract is a core and mandatory element
  writer.writeStartElement("gmd:abstract");
  writer.writeStartElement("gco:CharacterString");
  writer.writeValue(id.getAbstract());
  writer.writeEndElement("gco:CharacterString");
  writer.writeEndElement("gmd:abstract");
  
  // status is not a core element, but it is mandatory in MGB
  writer.writeStartElement("gmd:status");
  writer.writeStartElement("gmd:MD_ProgressCode");
  writer.writeAttribute("codeList", "http://www.isotc211.org/2005/resources/codeList.xml#MD_ProgressCode");
  writer.writeAttribute("codeListValue", id.getStatus());
  //writer.writeValue(id.getStatus());
  writer.writeEndElement("gmd:MD_ProgressCode");
  writer.writeEndElement("gmd:status");
  
  // dataset responsible party - core optional element
  const boost::ptr_vector<te::md::CI_ResponsibleParty>& ctcs = id.getPointsOfContact();
  if (!ctcs.empty()) 
  {
    writer.writeStartElement("gmd:pointOfContact");
    for (boost::ptr_vector<te::md::CI_ResponsibleParty>::const_iterator it = ctcs.begin();
         it != ctcs.end(); ++it)
      te::md::Save(*it,writer);
    writer.writeEndElement("gmd:pointOfContact");
  }
  
  const te::md::MD_DataIdentification* did = static_cast<const te::md::MD_DataIdentification*>(&id);
  
  // spatial representation type - core optional element, but it has a default
  writer.writeStartElement("gmd:spatialRepresentationType");
  writer.writeStartElement("gmd:MD_SpatialRepresentationTypeCode");
  writer.writeAttribute("codeList","http://www.isotc211.org/2005/resources/codeList.xml#MD_SpatialRepresentationTypeCode");
  writer.writeAttribute("codeListValue", did->getSpatialRepTypeCode());
  writer.writeValue(did->getSpatialRepTypeCode());
  writer.writeEndElement("gmd:MD_SpatialRepresentationTypeCode");
  writer.writeEndElement("gmd:spatialRepresentationType");
  
  
  // dataset language - core mandatory element
  writer.writeStartElement("gmd:language");
  const std::vector<std::string>& mlang = did->getLanguages();
  if (!mlang.empty())
  {
    BOOST_FOREACH(const std::string& lang, mlang)
    {
      writer.writeStartElement("gmd:LanguageCode");
      writer.writeAttribute("codeList","http://www.isotc211.org/2005/resources/codeList.xml#LanguageCode");
      writer.writeAttribute("codeListValue",lang);
      writer.writeValue(lang);
      writer.writeEndElement("gmd:LanguageCode");
    }
  }
  else 
      writer.writeAttribute("gco:nilReason", "unknown");
  writer.writeEndElement("gmd:language");
  
  // dataset character set - core conditional element
  writer.writeStartElement("gmd:characterSet");
  const std::vector<te::md::MD_CharacterSetCode>& mchars = did->getCharsetCodes();
  if (!mchars.empty())
  {
    BOOST_FOREACH(const te::md::MD_CharacterSetCode& cs, mchars)
    {
      writer.writeStartElement("gmd:MD_CharacterSetCode");
      writer.writeAttribute("codeList","http://www.isotc211.org/2005/resources/codeList.xml#MD_CharacterSetCode");
      writer.writeAttribute("codeListValue",cs);
      writer.writeValue(cs);
      writer.writeEndElement("gmd:MD_CharacterSetCode");
    }
  }
  else 
    writer.writeAttribute("gco:nilReason", "unknown");
  writer.writeEndElement("gmd:characterSet");
  
  // dataset topic category - core mandatory element
  writer.writeStartElement("gmd:topicCategory");
  const std::vector<te::md::MD_TopicCategoryCode>& mtop = did->getTopicCategories();
  if (!mtop.empty())
  {
    BOOST_FOREACH(const te::md::MD_TopicCategoryCode& t, mtop)
    {
      writer.writeStartElement("gmd:MD_TopicCategoryCode");
      writer.writeAttribute("codeList","http://www.isotc211.org/2005/resources/codeList.xml#MD_TopicCategoryCode");
      writer.writeAttribute("codeListValue",t);
      writer.writeValue(t);
      writer.writeEndElement("gmd:MD_TopicCategoryCode");
    }
  }
  else 
    writer.writeAttribute("gco:nilReason", "unknown");
  writer.writeEndElement("gmd:topicCategory");
  
  // geographic location of the dataset - core conditional element
  writer.writeStartElement("gmd:extent");
  writer.writeStartElement("gmd:EX_Extent");
  
  writer.writeStartElement("gmd:description");
  writer.writeAttribute("gco:nilReason","inapplicable");
  writer.writeEndElement("gmd:description");
  
  writer.writeStartElement("gmd:geographicElement");  
  const te::gm::Envelope& ext = did->getExtent();
  if (ext.isValid())
  {
    writer.writeStartElement("gmd:EX_GeographicBoundingBox");
    
    writer.writeStartElement("gmd:westBoundLongitude");
    writer.writeStartElement("gco:Decimal");
    writer.writeValue(ext.getLowerLeftX());
    writer.writeEndElement("gco:Decimal");
    writer.writeEndElement("gmd:westBoundLongitude");
    
    writer.writeStartElement("gmd:eastBoundLongitude");
    writer.writeStartElement("gco:Decimal");
    writer.writeValue(ext.getUpperRightX());
    writer.writeEndElement("gco:Decimal");
    writer.writeEndElement("gmd:eastBoundLongitude");
    
    writer.writeStartElement("gmd:southBoundLatitude");
    writer.writeStartElement("gco:Decimal");
    writer.writeValue(ext.getLowerLeftY());
    writer.writeEndElement("gco:Decimal");    
    writer.writeEndElement("gmd:southBoundLatitude");
    
    writer.writeStartElement("gmd:northBoundLatitude");
    writer.writeStartElement("gco:Decimal");
    writer.writeValue(ext.getUpperRightY());
    writer.writeEndElement("gco:Decimal");
    writer.writeEndElement("gmd:northBoundLatitude");

    writer.writeEndElement("gmd:EX_GeographicBoundingBox");
    
  }
  else 
    writer.writeAttribute("gco:nilReason", "unknown");
  writer.writeEndElement("gmd:geographicElement");
  
  // aditional extent information for the dataset - core optional element
  writer.writeStartElement("gmd:temporalElement");
  writer.writeAttribute("gco:nilReason","inapplicable");
  writer.writeEndElement("gmd:temporalElement");
  
  writer.writeStartElement("gmd:verticalElement");
  writer.writeAttribute("gco:nilReason","inapplicable");
  writer.writeEndElement("gmd:verticalElement");
  
  writer.writeEndElement("gmd:EX_Extent");
  writer.writeEndElement("gmd:extent");
  
  // spatial resolution of the dataset - core optional element
  if (did->getScale() > 0)
  {
    writer.writeStartElement("gmd:spatialResolution");
    writer.writeStartElement("gmd:MD_Resolution");
    writer.writeStartElement("gmd:equivalenteScale");
    writer.writeStartElement("gmd:MD_RepresentativeFraction");
    writer.writeStartElement("gmd:denominator");
    writer.writeStartElement("gco:Integer");
    did->getScale();
    writer.writeEndElement("gco:Integer");
    writer.writeEndElement("gmd:denominator");
    writer.writeEndElement("gmd:MD_RepresentativeFraction");
    writer.writeEndElement("gmd:equivalenteScale");
    writer.writeEndElement("gmd:MD_Resolution");
    writer.writeEndElement("gmd:spatialResolution");
  }
  
  writer.writeEndElement("gmd:MD_DataIdentification");
}

void te::md::Save(const te::md::CI_Citation* cite, te::xml::AbstractWriter& writer)
{
  writer.writeStartElement("gmd:citation");
  writer.writeStartElement("gmd:CI_Citation");
  
  // dataset title - core mandatory element
  writer.writeStartElement("gmd:title");
  writer.writeStartElement("gco:CharacterString");
  writer.writeValue(cite->getTitle());
  writer.writeEndElement("gco:CharacterString");
  writer.writeEndElement("gmd:title");
  
  // dataset reference date - core mandatory  element
  writer.writeStartElement("gmd:date");
  writer.writeStartElement("gmd:CI_Date");
  writer.writeStartElement("gmd:date");
  writer.writeStartElement("gco:Date");
  writer.writeValue(boost::gregorian::to_iso_extended_string(cite->getDate()));
  writer.writeEndElement("gco:Date");
  writer.writeEndElement("gmd:date");
  writer.writeStartElement("gmd:dateType");
  writer.writeStartElement("gmd:CI_DateTypeCode");
  writer.writeAttribute("codeList","http://www.isotc211.org/2005/resources/codeList.xml#CI_DateTypeCode");
  writer.writeAttribute("codeListValue",cite->getDateType());
  writer.writeEndElement("gmd:CI_DateTypeCode");
  writer.writeEndElement("gmd:dateType");
  writer.writeEndElement("gmd:CI_Date");
  writer.writeEndElement("gmd:date");
  
  writer.writeEndElement("gmd:CI_Citation");
  writer.writeEndElement("gmd:citation");
}

