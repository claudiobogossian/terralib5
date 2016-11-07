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

#include "XMLParser.h"

#include <iostream>

//BOOST
#include <boost/algorithm/string.hpp>

#include "../../../../common/StringUtils.h"
#include "../../../core/Utils.h"

te::ws::ogc::wms::XMLParser::XMLParser()
{

}

te::ws::ogc::wms::XMLParser::~XMLParser()
{

}

te::ws::ogc::wms::WMSCapabilities  te::ws::ogc::wms::XMLParser::parseCapabilities(const std::string &xmlPath)
{
  // Read XML file
  std::auto_ptr<te::xml::Reader> reader(te::xml::ReaderFactory::make("XERCES"));
  reader->setValidationScheme(false);
  reader->setDoSchema(false);
  reader->setIgnoreWhiteSpaces(true);

  reader->read(xmlPath);

  // Validate the XML file
  if(!reader->next())
  {
    throw te::common::Exception(TE_TR("Can not read the Capabilities XML file!"));
  }

  if(!boost::iequals(reader->getElementLocalName(), "WMS_Capabilities"))
  {
    throw te::common::Exception(TE_TR("Can not find Capabilities on the received XML!"));
  }

  WMSCapabilities capabilities;

  while (reader->next())
  {
    if(boost::iequals(reader->getElementLocalName(), "Capability"))
    {
      capabilities.m_capability = this->parseCapability(reader);
    }
  }

  return capabilities;
}

te::ws::ogc::wms::Capability  te::ws::ogc::wms::XMLParser::parseCapability(const std::auto_ptr<te::xml::Reader> &reader)
{
  Capability capability;

  while (reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "Capability")))
  {
    if(boost::iequals(reader->getElementLocalName(), "Request"))
    {
      capability.m_request = this->parseRequest(reader);
    }
    else if (boost::iequals(reader->getElementLocalName(), "Layer"))
    {
      capability.m_layer = this->parseLayers(reader);
    }
  }

  return capability;
}

te::ws::ogc::wms::Request te::ws::ogc::wms::XMLParser::parseRequest(const std::auto_ptr<te::xml::Reader> &reader)
{
  te::ws::ogc::wms::Request request;

  while (reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "Request")))
  {

    if(boost::iequals(reader->getElementLocalName(), "GetCapabilities"))
    {
      request.m_getCapabilities = this->parseOperation(reader, "GetCapabilities");
    }
    else if(boost::iequals(reader->getElementLocalName(), "GetMap"))
    {
      request.m_getMap = this->parseOperation(reader, "GetMap");
    }
    else if(boost::iequals(reader->getElementLocalName(), "GetFeatureInfo"))
    {
      request.m_getFeatureInfo = this->parseOperation(reader, "GetFeatureInfo");
    }
  }

  return request;
}

te::ws::ogc::wms::Layer te::ws::ogc::wms::XMLParser::parseLayers(const std::auto_ptr<te::xml::Reader>& reader)
{
  Layer layer;

  if (reader->hasAttrs())
  {
    for (unsigned int i = 0; i < reader->getNumberOfAttrs(); i++)
    {
      if(boost::iequals(reader->getAttrLocalName(i), "queryable"))
        layer.m_queryable = reader->getAttrAsInt32("queryable");
      else if (boost::iequals(reader->getAttrLocalName(i), "cascaded"))
        layer.m_cascaded = reader->getAttrAsInt32("cascaded");
      else if (boost::iequals(reader->getAttrLocalName(i), "opaque"))
        layer.m_opaque = reader->getAttrAsInt32("opaque");
      else if (boost::iequals(reader->getAttrLocalName(i), "noSubsets"))
        layer.m_noSubsets = reader->getAttrAsInt32("noSubsets");
      else if (boost::iequals(reader->getAttrLocalName(i), "fixedWidth"))
        layer.m_fixedWidth = reader->getAttrAsInt32("fixedWidth");
      else if (boost::iequals(reader->getAttrLocalName(i), "fixedHeight"))
        layer.m_fixedHeight = reader->getAttrAsInt32("fixedHeight");
    }
  }

  std::vector<std::string> crsList;
  while (reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "Layer")))
  {
    if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "Name"))
    {
      layer.m_name = reader->getElementValue();
    }
    else if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "Title"))
    {
      layer.m_title = reader->getElementValue();
    }
    else if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "Abstract"))
    {
      layer.m_abstract = reader->getElementValue();
    }
    else if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "CRS"))
    {
      crsList.push_back(reader->getElementValue());
    }
    else if(reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "KeywordList"))
    {
      std::vector<std::string> keywordList;

      while (reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "KeywordList")))
      {
        if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "Keyword"))
        {
          keywordList.push_back(reader->getElementValue());
        }
      }

      layer.m_keywordList = keywordList;
    }
    else if (reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "EX_GeographicBoundingBox"))
    {
      layer.m_geoBoundingBox = this->parseGeographicBoundingBox(reader);
    }
    else if (reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "BoundingBox") && reader->hasAttrs())
    {
      layer.m_boundingBoxes.push_back(this->parseBoundingBox(reader));
    }
    else if (boost::iequals(reader->getElementLocalName(), "Dimension") && reader->hasAttrs())
    {
      layer.m_dimensions.push_back(this->parseDimension(reader));
    }
    else if (reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "Style"))
    {
      layer.m_styles.push_back(this->parseStyle(reader));
    }
    else if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "MinScaleDenominator"))
    {
      layer.m_minScaleDenominator = reader->getElementValueAsDouble();
    }
    else if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "MaxScaleDenominator"))
    {
      layer.m_maxScaleDenominator = reader->getElementValueAsDouble();
    }
    else if (reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "Layer"))
    {
      Layer subLayer = this->parseLayers(reader);
      layer.m_layers.push_back(subLayer);
    }
  }

  layer.m_crs = crsList;

  return layer;
}

