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
  \file VirtualMachine.h

  \brief An abstract class that defines a Virtual Machine for executing code in any TerraLib supported language.
*/

#ifndef __TERRALIB_BINDING_VM_INTERNAL_VIRTUALMACHINE_H
#define __TERRALIB_BINDING_VM_INTERNAL_VIRTUALMACHINE_H

// TerraLib
#include "Config.h"

// STL
#include <string>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace vm
  {
    class TEVMEXPORT VirtualMachine : public boost::noncopyable
    {
      public:

        VirtualMachine();

        virtual ~VirtualMachine();

        virtual std::string getName() const = 0;

        virtual std::string getTitle() const = 0;

        virtual std::string getDescription() const = 0;

        virtual void build(const std::string& file) = 0;

        virtual void execute() = 0;

        virtual void pause() = 0;

        virtual void stop() = 0;

        virtual void reset() = 0;
    };

  } // end namespace vm
}   // end namespace te

#endif  // __TERRALIB_BINDING_VM_INTERNAL_VIRTUALMACHINE_H

