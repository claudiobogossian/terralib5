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
  \file terralib/core/encoding/CharEncoding.cpp

  \brief A class for handling character enconding/decoding.

  \author Matheus Cavassan Zaglia
  \author Gilberto Ribeiro de Queiroz
*/

// TerraLib
#include "../../BuildConfig.h"
#include "../../Defines.h"
#include "Platform.h"

// Boost
#include <boost/filesystem.hpp>

std::string te::core::FindInTerraLibPath(const std::string& path)
{
// 1st: look in the neighborhood of the executable
  boost::filesystem::path tl_path = boost::filesystem::current_path();

  boost::filesystem::path eval_path = tl_path / path;

  if(boost::filesystem::exists(eval_path))
    return eval_path.string();

  // Check for one path above, works for Install
  tl_path /= "..";
  eval_path = tl_path / path;

  if(boost::filesystem::exists(eval_path))
    return eval_path.string();

  //Check for two paths above, works for Examples/Unittest
  tl_path /="..";
  eval_path = tl_path / path;

  if(boost::filesystem::exists(eval_path))
    return eval_path.string();

// 2rd: look for an environment variable defined by macro TERRALIB_DIR_VAR_NAME
  const char* te_env = getenv(TERRALIB_DIR_VAR_NAME);

  if(te_env != 0)
  {
    tl_path = te_env;

    eval_path = tl_path / path;

    if(boost::filesystem::exists(eval_path))
      return eval_path.string();
  }

// 3th: look into install prefix-path
  tl_path = TERRALIB_INSTALL_PREFIX_PATH;

  eval_path = tl_path / path;

  if(boost::filesystem::exists(eval_path))
    return eval_path.string();

// 4nd: look into the codebase path
  tl_path = TERRALIB_CODEBASE_PATH;

  eval_path = tl_path / path;

  if(boost::filesystem::exists(eval_path))
    return eval_path.string();


  return "";
}
