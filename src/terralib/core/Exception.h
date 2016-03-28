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
  \file te/core/Exception.h

  \brief Exception classes for the TerraLib Core Runtime Library.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TERRALIB_CORE_EXCEPTION_H__
#define __TERRALIB_CORE_EXCEPTION_H__

// TerraLib
#include "../Exception.h"

namespace te
{
  namespace core
  {
    //! Base exception class for TerraLib Core Runtime Library.
    struct Exception: virtual te::Exception { };

    //! Base exception for DataRetrieverWCS
    struct URIException : virtual Exception { };

  }  // end namespace core
}    // end namespace te

#endif  // __TERRALIB_CORE_EXCEPTION_H__
