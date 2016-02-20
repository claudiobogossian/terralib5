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
  \file terralib/module/Module.h

  \brief The base abstraction of a module.

  \author Gilberto Ribeiro de Queiroz
  \author Frederico Augusto Bede
 */

#ifndef __TERRALIB_MODULE_MODULE_H__
#define __TERRALIB_MODULE_MODULE_H__

// TerraLib
#include "Config.h"
#include "Info.h"

// STL
#include <memory>
#include <vector>

// Boost
#include <boost/function.hpp>

namespace te
{
  namespace module
  {
    typedef boost::function0<void> InitializerFnct;
    typedef boost::function0<void> FinalizerFnct;

    //! The base abstraction of a module.
    struct Module
    {
      Info info;
      InitializerFnct startup_fnct;
      FinalizerFnct shutdown_fnct;
      std::vector<std::shared_ptr<Module> > dependencies;
    };

  } // end namespace module
}   // end namespace te

#endif  // __TERRALIB_MODULE_MODULE_H__
