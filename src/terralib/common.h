/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file common.h

  \brief This file contains include headers for the TerraLib Common Runtime module.
 */

#ifndef __TERRALIB_INTERNAL_COMMON_H
#define __TERRALIB_INTERNAL_COMMON_H

// TerraLib
#include "common/AbstractFactory.h"
#include "common/BaseVisitable.h"
#include "common/BaseVisitor.h"
#include "common/ByteSwapUtils.h"
#include "common/Comparators.h"
#include "common/Config.h"
#include "common/Distance.h"
#include "common/Enums.h"
#include "common/Exception.h"
#include "common/FactoryDictionary.h"
#include "common/Globals.h"
#include "common/HexUtils.h"
#include "common/LoggedException.h"
#include "common/Logger.h"
#include "common/Module.h"
#include "common/ParameterizedAbstractFactory.h"
#include "common/Singleton.h"
#include "common/STLUtils.h"
#include "common/StringUtils.h"
#include "common/TerraLib.h"
#include "common/ThreadingPolicies.h"
#include "common/Translator.h"
#include "common/TreeItem.h"
#include "common/UnitOfMeasure.h"
#include "common/UnitsOfMeasureManager.h"
#include "common/Visitor.h"

namespace te
{
  /*!
    \brief Namespace for theTerraLib Common Runtime module.
   */
  namespace common
  {
  }	// end namespace common
}   // end namesp

#endif  // __TERRALIB_INTERNAL_COMMON_H

