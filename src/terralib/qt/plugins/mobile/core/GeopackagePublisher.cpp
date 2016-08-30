/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
\file src/terraMobilePlugin/core/GeoPackagePublisher.cpp

\brief This file is used to Publisher operation.
*/

//TerraLib
#include "GeopackagePublisher.h"
#include "../../../../core/encoding/CharEncoding.h"

// libcurl
#include <curl/curl.h>

// Boost
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::qt::plugins::terramobile::GeopackagePublisher* te::qt::plugins::terramobile::GeopackagePublisher::m_staticPointer = 0;

te::qt::plugins::terramobile::GeopackagePublisherOperation te::qt::plugins::terramobile::GeopackagePublisher::m_gpkgOpType = te::qt::plugins::terramobile::GPKG_OP_NONE;

int progressFunc(void *bar,  double donwTotal, double downNow, double upTotal, double upNow)
{
  if (te::qt::plugins::terramobile::GeopackagePublisher::m_gpkgOpType == te::qt::plugins::terramobile::GPKG_OP_DOWNLOAD)
  {
    te::qt::plugins::terramobile::GeopackagePublisher::m_staticPointer->emitSignal(downNow, donwTotal);
  }
  else if (te::qt::plugins::terramobile::GeopackagePublisher::m_gpkgOpType == te::qt::plugins::terramobile::GPKG_OP_UPLOAD)
  {
    te::qt::plugins::terramobile::GeopackagePublisher::m_staticPointer->emitSignal(upNow, upTotal);
  }

  return 0;
}

te::qt::plugins::terramobile::GeopackagePublisher::GeopackagePublisher() : m_errorMessage("")
{
  m_staticPointer = this;
}

te::qt::plugins::terramobile::GeopackagePublisher::~GeopackagePublisher()
{

}

std::string te::qt::plugins::terramobile::GeopackagePublisher::getErrorMessage()
{
  return m_errorMessage;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

te::qt::plugins::terramobile::GeopackageFiles te::qt::plugins::terramobile::GeopackagePublisher::getGeopackageFiles(std::string url)
{
  url += "/listprojects";

  CURL *curl;
  CURLcode res;

  struct curl_httppost *formpost = NULL;
  struct curl_httppost *lastptr = NULL;
  struct curl_slist *headerlist = NULL;
  static const char buf[] = "Expect:";

  curl_global_init(CURL_GLOBAL_ALL);

  /* Fill in the file upload field */
  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "user",
    CURLFORM_COPYCONTENTS, "userName",
    CURLFORM_END);

  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "password",
    CURLFORM_COPYCONTENTS, "password",
    CURLFORM_END);

  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "projectStatus",
    CURLFORM_COPYCONTENTS, "1",
    CURLFORM_END);

  std::string readBuffer;

  curl = curl_easy_init();

  headerlist = curl_slist_append(headerlist, buf);

  if (curl) 
  {
    /* what URL that receives this POST */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);

    /* Check for errors */
    if (res != CURLE_OK)
    {
      m_errorMessage = curl_easy_strerror(res);
      throw;
    }

    /* always cleanup */
    curl_easy_cleanup(curl);

    /* then cleanup the formpost chain */
    curl_formfree(formpost);
    /* free slist */
    curl_slist_free_all(headerlist);
  }

  return readJSONInfo(readBuffer);
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  size_t written;
  written = fwrite(ptr, size, nmemb, stream);
  return written;
}

void te::qt::plugins::terramobile::GeopackagePublisher::downloadGeopackageFile(std::string url, GeopackageFile gpkgFile, std::string pathDir)
{
  m_gpkgOpType = te::qt::plugins::terramobile::GPKG_OP_DOWNLOAD;

  url += "/downloadproject";

  CURL *curl;
  CURLcode res;

  FILE *fp;

  std::string outfilename = pathDir + "/" + gpkgFile.m_name;

  struct curl_httppost *formpost = NULL;
  struct curl_httppost *lastptr = NULL;
  struct curl_slist *headerlist = NULL;
  static const char buf[] = "Expect:";

  curl_global_init(CURL_GLOBAL_ALL);

  /* Fill in the file upload field */
  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "user",
    CURLFORM_COPYCONTENTS, "userName",
    CURLFORM_END);

  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "password",
    CURLFORM_COPYCONTENTS, "password",
    CURLFORM_END);

  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "projectId",
    CURLFORM_COPYCONTENTS, gpkgFile.m_objId.c_str(),
    CURLFORM_END);

  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "projectStatus",
    CURLFORM_COPYCONTENTS, "1",
    CURLFORM_END);

  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "projectName",
    CURLFORM_COPYCONTENTS, gpkgFile.m_name.c_str() ,
    CURLFORM_END);


  curl = curl_easy_init();

  headerlist = curl_slist_append(headerlist, buf);

  if (curl) 
  {
    fp = fopen(te::core::CharEncoding::fromUTF8(outfilename).c_str(), "wb");

    /* what URL that receives this POST */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progressFunc);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);

    /* Check for errors */
    if (res != CURLE_OK)
    {
      m_errorMessage = curl_easy_strerror(res);
      m_gpkgOpType = te::qt::plugins::terramobile::GPKG_OP_NONE;
      throw;
    }

    /* always cleanup */
    curl_easy_cleanup(curl);

    /* then cleanup the formpost chain */
    curl_formfree(formpost);

    /* free slist */
    curl_slist_free_all(headerlist);

    fclose(fp);
  }

  m_gpkgOpType = te::qt::plugins::terramobile::GPKG_OP_NONE;
}

