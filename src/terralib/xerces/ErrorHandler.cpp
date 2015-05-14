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
  \file ErrorHandler.cpp
   
  \brief This class implements Xerces interface for error handlers.
*/

// TerraLib
#include "ErrorHandler.h"
#include "Utils.h"

// Xerces-C++
#include <xercesc/sax/SAXParseException.hpp>

void te::xerces::ErrorHandler::warning(const xercesc::SAXParseException& exc)
{
  m_errors.push_back(ToString(exc.getMessage()));
}

void te::xerces::ErrorHandler::error(const xercesc::SAXParseException& exc)
{
  m_errors.push_back(ToString(exc.getMessage()));
}

void te::xerces::ErrorHandler::fatalError(const xercesc::SAXParseException& exc)
{
  m_errors.push_back(ToString(exc.getMessage()));
}

void te::xerces::ErrorHandler::resetErrors()
{
  m_errors.clear();
}

const std::size_t te::xerces::ErrorHandler::getNumErros() const
{
  return m_errors.size();
}

const std::string& te::xerces::ErrorHandler::getError(std::size_t i) const
{
  return m_errors[i];
}

const std::string te::xerces::ErrorHandler::getErrors() const
{
  std::string errs;

  for(std::size_t i = 0; i < m_errors.size(); ++i)
  {
    if(i != 0)
      errs += "; ";

    errs += m_errors[i];
  }

  return errs;
}



