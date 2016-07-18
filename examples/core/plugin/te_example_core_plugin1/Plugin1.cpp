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
  \file terralib/examples/core/plugin/Plugin1.cpp

  \brief Examples for the Terralib Plugin Module

  \author Matheus Cavassan Zaglia
*/
#include "Plugin1.h"

void Plugin1::startup()
{
  std::cout << "Plugin1 startup" << std::endl;
}

void Plugin1::shutdown()
{
  std::cout << "Plugin1 shutdown" << std::endl;
}


TERRALIB_PLUGIN_CALL_BACK_IMPL(Plugin1)