te::ws::ogc::wms::GeographicBoundingBox te::ws::ogc::wms::XMLParser::parseGeographicBoundingBox(const std::auto_ptr<te::xml::Reader> &reader)
{
  GeographicBoundingBox geoBoundingBox;

  while (reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "EX_GeographicBoundingBox")))
  {
    if(reader->getNodeType() == te::xml::VALUE)
    {
      if(boost::iequals(reader->getElementLocalName(), "westBoundLongitude"))
        geoBoundingBox.m_westBoundLongitude = reader->getElementValueAsDouble();
      else if(boost::iequals(reader->getElementLocalName(), "eastBoundLongitude"))
        geoBoundingBox.m_eastBoundLongitude = reader->getElementValueAsDouble();
      else if(boost::iequals(reader->getElementLocalName(), "southBoundLatitude"))
        geoBoundingBox.m_southBoundLatitude = reader->getElementValueAsDouble();
      else if(boost::iequals(reader->getElementLocalName(), "northBoundLatitude"))
        geoBoundingBox.m_northBoundLatitude = reader->getElementValueAsDouble();
    }
  }

  return geoBoundingBox;
}

te::ws::ogc::wms::BoundingBox te::ws::ogc::wms::XMLParser::parseBoundingBox(const std::auto_ptr<te::xml::Reader> &reader)
{
  BoundingBox box;

  box.m_crs = reader->getAttr("CRS");

  bool isInverted = te::ws::core::IsInvertedEPSG(box.m_crs);

  if(isInverted)
  {
    box.m_minX = reader->getAttrAsDouble("miny");
    box.m_minY = reader->getAttrAsDouble("minx");
    box.m_maxX = reader->getAttrAsDouble("maxy");
    box.m_maxY = reader->getAttrAsDouble("maxx");
  }
  else
  {
    box.m_minX = reader->getAttrAsDouble("minx");
    box.m_minY = reader->getAttrAsDouble("miny");
    box.m_maxX = reader->getAttrAsDouble("maxx");
    box.m_maxY = reader->getAttrAsDouble("maxy");
  }

  return box;
}

te::ws::ogc::wms::Dimension te::ws::ogc::wms::XMLParser::parseDimension(const std::auto_ptr<te::xml::Reader> &reader)
{
  Dimension dimension;

  for (unsigned int i = 0; i < reader->getNumberOfAttrs(); i++)
  {
    if(boost::iequals(reader->getAttrLocalName(i), "name"))
      dimension.m_name = reader->getAttr("name");
    else if (boost::iequals(reader->getAttrLocalName(i), "units"))
      dimension.m_units = reader->getAttr("units");
    else if (boost::iequals(reader->getAttrLocalName(i), "unitSymbol"))
      dimension.m_unitSymbol = reader->getAttr("unitSymbol");
    else if (boost::iequals(reader->getAttrLocalName(i), "default"))
      dimension.m_default = reader->getAttr("default");
    else if (boost::iequals(reader->getAttrLocalName(i), "multipleValues"))
      dimension.m_multipleValues = reader->getAttrAsInt32("multipleValues");
    else if (boost::iequals(reader->getAttrLocalName(i), "nearestValue"))
      dimension.m_nearestValue = reader->getAttrAsInt32("nearestValue");
    else if (boost::iequals(reader->getAttrLocalName(i), "current"))
      dimension.m_current = reader->getAttrAsInt32("current");
  }

  if(boost::iequals(dimension.m_name, "time"))
  {
    while (reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "Dimension")))
    {
      if(reader->getNodeType() == te::xml::VALUE)
      {
        std::string allowedValuesStr = reader->getElementValue();

        std::vector<std::string> allowedValues = te::common::SplitString(allowedValuesStr, ',');

        dimension.m_allowedValues = allowedValues;
      }
    }
  }

  return dimension;
}

