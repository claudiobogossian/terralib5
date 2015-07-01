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
  \file terralib/xml/ReaderFactory.cpp

  \brief This is the abstract factory for XML readers.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "ReaderFactory.h"

std::string te::xml::ReaderFactory::sm_defaultReaderType(TE_XML_DEFAULT_READER_TYPE);

te::xml::Reader* te::xml::ReaderFactory::make()
{
  return te::common::AbstractFactory<te::xml::Reader, std::string>::make(sm_defaultReaderType);
}

te::xml::Reader* te::xml::ReaderFactory::make(const std::string& readerType)
{
  std::string uType = te::common::Convert2UCase(readerType);
  return te::common::AbstractFactory<te::xml::Reader, std::string>::make(uType);
}

void te::xml::ReaderFactory::setDefaultReader(const std::string& readerType)
{
  std::string uType = te::common::Convert2UCase(readerType);
  sm_defaultReaderType = uType;
}

te::xml::ReaderFactory::ReaderFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<te::xml::Reader, std::string>(factoryKey)
{
}

