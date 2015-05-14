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
  \file terralib/common/Exception.cpp

  \brief This class is designed to declare objects to be thrown as exceptions by TerraLib.  
*/

// TerraLib
#include "Exception.h"

// STL
#include <ostream>

te::common::Exception::Exception() throw()
  : m_code(UNKNOWN_EXCEPTION)
{
}

te::common::Exception::Exception(const std::string& what, int code) throw()
  : m_code(code),
    m_msg(what)
{
}

te::common::Exception::Exception(const char* const what, int code) throw()
  : m_code(code),
    m_msg(what)
{
}

te::common::Exception::~Exception() throw()
{
}

int te::common::Exception::code() const throw()
{
  return m_code;
}

const char* te::common::Exception::what() const throw()
{
  return m_msg.c_str();
}

const char* te::common::Exception::getClassName() const throw()
{
  return "te::common::Exception";
}

std::ostream& operator<<(const te::common::Exception& e, std::ostream& o)
{
  o << e.getClassName() << ": " << e.what();

  return o;
}

