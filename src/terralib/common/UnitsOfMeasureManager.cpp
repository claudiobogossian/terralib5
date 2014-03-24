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
  \file terralib/common/UnitsOfMeasureManager.cpp

  \brief A singleton class for dealing with units-of-measurement.  
*/

// TerraLib
#include "Exception.h"
#include "STLUtils.h"
#include "StringUtils.h"
#include "Translator.h"
#include "UnitOfMeasure.h"
#include "UnitsOfMeasureManager.h"

// STL
#include <algorithm>
#include <cassert>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

te::common::UnitsOfMeasureManager::UnitsOfMeasureManager()
{}

te::common::UnitsOfMeasureManager::~UnitsOfMeasureManager()
{
  clear();
}

void te::common::UnitsOfMeasureManager::clear()
{
  m_uoms.clear();
  m_uomsIdxByName.clear();
}

void te::common::UnitsOfMeasureManager::insert(UnitOfMeasurePtr& uom)
{
  assert(uom.get());

  const_iterator it = m_uoms.find(uom->getId());
  if(it != m_uoms.end())
    return;
  
  m_uoms.insert(std::pair<unsigned int, UnitOfMeasurePtr>(uom->getId(),uom));

  std::string upstr = boost::to_upper_copy(uom->getName());
  m_uomsIdxByName.insert(std::pair<std::string, unsigned int>(upstr,uom->getId()));
}

void te::common::UnitsOfMeasureManager::insert(UnitOfMeasurePtr& uom, const std::vector<std::string>& alternativeNames)
{
  insert(uom);
  
  for(std::size_t i=0; i<alternativeNames.size(); ++i)
  {
    std::string upstr = boost::to_upper_copy(alternativeNames[i]);
    m_uomsIdxByName.insert(std::pair<std::string, unsigned int>(upstr,uom->getId()));
  }
}

void te::common::UnitsOfMeasureManager::remove(UnitOfMeasurePtr& uom)
{
  assert(uom.get());

  iterator it = m_uoms.find(uom->getId());
  if (it == m_uoms.end())
    return;
  
  m_uoms.erase(it);
  
  for (std::map<std::string, unsigned int>::iterator itn = m_uomsIdxByName.begin(); itn != m_uomsIdxByName.end();)
  {
    if (itn->second == uom->getId())
      m_uomsIdxByName.erase(itn++);
    else
      ++itn;
  }
}

te::common::UnitOfMeasurePtr
te::common::UnitsOfMeasureManager::find(unsigned int id) const
{ 
  const_iterator it = m_uoms.find(id);
  if (it == m_uoms.end())
    return UnitOfMeasurePtr();
  
  return it->second;
}

te::common::UnitOfMeasurePtr
te::common::UnitsOfMeasureManager::find(const std::string& name) const
{
  std::string upstr = boost::to_upper_copy(name);
  
  std::map<std::string, unsigned int>::const_iterator it = m_uomsIdxByName.find(upstr);
  
  if(it != m_uomsIdxByName.end())
  {
    const_iterator it2 = m_uoms.find(it->second);
    if (it2 != m_uoms.end())
      return it2->second;
  }
  
  return UnitOfMeasurePtr();
}

te::common::UnitOfMeasurePtr
te::common::UnitsOfMeasureManager::findBySymbol(const std::string& symbol) const
{
  const_iterator it = m_uoms.begin();
  while (it != m_uoms.end())
  {
    if (it->second->getSymbol() == symbol)
      return it->second;
    ++it;
  }
  return UnitOfMeasurePtr();
}

void te::common::UnitsOfMeasureManager::getNames(UnitOfMeasurePtr& uom, std::vector<std::string>& names) const
{
  std::map<std::string, unsigned int>::const_iterator it = m_uomsIdxByName.begin();
  while (it != m_uomsIdxByName.end()) 
  {
    if (it->second == uom->getId())
      names.push_back(it->first);
    ++it;
  }
}

double te::common::UnitsOfMeasureManager::getConversion(const std::string& unitFromName, const std::string& unitToName) const
{
  UnitOfMeasurePtr uFrom = this->find(unitFromName);
  UnitOfMeasurePtr uTo = this->find(unitToName);

  if (uFrom->getType() != uTo->getType())
    throw Exception(TE_TR("There is not conversion between units for different types of measures."));

  if (uFrom->getBaseUnitId() == uTo->getId()) // converting from derived to base
  {
    return (uFrom->getConversionValue());
  }
  else if (uTo->getBaseUnitId() == uFrom->getId())  // converting from base to derived
  {
    double a, b, c, d;
    
    uTo->getConversionFactors(a,b,c,d);
    return ((b-d)/(c-a));
  }
  throw Exception(TE_TR("There is no known conversion."));
}

void te::common::UnitsOfMeasureManager::init()
{
  if(!m_uoms.empty())
    throw Exception(TE_TR("The unit of measure manager is already initialized!"));

  boost::property_tree::ptree pt;

  const char* te_env = getenv("TERRALIB_DIR");

  if(te_env == 0)
    throw Exception(TE_TR("Environment variable \"TERRALIB_DIR\" not found.\nTry to set it before run the application."));

  std::string uom_file(te_env);
  uom_file += "/resources/json/uom.json";

  boost::property_tree::json_parser::read_json(uom_file, pt);

  BOOST_FOREACH(boost::property_tree::ptree::value_type& v, pt.get_child("units"))
  {
    unsigned int id = v.second.get<unsigned int>("id");
    std::string name = v.second.get<std::string>("name");
    std::string symbol = v.second.get<std::string>("symbol");
    std::string stype = v.second.get<std::string>("type");
    std::string description = v.second.get<std::string>("description");
    unsigned int targetUOM = v.second.get<unsigned int>("target_uom");
    double a = v.second.get<double>("factor_a");
    double b = v.second.get<double>("factor_b");
    double c = v.second.get<double>("factor_c");
    double d = v.second.get<double>("factor_d");
    
    MeasureType t = Length;

    if(stype == "length")
      t = Length;
    else if(stype == "area")
      t = Area;
    else if(stype == "volume")
      t = Volume;
    else if(stype == "angle")
      t = Angle;
    else if(stype == "scale")
      t = Scale;
    else if(stype == "time")
      t = Time;
    else if(stype == "speed")
      t = Speed;
    else
    {
      throw Exception((boost::format(TE_TR("Invalid unit of measure type: %1%!")) % stype).str());
    }

    UnitOfMeasurePtr uom(new UnitOfMeasure(id, name, symbol, t, targetUOM, a, b, c, d, description));

    insert(uom);
  }
}

