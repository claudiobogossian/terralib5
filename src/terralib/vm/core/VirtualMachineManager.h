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
  \file terralib/vm/core/VirtualMachineManager.h

  \brief A singleton for managing TerraLib Virtual Machines.

  \author Frederico Augusto Bedê
  \author Gilberto Ribeiro de Queiroz
*/

#ifndef __TERRALIB_VM_CORE_VIRTUALMACHINEMANAGER_H__
#define __TERRALIB_VM_CORE_VIRTUALMACHINEMANAGER_H__

// TerraLib
#include "Config.h"

// STL
#include <memory>

#include <boost/function.hpp>

namespace te
{
  namespace vm
  {
    namespace core
    {
// Forward declarations
      class VirtualMachine;

      /*!
        \class VirtualMachineManager

        \brief A singleton for managing TerraLib Virtual Machines.

        \sa VirtualMachine
      */
      class TEVMEXPORT VirtualMachineManager
      {
        public: 

          /*!
            \brief It returns the VM identified by id.

            \param id The VM identifier. Example: lua, python.

            \return A pointer to a VM managed by this singleton.

            \exception te::OutOfRangeException If there is no virtual machine registered with the given id.
           */
          VirtualMachine* get(const std::string& id) const;

          /*!
            \brief It adds a new VM to be managed.

            \param id  The VM id.
            \param lvm The VM to be managed. The singleton will take the VM ownership.

            \exception te::InvalidArgumentException It throws an exception if a VM with the same ID already exists.
           */
          void insert(const std::string& id, std::unique_ptr<VirtualMachine> vm);

          /*! \brief It releases all VM. */
          void clear();

          /*!
            \brief It removes a given virtual machine.

            \param id The VM id.

            \exception te::OutOfRangeException If there is no virtual machine registered with the given id.
           */
          void erase(const std::string& id);

          //! Return a reference to the singleton.
          static VirtualMachineManager& instance();

        private:

          /*! \brief Singleton constructor. */
          VirtualMachineManager();

          /*! \brief Singleton destructor. */
          ~VirtualMachineManager();

// No copy allowed
          VirtualMachineManager(const VirtualMachineManager&); 
          VirtualMachineManager& operator=(const VirtualMachineManager&);

        private:

          struct Impl;
          Impl* m_pimpl;
      };

    } // end namespace core
  }   // end namespace vm
}     // end namespace te

#endif  // __TERRALIB_VM_CORE_VIRTUALMACHINEMANAGER_H__
