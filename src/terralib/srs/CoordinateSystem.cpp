#include "CoordinateSystem.h"

te::srs::CoordinateSystem::CoordinateSystem(const std::string& name, const std::string& unitName) :
  m_name(name),
  m_unitName(unitName),
  m_id(-1),
  m_authority("")
{}

te::srs::CoordinateSystem::~CoordinateSystem()
{
}

void te::srs::CoordinateSystem::getId(int& id, std::string& authName) const
{
  id = m_id;
  authName = m_authority;
}

void te::srs::CoordinateSystem::setId(int id, const std::string& authName)
{
  m_id = id;
  m_authority = authName;
}

