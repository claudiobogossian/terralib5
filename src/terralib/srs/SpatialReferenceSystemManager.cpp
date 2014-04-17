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
#include "../common/PlatformUtils.h"
#include "../common/Translator.h"
#include "../common/UnitsOfMeasureManager.h"
#include "Exception.h"
#include "SpatialReferenceSystemManager.h"
#include "WKTReader.h"

// STL
#include <algorithm>
#include <cassert>

// Boost
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

void LoadSpatialReferenceSystemManager(const std::string fileName, te::srs::SpatialReferenceSystemManager* mger) 
{
  std::ifstream f;

  f.open(fileName.c_str());

  if (!f.is_open())
    return;
    
  boost::property_tree::ptree pt;
  boost::property_tree::json_parser::read_json(f,pt);
  BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("SRSs"))
  {
    if (v.second.get<unsigned int>("srid") > 100000)
      mger->add(v.second.get<std::string>("name"), v.second.get<std::string>("pj4txt"),
          v.second.get<std::string>("wkt"), v.second.get<unsigned int>("srid"), "USER");
    else
      mger->add(v.second.get<std::string>("name"), v.second.get<std::string>("pj4txt"),
          v.second.get<std::string>("wkt"), v.second.get<unsigned int>("srid"));
  }
  f.close();

  return;
}

te::srs::SpatialReferenceSystemManager::srs_desc::srs_desc(const std::string& name, unsigned int auth_id, const std::string& auth_name, const std::string& p4txt, const std::string& wkt):
  m_name(name),
  m_auth_id(auth_id),
  m_auth_name(auth_name),
  m_p4txt(p4txt),
  m_wkt(wkt)
{}

std::string 
te::srs::SpatialReferenceSystemManager::srs_desc::srid() const
{
  std::string ssrid = m_auth_name;
  ssrid += ":";
  ssrid += boost::lexical_cast<std::string>(m_auth_id);
  return ssrid;
}

te::srs::SpatialReferenceSystemManager::SpatialReferenceSystemManager()
{
}

te::srs::SpatialReferenceSystemManager::~SpatialReferenceSystemManager()
{
  clear();
}

void te::srs::SpatialReferenceSystemManager::init()
{
  if(!m_set.empty())
    throw Exception(TE_TR("The spatial reference system manager is already initialized!"));
  
  std::string jsonf = te::common::FindInTerraLibPath("share/terralib/json/srs.json");
  
  if(jsonf.empty())
    throw Exception(TE_TR("Could not find srs.json!"));

  init(jsonf);
}

