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
  \file Config.h

  \brief Configuration flags for the TerraLib Xerces Module.
 */

#ifndef __TERRALIB_XERCES_INTERNAL_CONFIG_H
#define __TERRALIB_XERCES_INTERNAL_CONFIG_H

// TerraLib
#include "../xml/Config.h"

#define TE_XERCES_MODULE_NAME "TEXERCES"

/** @name XERCES Module Config
 *  Configuration flags for XERCES module.
 */
//@{

/*!
  \def TE_XERCES_READER_MAX_BUFFSIZE
  
  \brief It defines the default internal buffer size for parsing a XERCES file.
 */
#define TE_XERCES_READER_MAX_BUFFSIZE TE_XML_READER_MAX_BUFFSIZE

/*!
  \def XERCES_DRIVER_IDENTIFIER

  \brief The name of this XML reader driver.
 */
#define XERCES_DRIVER_IDENTIFIER "XERCES"

//@}

#endif  // __TERRALIB_XERCES_INTERNAL_CONFIG_H

