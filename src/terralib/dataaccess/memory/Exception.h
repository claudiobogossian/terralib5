/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/memory/Exception.h

  \brief An exception class for the TerraLib In-Memory Data Access driver.
*/

#ifndef __TERRALIB_DATAACCESS_MEMORY_INTERNAL_EXCEPTION_H
#define __TERRALIB_DATAACCESS_MEMORY_INTERNAL_EXCEPTION_H

// TerraLib
#include "../core/Exception.h"
#include "Config.h"


namespace te
{
  namespace da
  {
    namespace mem
    {
      TE_DECLARE_EXCEPTION_CLASS(TEMEMORYEXPORT, Exception, te::da::core::Exception)

    } // end namespace mem
  }   // end namespace da
}     // end namespace te

#endif  // __TERRALIB_DATAACCESS_MEMORY_INTERNAL_EXCEPTION_H

