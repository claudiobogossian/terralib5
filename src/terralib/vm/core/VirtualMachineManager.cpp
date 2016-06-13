/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/vm/core/VirtualMachineManager.cpp

  \brief A singleton for managing TerraLib Virtual Machines.

  \author Frederico Augusto Bedê
  \author Gilberto Ribeiro de Queiroz
*/

// TerraLib
#include "VirtualMachineManager.h"

#include "Exception.h"
#include "VirtualMachine.h"

#include "../../core/translator/Translator.h"

// STL
#include <map>

struct te::vm::core::VirtualMachineManager::Impl
{
  std::map<std::string, std::shared_ptr<VirtualMachine> > m_vms;  //!< A map from (VM id) to (VM instance).
};

te::vm::core::VirtualMachine*
te::vm::core::VirtualMachineManager::get(const std::string& id) const
{
  std::map<std::string, std::shared_ptr<VirtualMachine> >::iterator it = m_pimpl->m_vms.find(id);

  if(it == m_pimpl->m_vms.end())
    throw te::OutOfRangeException() << te::ErrorDescription(TE_TR("There is NO Virtual Machine registered for the required id."));

  return it->second.get();
}

void
te::vm::core::VirtualMachineManager::insert(const std::string& id, std::unique_ptr<VirtualMachine> vm)
{
  std::map<std::string, std::shared_ptr<VirtualMachine> >::iterator it = m_pimpl->m_vms.find(id);

  if(it != m_pimpl->m_vms.end())
    throw te::InvalidArgumentException() << te::ErrorDescription(TE_TR("There is a Virtual Machine already registered with the required id."));

  m_pimpl->m_vms[id] = std::shared_ptr<VirtualMachine>(vm.release());
}

void
te::vm::core::VirtualMachineManager::clear()
{
  m_pimpl->m_vms.clear();
}

void
te::vm::core::VirtualMachineManager::erase(const std::string& id)
{
  std::map<std::string, std::shared_ptr<VirtualMachine> >::iterator it = m_pimpl->m_vms.find(id);

  if(it == m_pimpl->m_vms.end())
    throw te::OutOfRangeException() << te::ErrorDescription(TE_TR("There is NO Virtual Machine registered for the required id."));

  m_pimpl->m_vms.erase(id);
}

te::vm::core::VirtualMachineManager&
te::vm::core::VirtualMachineManager::instance()
{
  static VirtualMachineManager m_singleton;  // The singleton instance.

  return m_singleton;
}

te::vm::core::VirtualMachineManager::VirtualMachineManager()
{
  m_pimpl = new Impl();
}

te::vm::core::VirtualMachineManager::~VirtualMachineManager()
{
  delete m_pimpl;
}
