#include "SpatialReferenceSystem.h"

te::srs::SpatialReferenceSystem::SpatialReferenceSystem(const std::string& name, const std::string& unitName) :
  m_name(name),
  m_unitName(unitName),
  m_srid(std::pair<unsigned int,std::string>(0,""))
{
}

te::srs::SpatialReferenceSystem::~SpatialReferenceSystem()
{
}

void te::srs::SpatialReferenceSystem::setName(const std::string& name) 
{ 
  m_name = name; 
}

const std::string& te::srs::SpatialReferenceSystem::getName() const 
{ 
  return m_name; 
}

void te::srs::SpatialReferenceSystem::setUnitName(const std::string& unitName)  
{ 
  m_unitName = unitName; 
}

const std::string& te::srs::SpatialReferenceSystem::getUnitName() const 
{ 
  return m_unitName; 
}

void te::srs::SpatialReferenceSystem::getSRID(int& id, std::string& authName) const
{
  id = m_srid.first;
  authName = m_srid.second;
}

void te::srs::SpatialReferenceSystem::setSRID(int id, const std::string& authName)
{
  m_srid.first = id;
  m_srid.second = authName;
}

