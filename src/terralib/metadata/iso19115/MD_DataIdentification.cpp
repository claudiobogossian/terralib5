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
 \file terralib/metadata/iso19115/MD_DataIdentification.cpp
 */

// TerraLib
#include "MD_DataIdentification.h"

// Boost
#include <boost/foreach.hpp>

      
te::md::MD_DataIdentification::MD_DataIdentification(te::md::CI_Citation* cite, const std::string& abstract,MD_ProgressCode status, const std::string& lang):
  MD_Identification(cite,abstract, status),
  m_dataIsCovered(true),
  m_spatialRepresentationType(te::md::MD_vector),
  m_scale(0)
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

void
te::md::MD_DataIdentification::addGeographicBBoxExt(const te::gm::Envelope* bb, bool dataIsCovered)
{
  assert(bb);
  m_extent.init(bb->getLowerLeftX(), bb->getLowerLeftY(), bb->getUpperRightX(), bb->getUpperRightY());
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
te::md::MD_DataIdentification::getSpatialRepTypeCode() const
{
  return m_spatialRepresentationType;
}


void 
te::md::MD_DataIdentification::setScale(long d)
{
  assert(d>0);
  
  m_scale = d;
}

long 
te::md::MD_DataIdentification::getScale() const
{
  return m_scale;
}

te::md::MD_Identification* te::md::MD_DataIdentification::clone() const
{
  std::string l("");
  if (!m_language.empty())
    l = m_language[0];
  
  te::md::MD_DataIdentification* mid = new MD_DataIdentification(m_citation, m_abstract, m_status,l);

  BOOST_FOREACH(const te::md::MD_CharacterSetCode& cs, m_charSet)
  {
    mid->addCharacterSet(cs);
  }
  BOOST_FOREACH(const te::md::MD_TopicCategoryCode& t, m_topicCategory)
  {
    mid->addTopicCategory(t);
  }  
  mid->addGeographicBBoxExt(m_extent.getLowerLeftX(),m_extent.getLowerLeftY(),
                            m_extent.getUpperRightX(), m_extent.getUpperRightY(),
                            m_dataIsCovered);
  mid->setSpatialRepType(m_spatialRepresentationType);  
  
  return mid;
}


