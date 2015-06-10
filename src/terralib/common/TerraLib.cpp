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
  \file terralib/common/TerraLib.cpp

  \brief An utility class to control the startup and cleanup of the TerraLib Platform and its resources.
*/

// TerraLib
#include "TerraLib.h"

// STL
#include <algorithm>
#include <cassert>

void TerraLib::initialize()
{
// let's initialize all non-initialized modules
  const std::size_t size = m_modules.size();

  for(std::size_t i = 0; i < size; ++i)
    initialize(i);
}

void TerraLib::initialize(const std::size_t i)
{
  assert(i < m_modules.size());

  if(m_initialized[i])
    return;

  Module& module = m_modules[i];

// invoke startup just for modules that has an init function
  if(module.m_startFptr != 0)
    (module.m_startFptr)();

  m_initialized[i] = true;
}

void TerraLib::initialize(const std::string& moduleName)
{
  const std::size_t i = getModuleIdx(moduleName);
  initialize(i);
}

void TerraLib::finalize()
{
// let's finalize all modules
  const std::size_t size = m_modules.size();

  for(std::size_t i = 0; i < size; ++i)
    finalize(i);
}

void TerraLib::finalize(const std::size_t i)
{
  assert(i < m_modules.size());

  if(!m_initialized[i])
    return;

  Module& module = m_modules[i];

// invoke cleanup just for modules that has a cleanup function
  if(module.m_cleanupFptr != 0)
    (module.m_cleanupFptr)();

  m_initialized[i] = false;
}

void TerraLib::finalize(const std::string& moduleName)
{
  const std::size_t i = getModuleIdx(moduleName);
  finalize(i);
}

void TerraLib::add(const Module& m)
{
  m_modules.push_back(m);
  m_initialized.push_back(false);
}

void TerraLib::remove(const std::string& moduleName)
{
// let's find the module
  const std::size_t size = m_modules.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    const Module& module = m_modules[i];

    if(module.m_name == moduleName)
    {
// after finding the module let's remove it from the initialization list and the module list
      m_initialized.erase(m_initialized.begin() + i);
      m_modules.erase(m_modules.begin() + i);
      break;
    }
  }
}

void TerraLib::remove(const std::size_t i)
{
  assert((i < m_initialized.size()) && (i < m_modules.size()));

  m_initialized.erase(m_initialized.begin() + i);
  m_modules.erase(m_modules.begin() + i);
}

std::size_t TerraLib::getNumRegModules() const
{
  return m_modules.size();
}

std::size_t TerraLib::getModuleIdx(const std::string& moduleName) const
{
  const std::size_t size = m_modules.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(moduleName == m_modules[i].m_name)
      return i;
  }

  return (std::size_t)(-1);
}

const std::string& TerraLib::getModuleName(std::size_t i) const
{
  assert(i < m_modules.size());
  return m_modules[i].m_name;
}

TerraLib::TerraLib()
{
  //std::atexit(&TerraLib::sfinalize);
}

TerraLib::~TerraLib()
{
}

//void TerraLib::sfinalize()
//{
//  TerraLib::getInstance().finalize();
//}


