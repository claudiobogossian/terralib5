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
  \file UnitsOfMeasureManager.cpp
 
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
#include <cassert>
#include <algorithm>

void te::common::UnitsOfMeasureManager::insert(UnitOfMeasure* uom)
{
  assert(uom);

  std::vector<UnitOfMeasure*>::const_iterator it = std::find(m_uoms.begin(),m_uoms.end(),uom);
  if (it != m_uoms.end())
    throw Exception(TR_COMMON("Unit already exists in the manager."));
  
  m_uoms.push_back(uom);
  m_uomsIdxByName.insert(std::map<std::string, UnitOfMeasure*>::value_type(uom->getName(), uom));
}

void te::common::UnitsOfMeasureManager::insert(UnitOfMeasure* uom,
                                                    const std::vector<std::string>& alternativeNames)
{
  assert(uom);

  std::vector<UnitOfMeasure*>::const_iterator it = std::find(m_uoms.begin(),m_uoms.end(),uom);
  if (it != m_uoms.end())
    throw Exception(TR_COMMON("Unit already exists in the manager."));
  
  m_uoms.push_back(uom);
  m_uomsIdxByName.insert(std::map<std::string, UnitOfMeasure*>::value_type(uom->getName(), uom));

  size_t size = alternativeNames.size();

  for(size_t i = 0; i < size; ++i)
  {
    std::string upName = te::common::Convert2UCase(alternativeNames[i]);
    m_uomsIdxByAlternativeName.insert(std::map<std::string, UnitOfMeasure*>::value_type(upName, uom));
  }
}

void te::common::UnitsOfMeasureManager::remove(UnitOfMeasure* uom)
{
  assert(uom);

// remove m_uomsIdxByName index entry
  std::map<std::string, UnitOfMeasure*>::iterator itIdxByName = m_uomsIdxByName.find(uom->getName());

  if(itIdxByName == m_uomsIdxByName.end())
    throw Exception(TR_COMMON("Could not find the informed unit of measure searching by its name."));

  m_uomsIdxByName.erase(itIdxByName);

// remove m_uomsIdxByAlternativeName index entry(ies)
  std::map<std::string, UnitOfMeasure*>::iterator itIdxByAlternativeName = m_uomsIdxByAlternativeName.begin();

  while(itIdxByAlternativeName != m_uomsIdxByAlternativeName.end())
  {
    if(itIdxByAlternativeName->second->getId() == uom->getId())
    {
      std::map<std::string, UnitOfMeasure*>::iterator auxIt = itIdxByAlternativeName;

      ++itIdxByAlternativeName;

      m_uomsIdxByAlternativeName.erase(itIdxByAlternativeName);
    }
    else
    {
      ++itIdxByAlternativeName;
    }
  }

// remove from the list and free it resources

  size_t size = m_uoms.size();

  for(size_t i = 0; i < size; ++i)
    if(uom->getId() == m_uoms[i]->getId())
    {
      delete m_uoms[i];
      m_uoms.erase(m_uoms.begin() + i);
      return;
    }

  throw Exception(TR_COMMON("Could not find the informed unit of measure."));
}

te::common::UnitOfMeasure*
te::common::UnitsOfMeasureManager::findById(unsigned int id) const
{
  assert(id < m_uoms.size());

  return m_uoms[id];
}

te::common::UnitOfMeasure*
te::common::UnitsOfMeasureManager::findByName(const std::string& name) const
{
  assert(name.empty() == false);

  std::string upName = te::common::Convert2UCase(name);

  std::map<std::string, UnitOfMeasure*>::const_iterator itIdxByName = m_uomsIdxByName.find(upName);

  if(itIdxByName != m_uomsIdxByName.end())
    return itIdxByName->second;

  return 0;
}

te::common::UnitOfMeasure*
te::common::UnitsOfMeasureManager::findByAlternativeName(const std::string& name) const
{
  assert(name.empty() == false);

  std::string upName = te::common::Convert2UCase(name);

  std::map<std::string, UnitOfMeasure*>::const_iterator itIdxByAlternativeName = m_uomsIdxByAlternativeName.find(upName);

  if(itIdxByAlternativeName != m_uomsIdxByAlternativeName.end())
    return itIdxByAlternativeName->second;

  return 0;
}

te::common::UnitOfMeasure*
te::common::UnitsOfMeasureManager::find(const std::string& name) const
{
  assert(name.empty() == false);

  std::string upName = te::common::Convert2UCase(name);

  UnitOfMeasure* uom = findByName(upName);

  if(uom)
    return uom;

  uom = findByAlternativeName(name);

  return uom;
}

std::pair<std::vector<te::common::UnitOfMeasure*>::const_iterator,
          std::vector<te::common::UnitOfMeasure*>::const_iterator>
te::common::UnitsOfMeasureManager::getIterator() const
{
  return std::pair<std::vector<te::common::UnitOfMeasure*>::const_iterator,
                   std::vector<te::common::UnitOfMeasure*>::const_iterator>(m_uoms.begin(), m_uoms.end());
}

void te::common::UnitsOfMeasureManager::clear()
{
  m_uomsIdxByName.clear();
  m_uomsIdxByAlternativeName.clear();

  FreeContents(m_uoms);

  m_uoms.clear();
}

te::common::UnitsOfMeasureManager::~UnitsOfMeasureManager()
{
  clear();
}

te::common::UnitsOfMeasureManager::UnitsOfMeasureManager()
{
}

void te::common::UnitsOfMeasureManager::getAlternativeNames(UnitOfMeasure* uom,
                                        std::vector<std::string>& altNames) const
{
  assert(uom);
  std::map<std::string, UnitOfMeasure*>::const_iterator it = m_uomsIdxByAlternativeName.begin();
  while (it != m_uomsIdxByAlternativeName.end()) 
  {
    if (it->second == uom) 
      altNames.push_back(it->first);
    ++it;
  }
}

double te::common::UnitsOfMeasureManager::getConversion(const std::string& unitFromName, 
                                                        const std::string& unitToName) const
{
  assert(unitFromName.empty() == false);
  assert(unitToName.empty() == false);
  
  UnitOfMeasure* uFrom = this->find(unitFromName);
  UnitOfMeasure* uTo = this->find(unitToName);
  
  if (uFrom->getType() != uTo->getType())
    throw Exception(TR_COMMON("There is not conversion between units for different types of measures."));

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
  throw Exception(TR_COMMON("There is no known conversion."));
}
