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
#include "../../../core/CurlWrapper.h"
#include "WCSClient.h"

//STL
#include <iostream>
#include <string>

// Boost
#include <boost/filesystem.hpp>

te::ws::ogc::WCSClient::WCSClient(const std::string usrDataDir, const std::string uri, const std::string version)
  : m_version(version),
    m_uri (uri)
{
  m_dataDir = usrDataDir + "/wcs/";

  std::string baseUrl = m_uri.scheme() + "://" + m_uri.host();

  if (!m_uri.port().empty())
  {
    baseUrl = baseUrl + ":" + m_uri.port();
  }

  baseUrl = baseUrl + m_uri.path() + "?";

  if (!m_uri.query().empty())
  {
    std::string query = m_uri.query();

    size_t endsWith = query.rfind("&");

    if (endsWith != (query.size() - 1))
    {
      query.append("&");
    }

    baseUrl = baseUrl + query;
  }

  m_uri = te::core::URI(baseUrl);

  m_curl = std::shared_ptr<te::ws::core::CurlWrapper>(new te::ws::core::CurlWrapper());

  if (boost::filesystem::is_directory(usrDataDir) && !boost::filesystem::exists(m_dataDir))
    boost::filesystem::create_directories(m_dataDir);
}


te::ws::ogc::WCSClient::~WCSClient()
{
}


void te::ws::ogc::WCSClient::updateCapabilities()
{
  std::string url = m_uri.uri();

  if(m_version == "2.0.1")
  {
    url = url + "SERVICE=WCS" + "&VERSION=" + m_version + "&REQUEST=GetCapabilities";
  }
  else
  {
    throw te::common::Exception(TE_TR("WCS version not supported!"));
  }

  m_curl->setTaskMessage(TE_TR("Getting Capabilities"));

  // Request the WCS Capabilities XML file
  std::string xmlPath = te::ws::ogc::WCSClient::makeFileRequest(url, "capabilities.xml");

  // Parse the XML file into a struct
  m_capabilities = m_parser.parseCapabilities(xmlPath);
}


te::ws::ogc::wcs::CoverageDescription te::ws::ogc::WCSClient::describeCoverage(const std::string coverage)
{

  if(m_descriptionMap.find(coverage) != m_descriptionMap.end())
  {
    return m_descriptionMap[coverage];
  }

  te::ws::ogc::wcs::CoverageDescription describeCoverage;

  std::string url = m_uri.uri();

  if(m_version == "2.0.1")
  {
    url = url + "SERVICE=WCS" + "&VERSION=" + m_version + "&REQUEST=DescribeCoverage&CoverageID=" + coverage;
  }
  else
  {
    throw te::common::Exception(TE_TR("WCS version not supported!"));
  }

  m_curl->setTaskMessage(TE_TR("Getting Coverage Description"));

  // Request the WCS Describe Coverage XML file
  std::string xmlPath = te::ws::ogc::WCSClient::makeFileRequest(url, "describeCoverage.xml");

  // Parse the XML file into a struct
  describeCoverage = m_parser.parseDescribeCoverage(xmlPath);

  m_descriptionMap[coverage] = describeCoverage;

  return describeCoverage;
}


std::string te::ws::ogc::WCSClient::getCoverage(const te::ws::ogc::wcs::CoverageRequest coverageRequest) const
{
  std::string coveragePath;

  std::string url = m_uri.uri();

  if(m_version == "2.0.1")
  {
    url = url + "SERVICE=WCS" + "&VERSION=" + m_version + "&REQUEST=GetCoverage&COVERAGEID=" + coverageRequest.coverageID;

    if(!coverageRequest.format.empty())
      url += "&FORMAT=" + coverageRequest.format;

    if(!coverageRequest.mediaType.empty())
      url += "&MEDIATYPE=" + coverageRequest.mediaType;

    const te::ws::ogc::wcs::EnvelopeWithTimePeriod* envelope = &coverageRequest.envelope;

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

  m_curl->setTaskMessage(TE_TR("Getting Coverage"));

  coveragePath = makeFileRequest(url, coverageRequest.coverageID);

  return coveragePath;
}

std::string te::ws::ogc::WCSClient::makeFileRequest(const std::string url, const std::string fileName) const
{
  std::string path = m_dataDir + fileName;

  m_curl->downloadFile(url, path);

  return path;
}

const te::ws::ogc::wcs::Capabilities& te::ws::ogc::WCSClient::getCapabilities() const
{
  return m_capabilities;
}

void te::ws::ogc::WCSClient::setCurlWrapper(te::ws::core::CurlWrapper* curlWrapper)
{
  m_curl.reset(curlWrapper);
}