void te::qt::plugins::terramobile::GeopackagePublisher::uploadGeopackageFile(std::string url, std::string filePath, std::string fileName)
{
  m_gpkgOpType = te::qt::plugins::terramobile::GPKG_OP_UPLOAD;

  url += "/uploadproject";

  CURL *curl;
  CURLcode res;

  struct curl_httppost *formpost = NULL;
  struct curl_httppost *lastptr = NULL;
  struct curl_slist *headerlist = NULL;
  static const char buf[] = "Expect:";

  curl_global_init(CURL_GLOBAL_ALL);

  /* Fill in the file upload field */
  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "user",
    CURLFORM_COPYCONTENTS, "userName",
    CURLFORM_END);

  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "password",
    CURLFORM_COPYCONTENTS, "password",
    CURLFORM_END);

  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "fileName",
    CURLFORM_COPYCONTENTS, fileName.c_str(),
    CURLFORM_END);

  curl_formadd(&formpost,
    &lastptr,
    CURLFORM_COPYNAME, "file",
    CURLFORM_FILE, filePath.c_str(),
    CURLFORM_END);

  curl = curl_easy_init();

  headerlist = curl_slist_append(headerlist, buf);

  if (curl) 
  {
    /* what URL that receives this POST */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progressFunc);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);

    /* Check for errors */
    if (res != CURLE_OK)
    {
      m_errorMessage = curl_easy_strerror(res);
      m_gpkgOpType = te::qt::plugins::terramobile::GPKG_OP_NONE;
      throw;
    }

    /* always cleanup */
    curl_easy_cleanup(curl);

    /* then cleanup the formpost chain */
    curl_formfree(formpost);

    /* free slist */
    curl_slist_free_all(headerlist);
  }

  m_gpkgOpType = te::qt::plugins::terramobile::GPKG_OP_NONE;
}

te::qt::plugins::terramobile::GeopackageFiles te::qt::plugins::terramobile::GeopackagePublisher::readJSONInfo(std::string stream)
{
  std::stringstream ss(stream);

  te::qt::plugins::terramobile::GeopackageFiles gpkgFiles;

  try
  {
    boost::property_tree::ptree pt;
    boost::property_tree::json_parser::read_json(ss, pt);

    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("projects"))
    {
      te::qt::plugins::terramobile::GeopackageFile gpkgFile;

      gpkgFile.m_name = v.second.get<std::string>("project_name");
      gpkgFile.m_status = v.second.get<std::string>("project_status");
      gpkgFile.m_objId = v.second.get<std::string>("project_id");
      gpkgFile.m_desc = v.second.get<std::string>("project_description");

      gpkgFiles.push_back(gpkgFile);
    }
  }
  catch (boost::property_tree::json_parser::json_parser_error &je)
  {
    m_errorMessage = je.message();
  }
  catch (std::exception const& e)
  {
    m_errorMessage = e.what();
  }

  return gpkgFiles;
}

void te::qt::plugins::terramobile::GeopackagePublisher::emitSignal(double curStep, double totalStep)
{
  std::string msg = "";

  if (m_gpkgOpType == te::qt::plugins::terramobile::GPKG_OP_DOWNLOAD)
  {
    msg = "Downloading Geopackage File...";
  }
  else if (m_gpkgOpType == te::qt::plugins::terramobile::GPKG_OP_UPLOAD)
  {
    msg = "Uploading Geopackage File...";
  }

  emit setCurrentStep(curStep, totalStep, msg);
}
