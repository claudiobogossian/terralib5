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
  \file terralib/vm/python/VirtualMachine.h

  \brief Implementation of a virtual machine for the Python Programming Language.

  \author Frederico Augusto Bedê
*/

#ifndef __TERRALIB_VM_PYTHON_VIRTUALMACHINE_H__
#define __TERRALIB_VM_PYTHON_VIRTUALMACHINE_H__

// TerraLib
#include "../core/VirtualMachine.h"

namespace te
{
  namespace vm
  {
    namespace python
    {
      class VirtualMachine : public te::vm::core::VirtualMachine
      {
        public:

          VirtualMachine();

          ~VirtualMachine();

          std::string getName() const;

          std::string getTitle() const;

          std::string getDescription() const;

          void build(const std::string& file);

          void execute();

          void pause();

          void stop();

          void reset();
      };
    } // end namespace python
  }   // end namespace vm
}     // end namespace te

#endif  // __TERRALIB_VM_PYTHON_VIRTUALMACHINE_H__