void te::srs::SpatialReferenceSystemManager::init(const std::string& fileName)
{
  try
  {
    clear();
    LoadSpatialReferenceSystemManager(fileName, this);
  }
  catch(boost::property_tree::json_parser::json_parser_error &je)
  {
    std::string errmsg = "Error parsing: " + je.filename() + ": " + je.message();
    te::srs::Exception ex(TE_TR(errmsg));
    throw(ex);
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return;
}

void te::srs::SpatialReferenceSystemManager::add(const std::string& name, const std::string& p4Txt, const std::string& wkt, unsigned int id, const std::string& authName)
{
  std::string key = authName;
  key += ":";
  key += boost::lexical_cast<std::string>(id);
  
  boost::multi_index::nth_index<srs_set,0>::type::iterator it = boost::multi_index::get<0>(m_set).find(key);
  if (it != boost::multi_index::get<0>(m_set).end())
    throw te::srs::Exception(TE_TR("The CS identification already exists in the manager.")); 
  
  srs_desc record(name, id, authName, p4Txt, wkt);
  m_set.insert(record);
}

bool te::srs::SpatialReferenceSystemManager::recognizes(unsigned int id, const std::string& authName) const
{
  std::string key = authName;
  key += ":";
  key += boost::lexical_cast<std::string>(id);
  
  boost::multi_index::nth_index<srs_set,0>::type::iterator it = boost::multi_index::get<0>(m_set).find(key);
  if (it == boost::multi_index::get<0>(m_set).end())
  {
    key = "USER";
    key += ":";
    key += boost::lexical_cast<std::string>(id);
    it = boost::multi_index::get<0>(m_set).find(key);
    return (it != boost::multi_index::get<0>(m_set).end());
  }
  else
    return true;
}

std::auto_ptr<te::srs::SpatialReferenceSystem> te::srs::SpatialReferenceSystemManager::getSpatialReferenceSystem(unsigned int id, const std::string& authName) const
{
  std::string wkt = getWkt(id,authName);
  if (wkt.empty())
    return std::auto_ptr<te::srs::SpatialReferenceSystem>();
  
  try
  {
    return std::auto_ptr<te::srs::SpatialReferenceSystem>(te::srs::WKTReader::read(wkt.c_str()));
  }
  catch(...)
  {
    throw te::srs::Exception(TE_TR("Error parsing the registered CS WKT."));
  }
  return std::auto_ptr<te::srs::SpatialReferenceSystem>();
}


std::string te::srs::SpatialReferenceSystemManager::getName(unsigned int id, const std::string& authName) const
{
  std::string key = authName;
  key += ":";
  key += boost::lexical_cast<std::string>(id);
  
  boost::multi_index::nth_index<srs_set,0>::type::iterator it = boost::multi_index::get<0>(m_set).find(key);
  if (it == boost::multi_index::get<0>(m_set).end())
  {
    key = "USER";
    key += ":";
    key += boost::lexical_cast<std::string>(id);
    it = boost::multi_index::get<0>(m_set).find(key);
    if (it!=boost::multi_index::get<0>(m_set).end())
      return it->m_name;
  }
  else
    return it->m_name;
  
  return "";
}

std::string te::srs::SpatialReferenceSystemManager::getWkt(unsigned int id, const std::string& authName) const
{
  std::string key = authName;
  key += ":";
  key += boost::lexical_cast<std::string>(id);
  
  boost::multi_index::nth_index<srs_set,0>::type::iterator it = boost::multi_index::get<0>(m_set).find(key);
  if (it==boost::multi_index::get<0>(m_set).end())
  {
    key = "USER";
    key += ":";
    key += boost::lexical_cast<std::string>(id);
    it = boost::multi_index::get<0>(m_set).find(key);
    if (it!=boost::multi_index::get<0>(m_set).end())
      return it->m_wkt;
  }
  else
    return it->m_wkt;
  
  return "";
}

std::string te::srs::SpatialReferenceSystemManager::getP4Txt(unsigned int id, const std::string& authName) const
{
  std::string key = authName;
  key += ":";
  key += boost::lexical_cast<std::string>(id);
  
  boost::multi_index::nth_index<srs_set,0>::type::iterator it = boost::multi_index::get<0>(m_set).find(key);
  if (it==boost::multi_index::get<0>(m_set).end())
  {
    key = "USER";
    key += ":";
    key += boost::lexical_cast<std::string>(id);
    it = boost::multi_index::get<0>(m_set).find(key);
    if (it!=boost::multi_index::get<0>(m_set).end())
      return it->m_p4txt;
  }
  else
    return it->m_p4txt;
  
  return "";
}

std::pair<std::string,unsigned int> te::srs::SpatialReferenceSystemManager::getIdFromName(const std::string& name) const
{ 
  boost::multi_index::nth_index<srs_set,1>::type::iterator it = boost::multi_index::get<1>(m_set).find(name);
  if (it==boost::multi_index::get<1>(m_set).end()) 
    throw te::srs::Exception(TE_TR("CS name not recognized."));
  
  return std::pair<std::string,unsigned int>(it->m_auth_name, it->m_auth_id);
}


std::pair<std::string,unsigned int> te::srs::SpatialReferenceSystemManager::getIdFromP4Txt(const std::string& p4Txt) const
{  
  boost::multi_index::nth_index<srs_set,2>::type::iterator it = boost::multi_index::get<2>(m_set).find(p4Txt);
  if (it==boost::multi_index::get<2>(m_set).end()) 
    throw te::srs::Exception(TE_TR("CS name not recognized."));
  
  return std::pair<std::string,unsigned int>(it->m_auth_name, it->m_auth_id);
}

std::pair<std::string,unsigned int> te::srs::SpatialReferenceSystemManager::getIdFromWkt(const std::string& wkt) const
{ 
  boost::multi_index::nth_index<srs_set,3>::type::iterator it = boost::multi_index::get<3>(m_set).find(wkt);
  if (it==boost::multi_index::get<3>(m_set).end()) 
    throw te::srs::Exception(TE_TR("CS name not recognized."));

  return std::pair<std::string,unsigned int>(it->m_auth_name, it->m_auth_id);
}

void te::srs::SpatialReferenceSystemManager::remove(unsigned int id, const std::string& authName)
{ 
  std::string key = authName;
  key += ":";
  key += boost::lexical_cast<std::string>(id);
  
  boost::multi_index::nth_index<srs_set,0>::type::iterator it = boost::multi_index::get<0>(m_set).find(key);
  if (it==boost::multi_index::get<0>(m_set).end())
  {
    key = "USER";
    key += ":";
    key += boost::lexical_cast<std::string>(id);
    it = boost::multi_index::get<0>(m_set).find(key);
    if (it!=boost::multi_index::get<0>(m_set).end())
      m_set.erase(it); 
  }
  else
    m_set.erase(it);
}

void te::srs::SpatialReferenceSystemManager::clear()
{
  m_set.clear();
}

std::pair<te::srs::SpatialReferenceSystemManager::iterator,te::srs::SpatialReferenceSystemManager::iterator> 
te::srs::SpatialReferenceSystemManager::getIterators() const
{
  return std::pair<te::srs::SpatialReferenceSystemManager::iterator,
                   te::srs::SpatialReferenceSystemManager::iterator>(boost::multi_index::get<0>(m_set).begin(), boost::multi_index::get<0>(m_set).end());
}

size_t te::srs::SpatialReferenceSystemManager::size() const
{
  return m_set.size();
}

te::common::UnitOfMeasurePtr te::srs::SpatialReferenceSystemManager::getUnit(unsigned int id, const std::string& authName)
{
  std::string unitName= "metre";
  if (isGeographic(id,authName))
    unitName = "degree";
  
  std::string pjstr = getP4Txt(id,authName);
  if (pjstr.empty())
    return te::common::UnitOfMeasurePtr();
  
  std::size_t found = pjstr.find("+units=");
  if (found!=std::string::npos)
  {
    std::size_t aux = pjstr.find(" ", found);
    std::string unitsymbol = pjstr.substr(found+7,aux-(found+7));
    return te::common::UnitsOfMeasureManager::getInstance().findBySymbol(unitsymbol);
  }
  
  return te::common::UnitsOfMeasureManager::getInstance().find(unitName);
}


bool te::srs::SpatialReferenceSystemManager::isGeographic(unsigned int id, const std::string& authName)
{
  std::string pjstr = getP4Txt(id,authName);
  return (pjstr.find("+proj=longlat")!=std::string::npos); 
}
