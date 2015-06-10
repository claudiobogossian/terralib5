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
 \file terralib/metadata/iso19115/MD_Identification.cpp
 */

// TerraLib
#include "CI_Citation.h"
#include "MD_Identification.h"

// STL
#include <cassert>

te::md::MD_Identification::MD_Identification(te::md::CI_Citation* citation, const std::string abstract, MD_ProgressCode status) :
  m_citation(0),
  m_abstract(abstract),
  m_status(status)
{
  assert(citation);
  
  m_citation = citation;
}

te::md::MD_Identification::~MD_Identification()
{
  if (m_citation)
    delete m_citation;
}

const te::md::CI_Citation* 
te::md::MD_Identification::getCitation() const
{
  return m_citation;
}


const std::string&
te::md::MD_Identification::getAbstract() const
{
  return m_abstract;
}

te::md::MD_ProgressCode 
te::md::MD_Identification::getStatus() const
{
  return m_status;
}

void 
te::md::MD_Identification::addPointOfContact(CI_ResponsibleParty* p)
{
  assert(p);
  m_poc.push_back(p);
}

//! Returns the resource points of contact.
const boost::ptr_vector<te::md::CI_ResponsibleParty>& 
te::md::MD_Identification:: getPointsOfContact() const
{
  return m_poc;
}

te::md::MD_Identification* te::md::MD_Identification::clone() const
{
  te::md::CI_Citation* ci = 0;
  if (m_citation)
    ci = m_citation->clone();
  
  te::md::MD_Identification* id = new MD_Identification(ci, m_abstract, m_status);
  
  boost::ptr_vector<te::md::CI_ResponsibleParty>::const_iterator it = m_poc.begin();
  while (it != m_poc.end())
  {
    CI_ResponsibleParty* rp = it->clone();
    id->addPointOfContact(rp);
  }
  return id;
}
