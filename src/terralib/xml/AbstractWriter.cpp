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
  \file terralib/xml/AbstractWriter.cpp

  \brief This class models a XML writer object.
*/

// TerraLib
#include "AbstractWriter.h"

// Boost
#include <boost/lexical_cast.hpp>

te::xml::AbstractWriter::AbstractWriter()
  : m_uri(""),
    m_rootNamespaceUri("")
{
}

te::xml::AbstractWriter::~AbstractWriter()
{
}

void te::xml::AbstractWriter::setURI(const std::string& uri)
{
  m_uri = uri;
}

void te::xml::AbstractWriter::setRootNamespaceURI(const std::string& uri)
{
  m_rootNamespaceUri = uri;
}
