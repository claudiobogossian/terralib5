/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/Exception.h

  \brief An exception class for the DataAccess module.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_EXCEPTION_H
#define __TERRALIB_DATAACCESS_INTERNAL_EXCEPTION_H

// TerraLib
#include "../common/Exception.h"
#include "Config.h"


namespace te
{
  namespace da
  {
    TE_DECLARE_EXCEPTION_CLASS(TEDATAACCESSEXPORT, Exception, te::common::Exception)

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_EXCEPTION_H

