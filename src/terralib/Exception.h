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
  \file te/Exception.h

  \brief TerraLib basic exception types.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TERRALIB_EXCEPTION_H__
#define __TERRALIB_EXCEPTION_H__

// STL
#include <stdexcept>
#include <string>

// Boost
#include <boost/exception/all.hpp>

namespace te
{
  //! Base exception class for plugin module.
  struct Exception: virtual std::exception, virtual boost::exception { };

  //! An exception indicating that a given item was not found in a collection.
  struct ItemNotFoundException: virtual Exception { };

  //! An exception indicating that a given item already exists in a collection.
  struct ItemAlreadyExistsException: virtual Exception { };

  //! The base type for error report messages.
  typedef boost::error_info<struct tag_error_description, std::string> ErrorDescription;

}    // end namespace te

#endif  // __TERRALIB_MODULE_EXCEPTION_H__
