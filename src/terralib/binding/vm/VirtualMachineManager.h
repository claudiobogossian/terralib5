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
  \file VirtualMachineManager.h
  
  \brief A singleton for managing TerraLib Virtual Machines.
*/

#ifndef __TERRALIB_BINDING_VM_INTERNAL_VIRTUALMACHINEMANAGER_H
#define __TERRALIB_BINDING_VM_INTERNAL_VIRTUALMACHINEMANAGER_H

// TerraLib
#include "../../common/Singleton.h"
#include "Config.h"

// STL
#include <map>

namespace te
{
  namespace vm
  {
// Forward declarations
    class VirtualMachine;

    /*!
      \class VirtualMachineManager
      
      \brief A singleton for managing TerraLib Virtual Machines.

      \sa VirtualMachine
    */
    class TEVMEXPORT VirtualMachineManager : public te::common::Singleton<VirtualMachineManager>
    {
      friend class te::common::Singleton<VirtualMachineManager>;

      public: 

        /*!
          \brief It returns the VM identified by id.

          \param id The VM identifier.

          \return A pointer to a VM managed by this singleton or NULL if none is found.
          */
        VirtualMachine* getVM(const std::string& id) const;

        /*!
          \brief It adds a new VM to be managed.

          \param id  The VM id.
          \param lvm The VM to be managed. The singleton will take the VM ownership.

          \exception Exception It throws an exception if a VM with the same ID already exists.
          */
        void add(const std::string& id, VirtualMachine* vm);

        /*! \brief It releases all VM. */
        void clear();

        /*!
          \brief It removes all virtual machines from the given type.

          \param vmType The type of VM to be removed.
        */
        void removeAll(const std::string& vmType);

      protected:

        /*! \brief Singleton constructor. */
        VirtualMachineManager();        

        /*! \brief Singleton destructor. */
        ~VirtualMachineManager();

      private:
      
        std::map<std::string, VirtualMachine*> m_vmMap;  //!< A map from (VM id) to (VM instance).
    };

  }   // end namespace vm
}     // end namespace te

#endif  // __TERRALIB_BINDING_VM_INTERNAL_VIRTUALMACHINEMANAGER_H
