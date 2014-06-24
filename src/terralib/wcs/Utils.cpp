/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/wcs/Utils.cpp
   
  \brief Utility functions for WCS driver.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "Config.h"
#include "Utils.h"
#include "WCSLayerInfo.h"

// Boost
#include <boost/lexical_cast.hpp>

// STL
#include <cassert>

void te::wcs::BuildLayersInfo(char** subdatasets, std::map<std::string, WCSLayerInfo>& info)
{
  if(subdatasets == 0)
    return;

  for(char** i = subdatasets; *i != 0; i = i + 2)
  {
    std::string request = std::string(*i);     // Pseudo-GetMap request. e.g. "SUBDATASET_1_NAME=WCS:http://..."
    std::string title = std::string(*(i + 1)); // User-friendly layer name. e.g. "SUBDATASET_1_DESC=Seritinga-MG"

    // Removing the key of GDAL metadata
    std::size_t pos = request.find("=");
    assert(pos != std::string::npos);
    request = request.substr(pos + 1);

    // Removing the key of GDAL metadata
    pos = title.find("=");
    assert(pos != std::string::npos);
    title = title.substr(pos + 1);

    std::map<std::string, std::string> kvps;
    te::common::ExtractKVP(request, kvps, "&", "=", true);

    // Builds the informations from the current layer
    WCSLayerInfo currentLayerInfo = BuildLayerInfo(kvps);
    //currentLayerInfo.m_title = title;
    //currentLayerInfo.m_pseudoGetMapRequest = request;

    // Adding the informations
    info[currentLayerInfo.m_name] = currentLayerInfo;
  }
}

te::wcs::WCSLayerInfo te::wcs::BuildLayerInfo(const std::map<std::string, std::string>& kvp)
{
  WCSLayerInfo info;

  assert(kvp.find("LAYERS") != kvp.end());
  info.m_name = kvp.find("LAYERS")->second;

  return info;
}

void te::wcs::ExtractRequestValues(const std::string& request, std::string& layer)
{
  assert(!request.empty());

  std::map<std::string, std::string> kvp;
  te::common::ExtractKVP(request, kvp, "&", "=", false);

  // Layers
  std::map<std::string, std::string>::const_iterator it = kvp.find("LAYER");
  if(it != kvp.end())
    layer = it->second;
  else
  {
    // Here assumes that is a simple request by name
    layer = request;
    return;
  }
}

std::string te::wcs::BuildRequest(const std::string& serverUrl, const WCSLayerInfo& info)
{
  assert(!serverUrl.empty());

  // Remove "WCS:" from given server url, if exists
  std::string url = serverUrl;
  if(url.find("WCS:") != std::string::npos)
  {
    std::size_t pos = serverUrl.find(":");
    url = url.substr(pos + 1);
  }

  // Build the request using the GDAL format. Details: http://www.gdal.org/frmt_wcs.html
  std::string request = "<WCS_GDAL>";

  // Service
  request += "<ServiceURL>" + serverUrl + "</ServiceURL>";

  // CoverageName
  request += "<CoverageName>" + info.m_name + "</CoverageName>";
  
  request += "</WCS_GDAL>";

  return request;
}
