/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.
 
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
 \file terralib/metadata/iso19115/MD_DataIdentification.cpp
 */

// TerraLib
#include "MD_DataIdentification.h"

      
te::md::MD_DataIdentification::MD_DataIdentification(te::md::CI_Citation* cite, const std::string& abstract,MD_ProgressCode status, const std::string& lang):
  MD_Identification(cite,abstract, status),
  m_dataIsCovered(true),
  m_spatialRepresentationType(te::md::MD_vector)
{
  m_language.push_back(lang);
}

te::md::MD_DataIdentification::~MD_DataIdentification() 
{
}

void 
te::md::MD_DataIdentification::addLanguage(const std::string& lang)
{
  m_language.push_back(lang);
}

const std::vector<std::string>& 
te::md::MD_DataIdentification::getLanguages() const
{
  return m_language;
}

void 
te::md::MD_DataIdentification::addCharacterSet(te::md::MD_CharacterSetCode code)
{
  m_charSet.push_back(code);
}

const std::vector<te::md::MD_CharacterSetCode>& 
te::md::MD_DataIdentification::getCharsetCodes() const
{
  return m_charSet;
}

void 
te::md::MD_DataIdentification::addTopicCategory(te::md::MD_TopicCategoryCode code)
{
  m_topicCategory.push_back(code);
}

const std::vector<te::md::MD_TopicCategoryCode>& 
te::md::MD_DataIdentification::getTopicCategories() const
{
  return m_topicCategory;
}

void 
te::md::MD_DataIdentification::addGeographicBBoxExt(const double& llx, const double& lly,
                                                    const double& urx, const double& ury, 
                                                    bool dataIsCovered)
{
  m_extent.init(llx, lly, urx, ury);
  m_dataIsCovered = dataIsCovered;
}

const 
te::gm::Envelope& te::md::MD_DataIdentification::getExtent() const
{
  return m_extent;
}


void
te::md::MD_DataIdentification::setSpatialRepType(te::md::MD_SpatialRepresentationTypeCode code)
{
  m_spatialRepresentationType = code;
}

te::md::MD_SpatialRepresentationTypeCode
te::md::MD_DataIdentification::getSpatialRepType() const
{
  return m_spatialRepresentationType;
}

