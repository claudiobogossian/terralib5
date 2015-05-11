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
  \file JVM.cpp

  \brief An utility class for dealing with Java Virtual Machines.
 */

// TerraLib
#include "../../../common/Exception.h"
#include "../../../common/STLUtils.h"
#include "../../../common/Translator.h"
#include "JsContext.h"

// STL
#include <cassert>

te::v8::common::JsContext::JsContext()
{
// create a persistent context to be used when running JavaScript code
  m_ctx = ::v8::Context::New();
}

te::v8::common::JsContext::~JsContext()
{
  //if(!m_ctx.IsEmpty())
  m_ctx.Dispose();  // it already checks if there is a context!
}

