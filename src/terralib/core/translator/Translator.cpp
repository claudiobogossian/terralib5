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
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

// TerraLib
#include "Translator.h"
#include "../Exception.h"
#include "../utils/Platform.h"

// GNU Text Utilities

const char* te::core::Translator::translate(const std::string& message)
{
  return translate(message.c_str());
}

const char* te::core::Translator::translate(const char* message)
{

#ifdef TERRALIB_TRANSLATOR_ENABLED
  for(auto it = m_textDomainVector.begin(); it != m_textDomainVector.end();++it)
  {
    std::string domain = *it;

    std::string path = boost::filesystem::current_path().string() + "/../../share/terralib/translations";
    boost::locale::generator gen;
    gen.add_messages_domain(domain);

    gen.add_messages_path(path);
    std::locale::global(gen(""));

    std::string translated = boost::locale::translate(message).str(std::locale());

    if(translated != message)
      return translated.c_str();
  }
  return message;
#else
  return message;
#endif
}

const char* te::core::Translator::translate(const std::string& msg1,
                                              const std::string& msg2,
                                              unsigned int n)
{
  return translate(msg1.c_str(), msg2.c_str(), n);
}

const char* te::core::Translator::translate(const char* msg1,
                                              const char* msg2,
                                              unsigned int n)
{
#ifdef TERRALIB_TRANSLATOR_ENABLED
  for(auto it = m_textDomainVector.begin(); it != m_textDomainVector.end();++it)
  {
    std::string domain = *it;

    std::string path = te::core::FindInTerraLibPath("share/terralib/translations");
    boost::locale::generator gen;
    gen.add_messages_domain(domain);

    gen.add_messages_path(path);
    std::locale::global(gen(""));

    std::string translated = boost::locale::translate(msg1, msg2, n).str(std::locale());

    if(n == 1 && translated != msg1)
      return translated.c_str();
    else if(n > 1 && translated != msg2)
      return translated.c_str();

  }
  return ((n == 1) ? msg1 : msg2);
#else
  return ((n == 1) ? msg1 : msg2);
#endif
}

void te::core::Translator::addTextDomain(const std::string& textDomain)
{
  if(exist(textDomain))
  {
    boost::format err_msg("The text domain %1 already exist.");
    throw Exception() << te::ErrorDescription((err_msg % textDomain).str());
  }
  m_textDomainVector.push_back(textDomain);
}


bool te::core::Translator::exist(const std::string& textDomain)
{
  std::vector<std::string>::iterator it = std::find(m_textDomainVector.begin(),m_textDomainVector.end(), textDomain);
  if(it != m_textDomainVector.end())
    return true;
  return false;
}

