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
  \file terralib/plugin/PluginInfo.cpp

  \brief The basic information about a plugin.
*/

// TerraLib
#include "../xml/Reader.h"
#include "../xml/ReaderFactory.h"
#include "PluginInfo.h"

// STL
#include <cassert>
#include <cstdlib>

te::plugin::PluginInfo& operator<<(te::plugin::PluginInfo& pInfo, te::xml::Reader& xmlReader)
{
  assert(xmlReader.getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader.getElementLocalName() == "PluginInfo");

  if(xmlReader.hasAttrs())
  {
    pInfo.m_engine = xmlReader.getAttr("engine");
    pInfo.m_release = xmlReader.getAttr("release");
    pInfo.m_version = xmlReader.getAttr("version");
  }

// Name
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader.getElementLocalName() == "Name");

  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::VALUE);
  pInfo.m_name = xmlReader.getElementValue();
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::END_ELEMENT); // End of Name element

// DisplayName
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader.getElementLocalName() == "DisplayName");

  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::VALUE);
  pInfo.m_displayName = xmlReader.getElementValue();
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::END_ELEMENT); // End of DisplayName element

// Description
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader.getElementLocalName() == "Description");

  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::VALUE);
  pInfo.m_description = xmlReader.getElementValue();
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::END_ELEMENT); // End of Description element

// TerraLibVersion
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader.getElementLocalName() == "TerraLibVersion");

  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::VALUE);
  pInfo.m_terralibVersion = xmlReader.getElementValue();
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::END_ELEMENT); // End of TerraLibVersion element

// License
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader.getElementLocalName() == "License");

  if(xmlReader.hasAttrs())
  {
    pInfo.m_licenseURL = xmlReader.getAttr("xlink:href");
  }

  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::VALUE);
  pInfo.m_licenseDescription = xmlReader.getElementValue();
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::END_ELEMENT); // End of License element

// Category
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader.getElementLocalName() == "Category");

  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::VALUE);
  pInfo.m_category = xmlReader.getElementValue();
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::END_ELEMENT); // End of Category element

// Site
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader.getElementLocalName() == "Site");

  if(xmlReader.hasAttrs())
  {
    pInfo.m_site = xmlReader.getAttr("xlink:href");
  }
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::END_ELEMENT); // End of Site element

// Provider
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader.getElementLocalName() == "Provider");

  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader.getElementLocalName() == "Name");

  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::VALUE);
  pInfo.m_provider.m_name = xmlReader.getElementValue();
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::END_ELEMENT); // End of Provider/Name element

  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader.getElementLocalName() == "Site");

  if(xmlReader.hasAttrs())
  {
    pInfo.m_provider.m_site = xmlReader.getAttr("xlink:href");
  }
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::END_ELEMENT); // End of Provider/Site element

  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::START_ELEMENT);
  assert(xmlReader.getElementLocalName() == "Email");

  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::VALUE);
  pInfo.m_provider.m_email = xmlReader.getElementValue();
  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::END_ELEMENT); // End of Provider/Email element

  xmlReader.next();
  assert(xmlReader.getNodeType() == te::xml::END_ELEMENT); // End of Provider

// RequiredPlugins
  xmlReader.next();

  if((xmlReader.getNodeType() == te::xml::START_ELEMENT) &&
     (xmlReader.getElementLocalName() == "RequiredPlugins"))
  {
    while(xmlReader.next())
    {
      if((xmlReader.getNodeType() != te::xml::START_ELEMENT) ||
         (xmlReader.getElementLocalName() != "PluginId"))
      {
        xmlReader.next();
        break;
      }

      xmlReader.next();
      assert(xmlReader.getNodeType() == te::xml::VALUE);
      std::string pluginId = xmlReader.getElementValue();
      pInfo.m_requiredPlugins.push_back(pluginId);
      xmlReader.next();
      assert(xmlReader.getNodeType() == te::xml::END_ELEMENT);
    }
  }

// RequiredPluginCategory
  if((xmlReader.getNodeType() == te::xml::START_ELEMENT) &&
     (xmlReader.getElementLocalName() == "RequiredPluginCategory"))
  {
    while(xmlReader.next())
    {
      if((xmlReader.getNodeType() != te::xml::START_ELEMENT) ||
         (xmlReader.getElementLocalName() != "CategoryId"))
      {
        xmlReader.next();
        break;
      }

      xmlReader.next();
      assert(xmlReader.getNodeType() == te::xml::VALUE);
      std::string categoryId = xmlReader.getElementValue();
      pInfo.m_requiredPluginCategories.push_back(categoryId);
      xmlReader.next();
      assert(xmlReader.getNodeType() == te::xml::END_ELEMENT);
    }
  }

// RequiredModules
  if((xmlReader.getNodeType() == te::xml::START_ELEMENT) &&
     (xmlReader.getElementLocalName() == "RequiredModules"))
  {
    while(xmlReader.next())
    {
      if((xmlReader.getNodeType() != te::xml::START_ELEMENT) ||
         (xmlReader.getElementLocalName() != "ModuleId"))
      {
        xmlReader.next();
        break;
      }

      xmlReader.next();
      assert(xmlReader.getNodeType() == te::xml::VALUE);
      std::string moduleId = xmlReader.getElementValue();
      pInfo.m_requiredModules.push_back(moduleId);
      xmlReader.next();
      assert(xmlReader.getNodeType() == te::xml::END_ELEMENT);
    }
  }

// Resources
  if((xmlReader.getNodeType() == te::xml::START_ELEMENT) &&
     (xmlReader.getElementLocalName() == "Resources"))
  {
    while(xmlReader.next())
    {
      if((xmlReader.getNodeType() != te::xml::START_ELEMENT) ||
         (xmlReader.getElementLocalName() != "Resource"))
      {
        xmlReader.next();
        break;
      }

      std::string name = xmlReader.getAttr("name");
      std::string href = xmlReader.getAttr("xlink:href");

      pInfo.m_resources.push_back(te::plugin::PluginInfo::Resource(name, href));
      xmlReader.next();
      assert(xmlReader.getNodeType() == te::xml::END_ELEMENT);
    }
  }

// Parameters
  if((xmlReader.getNodeType() == te::xml::START_ELEMENT) &&
     (xmlReader.getElementLocalName() == "Parameters"))
  {
    while(xmlReader.next())
    {
      if((xmlReader.getNodeType() != te::xml::START_ELEMENT) ||
         (xmlReader.getElementLocalName() != "Parameter"))
      {
        xmlReader.next();
        break;
      }

      xmlReader.next();
      assert(xmlReader.getNodeType() == te::xml::START_ELEMENT);
      assert(xmlReader.getElementLocalName() == "Name");

      xmlReader.next();
      assert(xmlReader.getNodeType() == te::xml::VALUE);
      std::string paramName = xmlReader.getElementValue();
      xmlReader.next();
      assert(xmlReader.getNodeType() == te::xml::END_ELEMENT);

      xmlReader.next();
      assert(xmlReader.getNodeType() == te::xml::START_ELEMENT);
      assert(xmlReader.getElementLocalName() == "Value");

      xmlReader.next();
      assert(xmlReader.getNodeType() == te::xml::VALUE);
      std::string paramValue = xmlReader.getElementValue();
      xmlReader.next();
      assert(xmlReader.getNodeType() == te::xml::END_ELEMENT);

      pInfo.m_parameters.push_back(te::plugin::PluginInfo::Parameter(paramName, paramValue));
    }
  }

  assert(xmlReader.getNodeType() == te::xml::END_DOCUMENT);

  return pInfo;
}
