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
  \file VirtualMachineManager.cpp
  
  \brief A singleton for managing TerraLib Virtual Machines.
*/

// TerraLib
#include "../../common/STLUtils.h"
#include "../../common/Translator.h"
#include "Exception.h"
#include "VirtualMachine.h"
#include "VirtualMachineManager.h"

te::vm::VirtualMachine* te::vm::VirtualMachineManager::getVM(const std::string& id) const
{
  return te::common::GetPValue(m_vmMap, id);
}

void te::vm::VirtualMachineManager::add(const std::string& id, VirtualMachine* vm)
{
  if(getVM(id))
    throw Exception(TR_VM("Could not manage VM because its id already exists!"));

  m_vmMap[id] = vm;
}

void te::vm::VirtualMachineManager::clear()
{
  te::common::FreeContents(m_vmMap);
  m_vmMap.clear();
}

void te::vm::VirtualMachineManager::removeAll(const std::string& vmType)
{
  std::map<std::string, VirtualMachine*>::iterator it = m_vmMap.begin();
  std::map<std::string, VirtualMachine*>::iterator itend = m_vmMap.end();

  while(it != itend)
  {
    if(it->second->getName() == vmType)
    {
      VirtualMachine* vm = it->second;
      ++it;
      delete vm;
      m_vmMap.erase(it);
    }
    else
      ++it;
  }
}

te::vm::VirtualMachineManager::VirtualMachineManager()
{
}

te::vm::VirtualMachineManager::~VirtualMachineManager()
{
  //clear();
}
