/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file color.h

  \brief This file contains include headers for the Color Model of TerraLib.

  TerraLib provides a basic set of classes for dealing with color systems and color schemes.
 */

#ifndef __TERRALIB_INTERNAL_COLOR_H
#define __TERRALIB_INTERNAL_COLOR_H

// TerraLib
#include "color/ColorScheme.h"
#include "color/ColorSchemeCatalog.h"
#include "color/ColorSchemeCatalogManager.h"
#include "color/ColorSchemeGroup.h"
#include "color/Config.h"
#include "color/RGBAColor.h"

/*!
  \defgroup color Color

  \brief The color module provides a basic set of classes for dealing with color systems and color schemes.

  By default you can use this module to generate nice colors and manage color catalogs that are suited for your users.

  The main classes/concepts in this module are listed here. The namespace associated to the Color module is te::color.
  To know more about it, see the te::color namespace documentation.
*/

namespace te
{
  /*!
    \brief Namespace for the Color Model of TerraLib.
  */
  namespace color
  {
  } // end namespace color
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_COLOR_H

