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
  \file terralib/ws/core/CurlWrapper.cpp

  \brief A Wrapper for Lib Curl.

  \author Emerson Moraes
 */

#include "CurlWrapper.h"

// Terralib
#include "../../../terralib/common/Exception.h"
#include "../../../terralib/core/translator/Translator.h"

// LibCurl
#include <curl/curl.h>

te::ws::core::CurlWrapper::CurlWrapper()
{
  m_curl = std::shared_ptr<CURL>(curl_easy_init(), curl_easy_cleanup);
}

te::ws::core::CurlWrapper::~CurlWrapper()
{
}

size_t write_file_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
  std::FILE *writehere = (std::FILE *)data;
  return fwrite(ptr, size, nmemb, writehere);
}

void te::ws::core::CurlWrapper::downloadFile(const std::string& url, const std::string& filePath) const
{

  std::FILE* file = std::fopen(filePath.c_str(), "wb");

  if(!file)
    throw te::common::Exception(TE_TR("Could not open file to write!"));

  downloadFile(url, file);

  if (!file)
    throw te::common::Exception(TE_TR("Error at received file!"));
  else
    std::fclose(file);
}

void te::ws::core::CurlWrapper::downloadFile(const std::string &url, std::FILE* file) const
{
  curl_easy_reset(m_curl.get());

  curl_easy_setopt(m_curl.get(), CURLOPT_URL, url.c_str());

  // Get data to be written
  curl_easy_setopt(m_curl.get(), CURLOPT_WRITEFUNCTION, write_file_callback);

  // Set a pointer to our file
  curl_easy_setopt(m_curl.get(), CURLOPT_WRITEDATA, file);

  /* Perform the request, status will get the return code */
  CURLcode status = curl_easy_perform(m_curl.get());

  // Check for errors
  if(status != CURLE_OK)
    throw te::common::Exception(curl_easy_strerror(status));
}
