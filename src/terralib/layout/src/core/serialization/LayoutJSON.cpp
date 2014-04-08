/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file LayoutJSON.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LayoutJSON.h"
#include "LayoutProperty.h"
#include "../../../../common/Exception.h"
#include "../../../../common/STLUtils.h"
#include "../../../../common/Translator.h"
#include "Config.h"

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "boost/system/system_error.hpp"

// STL
#include <iostream>
#include <fstream>

te::layout::LayoutJSON::LayoutJSON() 
{

}

te::layout::LayoutJSON::~LayoutJSON()
{
  std::vector<LayoutJSON*>::iterator it;
  for(it = m_jsons.begin(); it != m_jsons.end(); ++it)
  {
    LayoutJSON* json = (*it);

    if(json)
    {
      delete json;
    }
  }
}

bool te::layout::LayoutJSON::serialize()
{
  std::ostringstream buf; 
  std::string json;

  boost::property_tree::ptree array = retrievePTree();

  try 
  {     
    boost::property_tree::write_json (buf, array, false);  
    json = buf.str();

    std::ofstream outputFile;
    outputFile.open(m_serializationPath, std::ios::out | std::ios::app);
    outputFile << json;
    outputFile.close();
  } 
  catch(boost::property_tree::json_parser::json_parser_error &je)
  {
    std::string errmsg = "Error parsing: " + je.filename() + ": " + je.message();
    te::common::Exception ex(TR_LAYOUT(errmsg));
    throw(ex);
  }
  catch (std::ofstream::failure e) 
  {
    std::cerr << e.what() << std::endl;
    std::string errmsg = "Exception opening/reading/closing file: \n ";
    te::common::Exception ex(TR_LAYOUT(errmsg));
    //throw(ex);
    return false;
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
    return false;
  }

  return true;
}

boost::property_tree::ptree te::layout::LayoutJSON::retrievePTree()
{
  std::vector<LayoutJSON*>::iterator it;
  boost::property_tree::ptree array;

  boost::property_tree::ptree copyArray = m_array;

  for(it = m_jsons.begin(); it != m_jsons.end(); ++it)
  {
    std::vector<std::string> vecstr = (*it)->getKeys();

    std::vector<std::string>::iterator its;
    for(its = vecstr.begin(); its != vecstr.end(); ++its)
    {
      boost::property_tree::ptree  tree = (*it)->retrievePTree();
      for (boost::property_tree::ptree::const_iterator itv = tree.begin(); itv != tree.end(); ++itv) 
      {          
        array.put_child(itv->first, itv->second);
      }
    } 
  }

  if(!array.empty())
    copyArray.add_child(m_rootKey, array);
  
  return copyArray;
}

std::vector<te::layout::LayoutProperties*> te::layout::LayoutJSON::retrieve() 
{

  //>>>>>>>>>>>>>>>>>>>>>>>
  //>>>>>>>>>>>>>>>>>>
  //>>>>>>>>>>>>>


  std::vector<te::layout::LayoutProperties*> props;
  std::vector<LayoutJSON*>::iterator it;

  for(it = m_jsons.begin(); it != m_jsons.end(); ++it)
  {
    std::vector<std::string> vecstr = (*it)->getKeys();
    
    te::layout::LayoutProperties* properties = new te::layout::LayoutProperties((*it)->getRootKey());

    std::vector<std::string>::iterator its;
    for(its = vecstr.begin(); its != vecstr.end(); ++its)
    {
      te::layout::LayoutProperty prop;
      properties->addProperty(prop);
    }
    props.push_back(properties);
  }
  return props;
}

bool te::layout::LayoutJSON::isEmpty()
{
  return m_array.empty();
}

void te::layout::LayoutJSON::addChild( LayoutJSON* json )
{
  m_jsons.push_back(json);
}