te::ws::ogc::wms::Style te::ws::ogc::wms::XMLParser::parseStyle(const std::auto_ptr<te::xml::Reader> &reader)
{
  Style style;

  while (reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "Style")))
  {
    if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "Name"))
    {
      style.m_name = reader->getElementValue();
    }
    else if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "Title"))
    {
      style.m_title = reader->getElementValue();
    }
    else if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "Abstract"))
    {
      style.m_abstract = reader->getElementValue();
    }
    else if(reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "LegendURL"))
    {
      style.m_legendURL = this->parseLegendURL(reader);
    }
    else if(reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "StyleSheetURL"))
    {
      style.m_styleSheetURL = this->parseStyleSheetURL(reader);
    }
    else if(reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "StyleURL"))
    {
      style.m_styleURL = this->parseStyleURL(reader);
    }
  }

  return style;
}

te::ws::ogc::wms::LegendURL te::ws::ogc::wms::XMLParser::parseLegendURL(const std::auto_ptr<te::xml::Reader> &reader)
{
  LegendURL legendURL;

  if (reader->hasAttrs())
  {
    for (unsigned int i = 0; i < reader->getNumberOfAttrs(); i++)
    {
      if (boost::iequals(reader->getAttrLocalName(i), "width"))
        legendURL.m_width = reader->getAttrAsInt32("width");
      else if (boost::iequals(reader->getAttrLocalName(i), "height"))
        legendURL.m_height = reader->getAttrAsInt32("height");
    }
  }

  while (reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "LegendURL")))
  {
    if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "Format"))
    {
      legendURL.m_format = reader->getElementValue();
    }
    else if(boost::iequals(reader->getElementLocalName(), "OnlineResource") && reader->hasAttrs())
    {
      legendURL.m_onlineResource = this->parseOnlineResource(reader);
    }
  }

  return legendURL;
}

te::ws::ogc::wms::OnlineResource te::ws::ogc::wms::XMLParser::parseOnlineResource(const std::auto_ptr<te::xml::Reader> &reader)
{
  OnlineResource onlineResource;

  for (unsigned int i = 0; i < reader->getNumberOfAttrs(); i++)
  {
    if(boost::iequals(reader->getAttrLocalName(i), "href"))
      onlineResource.m_href = reader->getAttr(i);
    else if (boost::iequals(reader->getAttrLocalName(i), "type"))
      onlineResource.m_type = reader->getAttr(i);
  }

  return onlineResource;
}

te::ws::ogc::wms::StyleSheetURL te::ws::ogc::wms::XMLParser::parseStyleSheetURL(const std::auto_ptr<te::xml::Reader> &reader)
{
  StyleSheetURL styleSheet;

  while (reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "StyleSheetURL")))
  {
    if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "Format"))
    {
      styleSheet.m_format = reader->getElementValue();
    }
    else if(boost::iequals(reader->getElementLocalName(), "OnlineResource") && reader->hasAttrs())
    {
      styleSheet.m_onlineResource = this->parseOnlineResource(reader);
    }
  }

  return styleSheet;
}

te::ws::ogc::wms::StyleURL te::ws::ogc::wms::XMLParser::parseStyleURL(const std::auto_ptr<te::xml::Reader> &reader)
{
  StyleURL style;

  while (reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "StyleURL")))
  {
    if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "Format"))
    {
      style.m_format = reader->getElementValue();
    }
    else if(boost::iequals(reader->getElementLocalName(), "OnlineResource") && reader->hasAttrs())
    {
      style.m_onlineResource = this->parseOnlineResource(reader);
    }
  }

  return style;
}

te::ws::ogc::wms::Operation te::ws::ogc::wms::XMLParser::parseOperation(const std::auto_ptr<te::xml::Reader> &reader, const std::string& operationName)
{
  Operation operation;

  while(reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), operationName)))
  {
    if(reader->getNodeType() == te::xml::VALUE && boost::iequals(reader->getElementLocalName(), "Format"))
    {
      operation.m_formats.push_back(reader->getElementValue());
    }
    else if (reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "DCPType"))
    {
      while(reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "DCPType")))
      {
        if (reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "HTTP"))
        {
          operation.m_dcps.push_back(this->parseHTTP(reader));
        }
      }
    }
  }

  return operation;
}

te::ws::ogc::wms::HTTP te::ws::ogc::wms::XMLParser::parseHTTP(const std::auto_ptr<te::xml::Reader> &reader)
{
  HTTP http;

  while(reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "HTTP")))
  {
    if (reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "Get"))
    {
      while(reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "Get")))
      {
        if (reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "OnlineResource"))
        {
          http.m_get = this->parseOnlineResource(reader);
        }
      }
    }
    else if (reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "Post"))
    {
      while(reader->next() && !(reader->getNodeType() == te::xml::END_ELEMENT && boost::iequals(reader->getElementLocalName(), "Post")))
      {
        if (reader->getNodeType() == te::xml::START_ELEMENT && boost::iequals(reader->getElementLocalName(), "OnlineResource"))
        {
          http.m_post = this->parseOnlineResource(reader);
        }
      }
    }
  }

  return http;
}
