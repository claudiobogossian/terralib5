/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/xml/AbstractWriterFactory.cpp

  \brief This is the abstract factory for XML writers.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "AbstractWriterFactory.h"
#include "Writer.h"

std::string te::xml::AbstractWriterFactory::sm_defaultWriterType(TE_XML_DEFAULT_WRITER_TYPE);

te::xml::AbstractWriter* te::xml::AbstractWriterFactory::make()
{
  return te::common::AbstractFactory<te::xml::AbstractWriter, std::string>::make(sm_defaultWriterType);
}

te::xml::AbstractWriter* te::xml::AbstractWriterFactory::make(const std::string& writerType)
{
  if(te::common::Convert2UCase(writerType) == "BASIC")
    return new Writer;

  std::string uType = te::common::Convert2UCase(writerType);
  return te::common::AbstractFactory<te::xml::AbstractWriter, std::string>::make(uType);
}

void te::xml::AbstractWriterFactory::setDefaultWriter(const std::string& writerType)
{
  std::string uType = te::common::Convert2UCase(writerType);
  sm_defaultWriterType = uType;
}

te::xml::AbstractWriterFactory::AbstractWriterFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<te::xml::AbstractWriter, std::string>(factoryKey)
{
}

