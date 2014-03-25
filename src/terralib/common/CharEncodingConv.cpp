/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file CharEncodingConv.cpp

  \brief A class that deals with character encoding/decoding.
 */

// TerraLib
#include "CharEncodingConv.h"

#if 0 //TE_CHARENCODING_ENABLED
// TerraLib
#include "Exception.h"
#include "Translator.h"

// STL
#include <sstream>

// iconv
#include <errno.h>
#include <iconv.h>

#define TE_CONVERSION_BUFFERSIZE_SIZE 64

te::common::CharEncodingConv::CharEncodingConv(const std::string& fromCode, const std::string& toCode)
  : m_fromCode(fromCode),
    m_toCode(toCode)
{
  m_cd = iconv_open(toCode.c_str(), fromCode.c_str());

  if(m_cd == (iconv_t)(-1))
    throw Exception(TR_COMMON("Failed to start iconv to start converting charsets: maybe the conversion from fromCode to toCode is not supported!"));
}

te::common::CharEncodingConv::~CharEncodingConv()
{
  if(iconv_close(m_cd))
    throw Exception(TR_COMMON("Failed to close iconv! This wasn't supposed to occur! Contact TerraLib Team!"));
}

std::string te::common::CharEncodingConv::conv(const std::string& src)
{
  std::ostringstream outstring(std::ios_base::out);
  const char* inbuff = src.c_str();
  std::size_t inbytesleft = src.length();

  char outchar[TE_CONVERSION_BUFFERSIZE_SIZE];

  std::size_t nbytes = 0;

  for(;;)
  {
    char* outbuff = outchar;
    std::size_t outbytesleft = TE_CONVERSION_BUFFERSIZE_SIZE;

    nbytes = iconv(m_cd, &inbuff, &inbytesleft, &outbuff, &outbytesleft);

    if((nbytes == (std::size_t)(-1)) && (errno != E2BIG))
    {
      nbytes = iconv(m_cd, 0, 0, 0, 0);

      if(nbytes == (std::size_t)(-1))
        throw Exception(TR_COMMON("Failed to convert character sets and also to bring iconv to its initial state!"));
      else
        throw Exception(TR_COMMON("Failed to convert character sets!"));
    }

    outstring.write(outchar, TE_CONVERSION_BUFFERSIZE_SIZE - outbytesleft);

    if(inbytesleft == 0)
      break;
  }

  nbytes = iconv(m_cd, 0, 0, 0, 0);

  if(nbytes == (std::size_t)(-1))
    throw Exception(TR_COMMON("Failed to bring iconv to its initial state!"));

  return outstring.str();
}


std::string te::common::CharEncodingConv::convert(const std::string& src, const std::string& fromCode, const std::string& toCode)
{
  iconv_t cd = iconv_open(toCode.c_str(), fromCode.c_str());

  if(cd == (iconv_t)(-1))
    throw Exception(TR_COMMON("Failed to start iconv to start converting charsets: maybe the conversion from fromCode to toCode is not supported!"));

  std::ostringstream outstring(std::ios_base::out);
  const char* inbuff = src.c_str();
  std::size_t inbytesleft = src.length();

  char outchar[TE_CONVERSION_BUFFERSIZE_SIZE];

  for(;;)
  {
    char* outbuff = outchar;
    std::size_t outbytesleft = TE_CONVERSION_BUFFERSIZE_SIZE;

    std::size_t nbytes = iconv(cd, &inbuff, &inbytesleft, &outbuff, &outbytesleft);

    if((nbytes == (std::size_t)(-1)) && (errno != E2BIG))
    {
      iconv_close(cd);
      throw Exception(TR_COMMON("Failed to convert character sets!"));
    }

    outstring.write(outchar, TE_CONVERSION_BUFFERSIZE_SIZE - outbytesleft);

    if(inbytesleft == 0)
      break;
  }

  if(iconv_close(cd))
    throw Exception(TR_COMMON("Failed to close iconv!"));

  return outstring.str();
}

#endif  // TE_CHARENCODING_ENABLED

