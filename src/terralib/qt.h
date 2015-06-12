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
  \file qt.h

  \brief This file contains include headers for the Qt supportt in TerraLib.
 */

#ifndef __TERRALIB_INTERNAL_QT_H
#define __TERRALIB_INTERNAL_QT_H

// TerraLib
#include "qt/postgis.h"
#include "qt/widgets.h"

/*!
  \defgroup qt Qt

  \brief This module provides some geospatial components and a framework based on Qt.

    The module is organized as follow:

    <ul>
      <li>Widgets: GUI components for dealing with geospatial concepts;</li>
      <li>Designer: a set of components that can be used inside Qt Designer for rapid development of new forms;</li>
      <li>Plugins: a set of TerraLib Qt plugins;</li>
      <li>Application Framework: a framework to help customizing an application built on top of TerraLib's Qt Components. Notice that components can be used without this framework.</li>
    </ul>

    The namespace associated to the Qt module is te::qt. To know more about it, see the te::qt namespace documentation.
*/
namespace te
{
  /*!
    \brief Namespace for the Qt module of TerraLib.
  */
  namespace qt
  {
  } // end namespace qt
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_QT_H
