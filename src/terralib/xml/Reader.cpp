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
  \file terralib/xml/Reader.cpp
   
  \brief This class models a XML reader object.
*/

// TerraLib
#include "Reader.h"

// Boost
#include <boost/lexical_cast.hpp>

boost::int32_t te::xml::Reader::getElementValueAsInt32() const
{
  return boost::lexical_cast<boost::int32_t>(getElementValue());
}

double te::xml::Reader::getElementValueAsDouble() const
{
  return boost::lexical_cast<double>(getElementValue());
}

bool te::xml::Reader::getElementValueAsBoolean() const
{
  std::string value = getElementValue();

  return (value == "true") || (value == "TRUE"); 
}

boost::int32_t te::xml::Reader::getAttrAsInt32(const std::string& name) const
{
  return boost::lexical_cast<boost::int32_t>(getAttr(name));
}

boost::int32_t te::xml::Reader::getAttrAsInt32(std::size_t i) const
{
  return boost::lexical_cast<boost::int32_t>(getAttr(i));
}

boost::uint32_t te::xml::Reader::getAttrAsUInt32(std::size_t i) const
{
  return boost::lexical_cast<boost::uint32_t>(getAttr(i));
}

boost::uint32_t te::xml::Reader::getAttrAsUInt32(const std::string name) const
{
  return boost::lexical_cast<boost::uint32_t>(getAttr(name));
}

double te::xml::Reader::getAttrAsDouble(const std::string& name) const
{
  return boost::lexical_cast<double>(getAttr(name));
}

double te::xml::Reader::getAttrAsDouble(std::size_t i) const
{
  return boost::lexical_cast<double>(getAttr(i));
}

