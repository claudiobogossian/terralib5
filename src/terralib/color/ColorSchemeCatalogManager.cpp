/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/color/ColorSchemeCatalogManager.cpp

  \brief Implementation of the singleton to manage color scheme catalogs.
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/PlatformUtils.h"
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "ColorScheme.h"
#include "ColorSchemeCatalog.h"
#include "ColorSchemeGroup.h"
#include "ColorSchemeCatalogManager.h"

// STL
#include <cassert>
#include <fstream>
#include <vector>

// Boost
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

void te::color::ColorSchemeCatalogManager::init()
{
  te::color::ColorSchemeCatalog* csc = findByName("Default");

  if(csc)
    throw te::common::Exception(TE_TR("The default color scheme catalog is already initialized!"));

  try
  {
    std::ifstream f;

    std::string jsonf = te::common::FindInTerraLibPath("share/terralib/json/color_ramps.json");
    
    if(jsonf.empty())
      throw te::common::Exception(TE_TR("Could not find color_ramps.json file!"));
    
    f.open(jsonf.c_str());
    
    if (!f.is_open())
      return;

    //create default color scheme catalog
    csc = new te::color::ColorSchemeCatalog("Default");

    insert(csc);

    boost::property_tree::ptree pt;
    boost::property_tree::json_parser::read_json(f,pt);
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("ramps"))
    {
      std::string name = v.second.get<std::string>("name");

      //create terraview color scheme group
      te::color::ColorSchemeGroup* csg = new te::color::ColorSchemeGroup(name);

      csc->push_back(csg);

      BOOST_FOREACH(boost::property_tree::ptree::value_type &t, v.second.get_child("schemes"))
      {
        std::string name = t.second.get<std::string>("name");

        te::color::ColorScheme* cs = new te::color::ColorScheme(name);

        std::vector<te::color::RGBAColor>* rgbaVec = new std::vector<te::color::RGBAColor>();

        BOOST_FOREACH(boost::property_tree::ptree::value_type &c, t.second.get_child("values"))
        {
          unsigned int red = c.second.get<unsigned int>("red");
          unsigned int green = c.second.get<unsigned int>("green");
          unsigned int blue = c.second.get<unsigned int>("blue");

          rgbaVec->push_back(te::color::RGBAColor(red, green, blue, 0));
        }

        cs->push_back(rgbaVec);

        csg->push_back(cs);
      }
    }

    f.close();
  }
  catch(boost::property_tree::json_parser::json_parser_error &je)
  {
    std::string errmsg = "Error parsing: " + je.filename() + ": " + je.message();
    te::common::Exception ex(TE_TR(errmsg));
    throw(ex);
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return;
}

void te::color::ColorSchemeCatalogManager::insert(ColorSchemeCatalog* c)
{
  assert(c);

  if(findByName(c->getName()))
    throw te::common::Exception(TE_TR("There is already a color scheme catalog with the given name!"));

  m_catalogs.push_back(c);
  m_catalogIdxByName.insert(std::map<std::string, ColorSchemeCatalog*>::value_type(c->getName(), c));
}

void te::color::ColorSchemeCatalogManager::disconnect(ColorSchemeCatalog* c)
{
  assert(c);

// first, find candidates for deletion... if one of then is not found, raise an exception
  std::map<std::string, ColorSchemeCatalog*>::iterator itProjectIdxByName = m_catalogIdxByName.find(c->getName());

  if(itProjectIdxByName != m_catalogIdxByName.end())
    throw te::common::Exception(TE_TR("Couldn't find the catalog with the given name!"));

  size_t i = 0;

  for(; i < m_catalogs.size(); ++i)
  {
    if(m_catalogs[i]->getName() == c->getName())
      break;
  }

  if(i == m_catalogs.size())
    throw te::common::Exception(TE_TR("Couldn't find the catalog with the given name!"));
  
// if we are here, so all entries are ok... just remove them
  m_catalogs.erase(m_catalogs.begin() + i);
  m_catalogIdxByName.erase(itProjectIdxByName);
}

void te::color::ColorSchemeCatalogManager::erase(ColorSchemeCatalog* c)
{
  disconnect(c);

// and delete the projet from main memory
  delete (c);
}

te::color::ColorSchemeCatalog* te::color::ColorSchemeCatalogManager::findByName(const std::string& name) const
{
  std::map<std::string, ColorSchemeCatalog*>::const_iterator it = m_catalogIdxByName.find(name);

  if(it != m_catalogIdxByName.end())
    return it->second;

  return 0;
}

std::pair<std::vector<te::color::ColorSchemeCatalog*>::const_iterator,
          std::vector<te::color::ColorSchemeCatalog*>::const_iterator> te::color::ColorSchemeCatalogManager::getIterator() const
{
  return std::pair<std::vector<te::color::ColorSchemeCatalog*>::const_iterator,
                   std::vector<te::color::ColorSchemeCatalog*>::const_iterator>(m_catalogs.begin(), m_catalogs.end());
}

const std::vector<te::color::ColorSchemeCatalog*>& te::color::ColorSchemeCatalogManager::getCatalogs() const
{
  return m_catalogs;
}

bool te::color::ColorSchemeCatalogManager::isEmpty() const
{
  return m_catalogs.empty();
}

void te::color::ColorSchemeCatalogManager::clear()
{
  te::common::FreeContents(m_catalogs);

  m_catalogs.clear();

  m_catalogIdxByName.clear();
}

te::color::ColorSchemeCatalogManager::~ColorSchemeCatalogManager()
{  
  clear();
}

te::color::ColorSchemeCatalogManager::ColorSchemeCatalogManager()
{
}

