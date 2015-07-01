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
  \file SimpleLink.cpp
  
  \brief Simple link.
 */

// TerraLib
#include "Globals.h"
#include "SimpleLink.h"

te::xl::SimpleLink::SimpleLink()
  : m_show(SHOW_NONE),
    m_actuate(ACTUATE_NONE)
{
}

te::xl::SimpleLink::~SimpleLink()
{
}

const std::string& te::xl::SimpleLink::getType()
{
  return Globals::sm_xlinkType;
}

void te::xl::SimpleLink::setHref(const std::string& href)
{
  m_href = href;
}

const std::string& te::xl::SimpleLink::getHref() const
{
  return m_href;
}

void te::xl::SimpleLink::setRole(const std::string& role)
{
  m_role = role;
}

const std::string& te::xl::SimpleLink::getRole() const
{
  return m_role;
}

void te::xl::SimpleLink::setArcRole(const std::string& arcrole)
{
  m_arcrole = arcrole;
}

const std::string& te::xl::SimpleLink::getArcRole() const
{
  return m_arcrole;
}

void te::xl::SimpleLink::setTitle(const std::string& title)
{
  m_title = title;
}

const std::string& te::xl::SimpleLink::getTitle() const
{
  return m_title;
}

void te::xl::SimpleLink::setShow(ShowType show)
{
  m_show = show;
}

te::xl::ShowType te::xl::SimpleLink::getShow() const
{
  return m_show;
}

void te::xl::SimpleLink::setActuate(ActuateType actuate)
{
  m_actuate = actuate;
}

te::xl::ActuateType te::xl::SimpleLink::getActuate() const
{
  return m_actuate;
}

te::xl::SimpleLink* te::xl::SimpleLink::clone() const
{
  SimpleLink* sl = new SimpleLink;

  sl->m_href = m_href;
  sl->m_role = m_role;
  sl->m_arcrole = m_arcrole;
  sl->m_title = m_title;
  sl->m_show = m_show;
  sl->m_actuate = m_actuate;

  return sl;
}