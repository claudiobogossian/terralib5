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
  \file terralib/ws/ogc/wcs/client/WCS.cpp

  \brief WS Client for OGC WCS

  \author Vinicius Campanha
*/

// LibCURL
#include <curl/curl.h>

// TerraLib
#include "../../../../common/Translator.h"
#include "WCS.h"
#include <iostream>

te::ws::ogc::WCS::WCS(const std::string uri, const std::string version)
  : uri_(uri + "?SERVICE=WCS"), version_(version)
{

}


te::ws::ogc::WCS::~WCS()
{

}


void te::ws::ogc::WCS::updateCapabilities()
{
  try
  {
    std::string url;

    if(version_ == "2.0.1")
    {
      url = uri_ + "&VERSION=" + version_ + "&REQUEST=GetCapabilities";
    }
    else
    {
      throw te::common::Exception(TE_TR("WCS version not supported!"));
    }

    // Request the WCS Capabilities XML file
    std::string xmlPath = te::ws::ogc::WCS::makeFileRequest(url, "capabilities.xml");

    // Parse the XML file into a struct
    capabilities_ = parseCapabilities(xmlPath);
  }
  catch(const te::common::Exception& e)
  {
    QString messageError = TE_TR("Error at request! \n Details: \n");
    messageError.append(e.what());
    throw te::common::Exception(messageError.toStdString());
  }
  catch(const std::exception& e)
  {
    QString messageError = TE_TR("Error at request! \n Details: \n");
    messageError.append(e.what());
    throw te::common::Exception(messageError.toStdString());
  }
  catch(...)
  {
    throw te::common::Exception(TE_TR("Unknow error! \n"));
  }
}


te::ws::ogc::CoverageDescription te::ws::ogc::WCS::describeCoverage(const std::string coverage) const
{
  te::ws::ogc::CoverageDescription describeCoverage;

  try
  {
    std::string url;

    if(version_ == "2.0.1")
    {
      url = uri_ + "&VERSION=" + version_ + "&REQUEST=DescribeCoverage&CoverageID=" + coverage;
    }
    else
    {
      throw te::common::Exception(TE_TR("WCS version not supported!"));
    }

    // Request the WCS Describe Coverage XML file
    std::string xmlPath = te::ws::ogc::WCS::makeFileRequest(url, "describeCoverage.xml");

    // Parse the XML file into a struct
    describeCoverage = parseDescribeCoverage(xmlPath);
  }
  catch(const te::common::Exception& e)
  {
    QString messageError = TE_TR("Error at request! \n Details: \n");
    messageError.append(e.what());
    throw te::common::Exception(messageError.toStdString());
  }
  catch(const std::exception& e)
  {
    QString messageError = TE_TR("Error at request! \n Details: \n");
    messageError.append(e.what());
    throw te::common::Exception(messageError.toStdString());
  }
  catch(...)
  {
    throw te::common::Exception(TE_TR("Unknow error! \n"));
  }

  return describeCoverage;
}


std::string te::ws::ogc::WCS::getCoverage(const CoverageRequest coverageRequest) const
{
  std::string coveragePath;

  try
  {
    std::string url;

    if(version_ == "2.0.1")
    {
      url = uri_ + "&VERSION=" + version_ + "&REQUEST=GetCoverage&COVERAGEID=" + coverageRequest.coverageID;

      if(!coverageRequest.format.empty())
        url += "&FORMAT=" + coverageRequest.format;

      if(!coverageRequest.mediaType.empty())
        url += "&MEDIATYPE=" + coverageRequest.mediaType;

      for(unsigned int i = 0; i < coverageRequest.boundedBy.axisLabels.size(); i ++)
      {
        const BoundedBy* boundedBy = &coverageRequest.boundedBy;

        url += "&SUBSET=" + boundedBy->axisLabels.at(i) + "(";

        for(unsigned int j = 0; j < boundedBy->coordinate.size(); j++)
        {
          const Coordinate* coordinate = &boundedBy->coordinate.at(j);

          if(j != 0)
            url += ",";

          url += coordinate->coord.at(i);
        }

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
    std::cout << url.c_str() << std::endl;
    coveragePath = makeFileRequest(url, coverageRequest.coverageID);

  }
  catch(const te::common::Exception& e)
  {
    QString messageError = TE_TR("Error at get Coverage! \n Details: \n");
    messageError.append(e.what());
    throw te::common::Exception(messageError.toStdString());
  }
  catch(const std::exception& e)
  {
    QString messageError = TE_TR("Error get Coverage! \n Details: \n");
    messageError.append(e.what());
    throw te::common::Exception(messageError.toStdString());
  }
  catch(...)
  {
    throw te::common::Exception(TE_TR("Unknow error! \n"));
  }

  return coveragePath;
}


size_t write_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
  size_t sizeRead = size * nmemb;

  std::string* block = (std::string*) data;
  block->append((char *)ptr, sizeRead);

  return sizeRead;
}


std::string te::ws::ogc::WCS::makeRequest(const std::string url) const
{
  CURL* curl;
  std::string callback;

  try
  {
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
  }
  catch(const te::common::Exception& e)
  {
    if(curl) curl_easy_cleanup(curl);
    QString messageError = TE_TR("Error at request! \n Details: \n");
    messageError.append(e.what());
    throw te::common::Exception(messageError.toStdString());
  }
  catch(const std::exception& e)
  {
    if(curl) curl_easy_cleanup(curl);
    QString messageError = TE_TR("Error at request! \n Details: \n");
    messageError.append(e.what());
    throw te::common::Exception(messageError.toStdString());
  }
  catch(...)
  {
    if(curl) curl_easy_cleanup(curl);
    throw te::common::Exception(TE_TR("Unknow error! \n"));
  }

  return callback;
}

size_t write_file_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
  FILE *writehere = (FILE *)data;
  return fwrite(ptr, size, nmemb, writehere);
}

std::string te::ws::ogc::WCS::makeFileRequest(const std::string url, const std::string fileName) const
{
  CURL* curl;
  std::FILE* file;
  // VINICIUS: work with temp directory and filename
  std::string path = "/tmp/" + fileName;

  try
  {
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
  }
  catch(const te::common::Exception& e)
  {
    if(curl) curl_easy_cleanup(curl);
    if (file) std::fclose(file);
    QString messageError = TE_TR("Error at request! \n Details: \n");
    messageError.append(e.what());
    throw te::common::Exception(messageError.toStdString());
  }
  catch(const std::exception& e)
  {
    if(curl) curl_easy_cleanup(curl);
    if (file) std::fclose(file);
    QString messageError = TE_TR("Error at request! \n Details: \n");
    messageError.append(e.what());
    throw te::common::Exception(messageError.toStdString());
  }
  catch(...)
  {
    if(curl) curl_easy_cleanup(curl);
    if (file) std::fclose(file);
    throw te::common::Exception(TE_TR("Unknow error! \n"));
  }

  return path;
}


const struct te::ws::ogc::Capabilities& te::ws::ogc::WCS::getCapabilities() const
{
  return capabilities_;
}
