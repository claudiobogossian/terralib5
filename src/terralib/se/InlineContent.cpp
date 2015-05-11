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
  \file terralib/se/InlineContent.cpp
  
  \brief InlineContent is XML or base64 encoded content in some externally-defined format that is included in an SE in-line.
*/

// TerraLib
#include "InlineContent.h"

te::se::InlineContent::InlineContent(const EncodingType& e)
  : m_encoding(e)
{
}

te::se::InlineContent::~InlineContent()
{
}

void te::se::InlineContent::setData(const std::string& d)
{
  m_data = d;
}

const std::string& te::se::InlineContent::getData() const
{
  return m_data;
}

void te::se::InlineContent::setEncoding(EncodingType e)
{
  m_encoding = e;
}

te::se::InlineContent::EncodingType te::se::InlineContent::getEncoding() const
{
  return m_encoding;
}

te::se::InlineContent* te::se::InlineContent::clone() const
{
  InlineContent* ic = new InlineContent(m_encoding);
  ic->setData(m_data);

  return ic;
}

