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
  \file terralib/ws/ogc/wcs/client/WCSClient.cpp

  \brief WS Client for OGC WCS

  \author Vinicius Campanha
*/

// LibCURL
#include <curl/curl.h>

// TerraLib
#include "../../../../core/translator/Translator.h"
#include "WCSClient.h"

//STL
#include <iostream>
#include <string>

// Boost
#include <boost/filesystem.hpp>

te::ws::ogc::WCSClient::WCSClient(const std::string usrDataDir, const std::string uri, const std::string version)
  : m_uri(uri + "?SERVICE=WCS"), m_version(version)
{
  m_dataDir = usrDataDir + "/wcs/";

  if (boost::filesystem::is_directory(usrDataDir) && !boost::filesystem::exists(m_dataDir))
    boost::filesystem::create_directories(m_dataDir);
}


te::ws::ogc::WCSClient::~WCSClient()
{
}


void te::ws::ogc::WCSClient::updateCapabilities()
{
  std::string url ("");

  if(m_version == "2.0.1")
  {
    url = m_uri + "&VERSION=" + m_version + "&REQUEST=GetCapabilities";
  }
  else
  {
    throw te::common::Exception(TE_TR("WCS version not supported!"));
  }

  // Request the WCS Capabilities XML file
  std::string xmlPath = te::ws::ogc::WCSClient::makeFileRequest(url, "capabilities.xml");

  // Parse the XML file into a struct
  m_capabilities = parseCapabilities(xmlPath);
}


te::ws::ogc::CoverageDescription te::ws::ogc::WCSClient::describeCoverage(const std::string coverage) const
{

  te::ws::ogc::CoverageDescription describeCoverage;

  std::string url;

  if(m_version == "2.0.1")
  {
    url = m_uri + "&VERSION=" + m_version + "&REQUEST=DescribeCoverage&CoverageID=" + coverage;
  }
  else
  {
    throw te::common::Exception(TE_TR("WCS version not supported!"));
  }

  // Request the WCS Describe Coverage XML file
  std::string xmlPath = te::ws::ogc::WCSClient::makeFileRequest(url, "describeCoverage.xml");

  // Parse the XML file into a struct
  describeCoverage = parseDescribeCoverage(xmlPath);

  return describeCoverage;
}


std::string te::ws::ogc::WCSClient::getCoverage(const CoverageRequest coverageRequest) const
{
  std::string coveragePath;

  std::string url;

  if(m_version == "2.0.1")
  {
    url = m_uri + "&VERSION=" + m_version + "&REQUEST=GetCoverage&COVERAGEID=" + coverageRequest.coverageID;

    if(!coverageRequest.format.empty())
      url += "&FORMAT=" + coverageRequest.format;

    if(!coverageRequest.mediaType.empty())
      url += "&MEDIATYPE=" + coverageRequest.mediaType;

    const EnvelopeWithTimePeriod* envelope = &coverageRequest.envelope;

    std::string x1 = envelope->lowerCorner_X;
    std::string y1 = envelope->lowerCorner_Y;
    std::string x2 = envelope->upperCorner_X;
    std::string y2 = envelope->upperCorner_Y;

    std::string subset1(""), subset2("");

    if(!x1.empty() && !x2.empty())
      subset1 = "&SUBSET=" + envelope->firstLabel + "(" + x1 + (!x1.empty() && !x2.empty()? ",": "") + x2 + ")";

    if(!y1.empty() && !y2.empty())
      subset2 = "&SUBSET=" + envelope->secondLabel + "(" + y1 + (!y1.empty() && !y2.empty()? ",": "") + y2 + ")";

    url += subset1 + subset2;

    if(!envelope->timeLabel.empty())
    {
      url += "&SUBSET=" + envelope->timeLabel + "(";

      if(coverageRequest.time.empty())
        url += envelope->endPosition;
      else
        url += coverageRequest.time;

      url += ")";
    }

    for(std::map<std::string, std::string>::const_iterator parameter = coverageRequest.additionalParameters.begin(); parameter != coverageRequest.additionalParameters.end(); parameter++)
    {
      url += "&" + parameter->first + "=" + parameter->second;
    }
  }
  else
  {
    throw te::common::Exception(TE_TR("WCS version not supported!"));
  }

  coveragePath = makeFileRequest(url, coverageRequest.coverageID);

  return coveragePath;
}


size_t write_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
  size_t sizeRead = size * nmemb;

  std::string* block = (std::string*) data;
  block->append((char *)ptr, sizeRead);

  return sizeRead;
}


std::string te::ws::ogc::WCSClient::makeRequest(const std::string url) const
{
  CURL* curl;
  std::string callback;

  curl = curl_easy_init();
  curl_easy_cleanup(curl);
  curl = curl_easy_init();

  CURLcode status;

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

  // Get data to be written
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  // Set a pointer to our xml string
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&callback);
  /* Perform the request, status will get the return code */
  status = curl_easy_perform(curl);

  // Check for errors
  if(status != CURLE_OK)
    throw te::common::Exception(curl_easy_strerror(status));

  if(curl) curl_easy_cleanup(curl);

  return callback;

  return callback;
}

size_t write_file_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
  FILE *writehere = (FILE *)data;
  return fwrite(ptr, size, nmemb, writehere);
}

std::string te::ws::ogc::WCSClient::makeFileRequest(const std::string url, const std::string fileName) const
{
  CURL* curl;
  std::FILE* file;

  std::string path = m_dataDir + fileName;

  curl = curl_easy_init();
  curl_easy_cleanup(curl);
  curl = curl_easy_init();

  CURLcode status;
  file = std::fopen(path .c_str(), "wb");

  if(!file)
    throw te::common::Exception(TE_TR("Could not open file to write!"));

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

  // Get data to be written
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_file_callback);
  // Set a pointer to our file
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
  /* Perform the request, status will get the return code */
  status = curl_easy_perform(curl);

  // Check for errors
  if(status != CURLE_OK)
    throw te::common::Exception(curl_easy_strerror(status));

  if (!file)
    throw te::common::Exception(TE_TR("Error at received file!"));
  else
    std::fclose(file);

  if(curl) curl_easy_cleanup(curl);

  return path;
}


const struct te::ws::ogc::Capabilities& te::ws::ogc::WCSClient::getCapabilities() const
{
  return m_capabilities;
}