void te::layout::LayoutJSON::loadFromPath( std::string loadPath )
{
  m_loadPath = loadPath;

  boost::property_tree::ptree jsonTree; 
  std::ifstream inputFile;

  try 
  {     
    inputFile.open(m_loadPath);

    if (!inputFile.is_open())
      return;

    boost::property_tree::json_parser::read_json(inputFile, jsonTree);

    int count = jsonTree.size();

    for (boost::property_tree::ptree::const_iterator itv = jsonTree.begin(); itv != jsonTree.end(); ++itv) 
    {          
      LayoutJSON* j_child = new LayoutJSON;
      j_child->add(itv->first, itv->second);
      searchPTree(itv->second, j_child);
      
      this->addChild(j_child);
    }

    inputFile.close();
  }
  catch(boost::property_tree::json_parser::json_parser_error &je)
  {
    std::string errmsg = "Error parsing: " + je.filename() + ": " + je.message();
    te::common::Exception ex(TR_LAYOUT(errmsg));
    //throw(ex);
    return;
  }
  catch (std::ifstream::failure &e) 
  {
    std::cerr << e.what() << std::endl;
    std::string errmsg = "Exception opening/reading/closing file: \n ";
    te::common::Exception ex(TR_LAYOUT(errmsg));
    //throw(ex);
    return;
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void te::layout::LayoutJSON::searchPTree( boost::property_tree::ptree tree, LayoutJSON* json )
{
  for (boost::property_tree::ptree::const_iterator itv = tree.begin(); itv != tree.end(); ++itv) 
  {          
    LayoutJSON* j_child = new LayoutJSON;
    j_child->add(itv->first, itv->second);
    searchPTree(itv->second, j_child);

    json->addChild(j_child);
  }
}

void te::layout::LayoutJSON::loadFromProperties( std::vector<te::layout::LayoutProperties*> properties )
{
  m_properties = properties;
  
  std::vector<te::layout::LayoutProperties*>::iterator it;
  std::vector<te::layout::LayoutProperty>::iterator ity;
  
  add("template", m_rootKey);

  for(it = m_properties.begin(); it != m_properties.end(); ++it)
  {		
    LayoutProperties* props = (*it);
    if(!props)
      continue;

    std::vector<te::layout::LayoutProperty> vec = props->getProperties();

    if(vec.empty())
      continue;
    
    /*Falta ainda saber de que tipo é o valor: */
    LayoutJSON* j_child = new LayoutJSON;

    for(ity = vec.begin(); ity != vec.end(); ++ity)
    {
      LayoutProperty prop = (*ity);

      /*Falta ainda saber de que tipo é o valor: */
      LayoutJSON* j_child_prop = new LayoutJSON;
      searchProperty(prop, j_child_prop);
      j_child_prop->add(prop.getName(), prop.getValue().toString());
      j_child->addChild(j_child_prop);
    }
    j_child->setRootKey(props->getObjectName());
    j_child->add("name", props->getObjectName());
    addChild(j_child);
  }
}

void te::layout::LayoutJSON::searchProperty( LayoutProperty property, LayoutJSON* json )
{
  if(!property.getSubProperty().empty())
  {
    std::vector<LayoutProperty> props = property.getSubProperty();

    std::vector<LayoutProperty>::iterator it;

    for(it = props.begin(); it != props.end(); ++it)
    {		
      LayoutProperty prop = (*it);

      /*Falta ainda saber de que tipo é o valor: */
      LayoutJSON* j_child = new LayoutJSON;
      j_child->add(prop.getName(), prop.getValue().toString());
      json->addChild(j_child);
      
      searchProperty(prop, j_child);
    }
  }
}

int te::layout::LayoutJSON::numberchilds()
{
  return m_jsons.size();
}

std::vector<te::layout::LayoutJSON*> te::layout::LayoutJSON::getChilds() const
{
  return m_jsons;
}

std::vector<std::string> te::layout::LayoutJSON::getKeys()
{
  return m_keys;
}
