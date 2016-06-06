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
  \file terralib/examples/core/encoding/main.cpp

  \brief Examples for the Terralib Encoding Module

  \author Matheus Cavassan Zaglia
*/

// TerraLib
#include <terralib/core/logger/Logger.h>

// STL

int main(int argc, char *argv[])
{
  TE_LOG_CORE_TRACE("mensagem");
  TE_INIT_LOGGER("terraview", "logs");
  TE_LOG_CORE_TRACE("log2");
  TE_LOG_CORE_TRACE("log2");
}
