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
  \file terralib/core/translator/Translator.cpp

  \brief This class is designed for dealing with multi-language text translation in TerraLib.

  \author Matheus Cavassan Zaglia
  \author Gilberto Ribeiro de Queiroz
 */

// Boost
#include <boost/locale.hpp>

// TerraLib
#include "Translator.h"
#include "../Exception.h"


// GNU Text Utilities

const char* te::core::Translator::translate(const std::string& message, const std::string& textDomain)
{
  return translate(message.c_str(), textDomain.c_str());
}

const char* te::core::Translator::translate(const char* message, const char* textDomain)
{

#ifdef TERRALIB_TRANSLATOR_ENABLED
  return boost::locale::dgettext(textDomain, message).c_str();
#else
  return message;
#endif
}

const char* te::core::Translator::translate(const std::string& textDomain,
                                              const std::string& msg1,
                                              const std::string& msg2,
                                              unsigned int n)
{
  return translate(textDomain.c_str(), msg1.c_str(), msg2.c_str(), n);
}

const char* te::core::Translator::translate(const char* textDomain,
                                              const char* msg1,
                                              const char* msg2,
                                              unsigned int n)
{
#ifdef TERRALIB_TRANSLATOR_ENABLED
  return boost::locale::dngettext(textDomain, msg1, msg2, n).c_str();
#else
  return ((n == 1) ? msg1 : msg2);
#endif
}

const char* te::core::Translator::addTextDomain(const std::string& textDomain, const std::string& textDomainDir)
{
  if(exist(textDomain))
    throw;// Exception(TE_TR("The text domain already exist."));

  m_textDomainMap[textDomain] = textDomainDir;

#ifdef TERRALIB_TRANSLATOR_ENABLED
  return bindtextdomain(textDomain.c_str(), textDomainDir.c_str());
#else
  return "";
#endif
}

const char* te::core::Translator::setTextDomainCodeSet(const std::string& textDomain, const std::string& codeset)
{
  if(exist(textDomain) == false)
    throw;// Exception(TE_TR("The text domain doesn't exist."));

#ifdef TERRALIB_TRANSLATOR_ENABLED
  return bind_textdomain_codeset(textDomain.c_str(), codeset.c_str());
#else
  return "";
#endif
}

bool te::core::Translator::exist(const std::string& textDomain)
{
  std::map<std::string, std::string>::const_iterator it = m_textDomainMap.find(textDomain);

  if(it != m_textDomainMap.end())
    return true;
  else
    return false;
}

std::string te::core::Translator::getTextDomainDir(const std::string& textDomain)
{
  std::map<std::string, std::string>::const_iterator it = m_textDomainMap.find(textDomain);

  if(it != m_textDomainMap.end())
    return it->second;
  else
    return std::string("");
}

te::core::Translator::Translator()
{
}

