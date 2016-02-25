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
  \file terralib/ws/ogc/wcs-client/ClientWCS.cpp

  \brief WS Client for OGC WCS
*/

// LibCURL
#include <curl/curl.h>

// TerraLib
#include "ClientWCS.h"
#include "../../../common/Translator.h"


te::ws::ogc::ClientWCS::ClientWCS(const std::string uri, const std::string version)
  : uri_(uri + "?SERVICE=WCS"), version_(version)
{

}


te::ws::ogc::ClientWCS::~ClientWCS()
{

}


size_t write_xml_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
  size_t sizeRead = size * nmemb;

  std::string* block = (std::string*) data;
  block->append((char *)ptr, sizeRead);

  return sizeRead;
}


QXmlStreamReader* te::ws::ogc::ClientWCS::getCapabilities()
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

    QXmlStreamReader* xml_stream_ptr = te::ws::ogc::ClientWCS::makeRequest(url);

    if(!xml_stream_ptr)
      throw te::common::Exception(TE_TR("Error to receive XML!"));

    // Validate the xml response
    if(!xml_stream_ptr->readNextStartElement())
    {
      if(xml_stream_ptr)
        delete xml_stream_ptr;
      throw te::common::Exception(TE_TR("Error on received XML!"));
    }

    if(xml_stream_ptr->name().compare("Capabilities") != 0)
    {
      if(xml_stream_ptr)
        delete xml_stream_ptr;
      throw te::common::Exception(TE_TR("Can not find Capabilities on the received XML!"));
    }

    return xml_stream_ptr;
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
    throw messageError.toStdString();
  }
  catch(...)
  {
    throw TE_TR("Unknow error! \n");
  }

  return NULL;
}


QXmlStreamReader* te::ws::ogc::ClientWCS::describeCoverage(const std::string coverage) const
{
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

    QXmlStreamReader* xml_stream_ptr = te::ws::ogc::ClientWCS::makeRequest(url);

    // Validate the xml response
    if(!xml_stream_ptr->readNextStartElement())
    {
      if(xml_stream_ptr)
        delete xml_stream_ptr;
      throw te::common::Exception(TE_TR("Error on received XML!"));
    }

    if(xml_stream_ptr->name().compare("CoverageDescriptions") != 0)
    {
      if(xml_stream_ptr)
        delete xml_stream_ptr;
      throw te::common::Exception(TE_TR("Can not find Coverage Descriptions on the received XML!"));
    }

    return xml_stream_ptr;
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
    throw messageError.toStdString();
  }
  catch(...)
  {
    throw TE_TR("Unknow error! \n");
  }

  return NULL;
}


te::da::DataSet* te::ws::ogc::ClientWCS::getCoverage(const CoverageRequest coverage) const
{
  try
  {
    std::string url;

    if(version_ == "2.0.1")
    {
      url = "WMS:" + uri_ + "&VERSION=" + version_ + "&REQUEST=GetCoverage&COVERAGEID=" + coverage.coverageID;

      if(!coverage.format.empty())
        url += "&FORMAT=" + coverage.format;

      if(!coverage.mediaType.empty())
        url += "&MEDIATYPE=" + coverage.mediaType;

      for(std::map<std::string, std::string>::const_iterator subset = coverage.subset.begin(); subset != coverage.subset.end(); subset++)
      {
        url += "&SUBSET=" + subset->first + "(" + subset->second + ")";
      }

      for(std::map<std::string, std::string>::const_iterator parameter = coverage.parameters.begin(); parameter != coverage.parameters.end(); parameter++)
      {
        url += "&" + parameter->first + "=" + parameter->second;
      }
    }
    else
    {
      throw te::common::Exception(TE_TR("WCS version not supported!"));
    }

    // VINICIUS: make CURL request
    // VINICIUS: make a TerraLib Dataset

  }
  catch(const te::common::Exception& e)
  {
    QString messageError = TE_TR("Error at request! \n Details: \n");
    messageError.append(e.what());
    throw messageError.toStdString();
  }
  catch(const std::exception& e)
  {
    QString messageError = TE_TR("Error at request! \n Details: \n");
    messageError.append(e.what());
    throw messageError.toStdString();
  }
  catch(...)
  {
    throw TE_TR("Unknow error! \n");
  }

  return NULL;
}


QXmlStreamReader* te::ws::ogc::ClientWCS::makeRequest(std::string url) const
{
  try
  {
    CURL* curl;
    curl = curl_easy_init();
    curl_easy_cleanup(curl);
    curl = curl_easy_init();

    CURLcode status;
    std::string xml;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Get data to be written
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_xml_callback);
    // Set a pointer to our xml string
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&xml);
    /* Perform the request, status will get the return code */
    status = curl_easy_perform(curl);

    // Check for errors
    if(status != CURLE_OK)
      throw te::common::Exception(curl_easy_strerror(status));

    QXmlStreamReader* xml_stream_ptr = new QXmlStreamReader(xml.c_str());

    // Check the xml response
    if(xml_stream_ptr->hasError())
    {
      if(xml_stream_ptr)
        delete xml_stream_ptr;
      throw te::common::Exception(TE_TR("Error on received XML!"));
    }

    return xml_stream_ptr;
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
    throw messageError.toStdString();
  }
  catch(...)
  {
    throw TE_TR("Unknow error! \n");
  }

  return NULL;
}
