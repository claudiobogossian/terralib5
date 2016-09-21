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
  \file terralib/ws/core/Exception.h

  \brief Exception classes for the WS Core Runtime Library.

  \author Emerson Moraes
 */

#ifndef __TERRALIB_WS_CORE_EXCEPTION_H__
#define __TERRALIB_WS_CORE_EXCEPTION_H__

//TerraLib
#include "../../common/Exception.h"
#include "Config.h"

namespace te
{
  namespace ws
  {
    namespace core
    {
      //! Base exception class for WS Core Runtime Library.
      TE_DECLARE_EXCEPTION_CLASS(TEWSCOREEXPORT, Exception, te::common::Exception)
    }
  }
}

#endif
