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
  \file JsContextManager.cpp
  
  \brief A singleton for managing JavaScript contexts.
 */

// TerraLib
#include "../../../common/Exception.h"
#include "../../../common/Logger.h"
#include "../../../common/STLUtils.h"
#include "../../../common/Translator.h"
#include "JsContext.h"
#include "JsContextManager.h"

te::v8::common::JsContext* te::v8::common::JsContextManager::getCtx(const std::string& id) const
{
  return te::common::GetPValue(m_ctxMap, id);
}

void te::v8::common::JsContextManager::add(const std::string& id, JsContext* ctx)
{
  if(getCtx(id))
    throw te::common::Exception(TR_V8COMMON("Could not manage JsContext because its id already exists!"));

  m_ctxMap[id] = ctx;
}

void te::v8::common::JsContextManager::clear()
{
  te::common::FreeContents(m_ctxMap);
  m_ctxMap.clear();
}

te::v8::common::JsContextManager::JsContextManager()
{
}

te::v8::common::JsContextManager::~JsContextManager()
{
  clear();
}

