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

// TerraLib
#include "Translator.h"
#include "../Exception.h"
#include "../utils/Platform.h"

te::core::Translator& te::core::Translator::getInstance()
{
  static Translator instance;

  return instance;
}

std::string te::core::Translator::translate(const std::string& message)
{
  return translate(message.c_str());
}

std::string te::core::Translator::translate(const char* message)
{

#ifdef TERRALIB_TRANSLATOR_ENABLED
  for(const auto& dir : m_textDomainMap)
  {
    for(const auto& domain : dir.second)
    {

      boost::locale::generator gen;
      gen.add_messages_domain(domain);
      gen.add_messages_path(dir.first);

      std::locale::global(gen(m_locale));

      std::string translated = boost::locale::translate(message).str(std::locale());

      if(translated != message)
        return translated;
    }
  }
  return message;
#else
  return message;
#endif
}

std::string
te::core::Translator::translate(const std::string& msg1,
                                const std::string& msg2,
                                unsigned int n)
{
  return translate(msg1.c_str(), msg2.c_str(), n);
}

std::string
te::core::Translator::translate(const char* msg1,
                                const char* msg2,
                                unsigned int n)
{
#ifdef TERRALIB_TRANSLATOR_ENABLED
  for(const auto& dir : m_textDomainMap)
  {
    for(const auto& domain : dir.second)
    {
      boost::locale::generator gen;
      gen.add_messages_domain(domain);
      gen.add_messages_path(dir.first);

      std::locale::global(gen(m_locale));

      std::string translated = boost::locale::translate(msg1, msg2, n).str(std::locale());

      if(n == 1 && translated != msg1)
        return translated;
      else if(n > 1 && translated != msg2)
        return translated;
    }
  }
  return ((n == 1) ? msg1 : msg2);
#else
  return ((n == 1) ? msg1 : msg2);
#endif
}

void te::core::Translator::setLocale(const std::string& locale)
{
  m_locale = locale + ".UTF-8";
}

void te::core::Translator::addTextDomain(const std::string& textDomain, const std::string& dir)
{
  if(exist(textDomain))
  {
    boost::format err_msg(TE_TR("The text domain %1% already exist."));
    throw Exception() << te::ErrorDescription((err_msg % textDomain).str());
  }
  m_textDomainMap[dir].push_back(textDomain);
}


bool te::core::Translator::exist(const std::string& textDomain)
{
  for(const auto& dir : m_textDomainMap)
  {
    auto domain = std::find(dir.second.begin(), dir.second.end(), textDomain);
    if(domain != dir.second.end())
      return true;
  }
  return false;
}

