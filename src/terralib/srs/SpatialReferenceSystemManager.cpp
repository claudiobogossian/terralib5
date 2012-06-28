/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.
 
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
 \file terralib/srs/SpatialReferenceSystemManager.cpp
 
 \brief A singleton to manage Coordinate Systems representations.  
*/

// TerraLib
#include "../common/Translator.h"
#include "Exception.h"
#include "SpatialReferenceSystem.h"
#include "SpatialReferenceSystemManager.h"
#include "WKTReader.h"

// STL
#include <algorithm>
#include <cassert>

te::srs::SpatialReferenceSystemManager::SpatialReferenceSystemManager()
{
}

te::srs::SpatialReferenceSystemManager::~SpatialReferenceSystemManager()
{
  clear();
}

void te::srs::SpatialReferenceSystemManager::add(const std::string& name, const std::string& p4Txt, const std::string& wkt, unsigned int id, const std::string& authName)
{
  assert(id > 0);
  assert(!p4Txt.empty());
  
  std::pair<std::string,unsigned int> mkey(authName,id);
  
  std::vector< std::pair<std::string,unsigned int> >::iterator it = std::find(m_authIdV.begin(),m_authIdV.end(),mkey);
  if (it != m_authIdV.end())
    throw te::srs::Exception(TR_SRS("The CS identification already exists in the manager."));
  
  m_authIdV.push_back(mkey);
  m_nameV.push_back(name); 
  m_p4txtV.push_back(wkt);
  m_wktV.push_back(p4Txt);
}

bool te::srs::SpatialReferenceSystemManager::recognizes(unsigned int id, const std::string& authName) const
{
  std::pair<std::string,unsigned int> mkey(authName,id);
  std::vector< std::pair<std::string,unsigned int> >::const_iterator it = std::find(m_authIdV.begin(),m_authIdV.end(),mkey);
  return (it != m_authIdV.end());
}

te::srs::SpatialReferenceSystem* te::srs::SpatialReferenceSystemManager::getSpatialReferenceSystem(unsigned int id, const std::string& authName) const
{
  assert(id > 0);
  
  std::pair<std::string,unsigned int> mkey(authName,id);
  std::vector< std::pair<std::string,unsigned int> >::const_iterator it = std::find(m_authIdV.begin(),m_authIdV.end(),mkey);
  if (it == m_authIdV.end())
    return 0;
  
  size_t idx = it-m_authIdV.begin();
  try
  {
    return te::srs::WKTReader::read(m_wktV[idx].c_str());
  }
  catch(...)
  {
    throw te::srs::Exception(TR_SRS("Error parsing the registered CS WKT."));
  }
  return 0;
}


std::string te::srs::SpatialReferenceSystemManager::getName(unsigned int id, const std::string& authName) const
{
  assert(id > 0);

  std::pair<std::string,unsigned int> mkey(authName,id);
  std::vector< std::pair<std::string,unsigned int> >::const_iterator it = std::find(m_authIdV.begin(),m_authIdV.end(),mkey);
  if (it == m_authIdV.end())
    return "";
                                       
  size_t idx = it-m_authIdV.begin();
  std::cout << idx << std::endl;
  return m_nameV[idx];
}

std::string te::srs::SpatialReferenceSystemManager::getWkt (unsigned int id, const std::string& authName) const
{
  assert(id > 0);
  
  std::pair<std::string,unsigned int> mkey(authName,id);
  std::vector< std::pair<std::string,unsigned int> >::const_iterator it = std::find(m_authIdV.begin(),m_authIdV.end(),mkey);
  if (it == m_authIdV.end())
    return "";
  
  size_t idx = it-m_authIdV.begin();
  return m_wktV[idx];
}

std::string te::srs::SpatialReferenceSystemManager::getP4Txt(unsigned int id, const std::string& authName) const
{
  assert(id > 0);
  
  std::pair<std::string,unsigned int> mkey(authName,id);
  std::vector< std::pair<std::string,unsigned int> >::const_iterator it = std::find(m_authIdV.begin(),m_authIdV.end(),mkey);
  if (it == m_authIdV.end())
    return "";
  
  size_t idx = it-m_authIdV.begin();
  return m_p4txtV[idx];
}

std::pair<std::string,unsigned int> te::srs::SpatialReferenceSystemManager::getIdFromName(const std::string& name) const
{
  assert(!name.empty());
  
  std::vector<std::string>::const_iterator itx = std::find(m_nameV.begin(),m_nameV.end(),name);
  if (itx == m_nameV.end())
    throw te::srs::Exception(TR_SRS("CS name not recognized."));    
  
  size_t idx = itx - m_nameV.begin();
  return m_authIdV[idx];
}


std::pair<std::string,unsigned int> te::srs::SpatialReferenceSystemManager::getIdFromP4Txt(const std::string& p4Txt) const
{
  assert(!p4Txt.empty());
  
  std::vector<std::string>::const_iterator it = std::find(m_p4txtV.begin(),m_p4txtV.end(),p4Txt);
  if (it == m_p4txtV.end())
    throw te::srs::Exception(TR_SRS("CS PROJ4 text not recognized."));    
  
  size_t idx = it - m_p4txtV.begin();
  return m_authIdV[idx]; 
}

std::pair<std::string,unsigned int> te::srs::SpatialReferenceSystemManager::getIdFromWkt(const std::string& wkt) const
{
  assert(!wkt.empty());
  
  std::vector<std::string>::const_iterator itx = std::find(m_wktV.begin(),m_wktV.end(),wkt);
  if (itx == m_wktV.end())
    throw te::srs::Exception(TR_SRS("CS WKT not recognized."));    
  
  size_t idx = itx - m_wktV.begin();
  return m_authIdV[idx];
}

void te::srs::SpatialReferenceSystemManager::remove(unsigned int id, const std::string& authName)
{
  assert(id > 0);
  
  std::pair<std::string,unsigned int> mkey(authName,id);
  std::vector< std::pair<std::string,unsigned int> >::const_iterator it = std::find(m_authIdV.begin(),m_authIdV.end(),mkey);
  if (it != m_authIdV.end())
  {
    size_t idx = it-m_authIdV.begin();
    m_nameV.erase(m_nameV.begin()+idx);
    m_p4txtV.erase(m_p4txtV.begin()+idx);
    m_wktV.erase(m_wktV.begin()+idx);
  }
}

void te::srs::SpatialReferenceSystemManager::clear()
{
  m_authIdV.clear();
  m_nameV.clear();
  m_p4txtV.clear();
  m_wktV.clear();
}


std::pair<std::vector<std::pair<std::string,unsigned int> >::const_iterator,
          std::vector<std::pair<std::string,unsigned int> >::const_iterator> te::srs::SpatialReferenceSystemManager::getIteratorIds() const
{
  return std::pair<std::vector<std::pair<std::string,unsigned int> >::const_iterator,
                   std::vector<std::pair<std::string,unsigned int> >::const_iterator>(m_authIdV.begin(), m_authIdV.end());
}

std::pair<std::vector<std::string>::const_iterator,
          std::vector<std::string>::const_iterator> te::srs::SpatialReferenceSystemManager::getIteratorNames() const
{
  return std::pair<std::vector<std::string>::const_iterator,
         std::vector<std::string>::const_iterator>(m_nameV.begin(),m_nameV.end());
}

