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

// TerraLib
#include "CI_ResponsibleParty.h"

// STL
#include <cassert>

te::md::CI_ResponsibleParty::CI_ResponsibleParty(const std::string& indname,
                                                 const std::string& orgname,
                                                 const std::string& posname,
                                                 CI_RoleCode code):
  m_individualName(indname),
  m_organizationName(orgname),
  m_positionName(posname), 
  m_roleCode(code)
{}

te::md::CI_ResponsibleParty::~CI_ResponsibleParty()
{
}

te::md::CI_ResponsibleParty* te::md::CI_ResponsibleParty::clone() const
{
  te::md::CI_ResponsibleParty* nrp = new te::md::CI_ResponsibleParty(m_individualName,m_organizationName, m_positionName, m_roleCode);
  return nrp;
}

void
te::md::CI_ResponsibleParty::setIndividualName(const std::string& name)
{
  assert(!name.empty());
  
  m_individualName = name;
}

const std::string&
te::md::CI_ResponsibleParty::getIndividualName() const
{
  return m_individualName;
}

void 
te::md::CI_ResponsibleParty::setOrganizationName(const std::string& name)
{
  assert(!name.empty());
  
  m_organizationName = name;
}

const std::string& 
te::md::CI_ResponsibleParty::getOrganizationName() const
{
  return m_organizationName;
}

void 
te::md::CI_ResponsibleParty::setPositionName(const std::string& name)
{ 
  assert(!name.empty());

  m_positionName = name;
}

const std::string& 
te::md::CI_ResponsibleParty::getPositionName() const
{
  return m_positionName;
}

te::md::CI_RoleCode 
te::md::CI_ResponsibleParty::getRoleCode() const
{
  return m_roleCode;
}
