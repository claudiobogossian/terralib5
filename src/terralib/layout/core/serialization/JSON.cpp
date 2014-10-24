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
  \file JSON.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "JSON.h"
#include "../property/Property.h"
#include "../property/Properties.h"
#include "../../../common/Exception.h"
#include "../../../common/STLUtils.h"
#include "../../../common/Translator.h"
#include "../Config.h"
#include "../enum/Enums.h"

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "boost/system/system_error.hpp"

// STL
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>  

te::layout::JSON::JSON() 
{

}

te::layout::JSON::~JSON()
{

}

bool te::layout::JSON::serialize()
{
  std::ostringstream buf; 
  std::string json;

  boost::property_tree::ptree array = retrievePTree();

  try 
  {     
    boost::property_tree::write_json (buf, array, false);  
    json = buf.str();

    std::ofstream outputFile;
    outputFile.open(m_serializationPath.c_str(), std::ios::out | std::ios::app);
    outputFile << json;
    outputFile.close();
  } 
  catch(boost::property_tree::json_parser::json_parser_error &je)
  {
    std::string errmsg = "Error parsing: " + je.filename() + ": " + je.message();
    te::common::Exception ex(TE_TR(errmsg));
    throw(ex);
  }
  catch (std::ofstream::failure e) 
  {
    std::cerr << e.what() << std::endl;
    std::string errmsg = "Exception opening/reading/closing file: \n ";
    te::common::Exception ex(TE_TR(errmsg));
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

boost::property_tree::ptree te::layout::JSON::retrievePTree()
{
  return m_array;
}

std::vector<te::layout::Properties*> te::layout::JSON::retrieve() 
{
  std::vector<te::layout::Properties*> propsRetrieve;
    
  //v.first //is the name of the child.
  //v.second //is the child tree.

  boost::property_tree::ptree::assoc_iterator it1 = m_array.find("template");
  boost::property_tree::ptree::assoc_iterator it_nofound = m_array.not_found();

  if (it1 == it_nofound)
    return propsRetrieve;

  boost::property_tree::ptree subtree = (*it1).second;  

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  int count = 0;
  while(true)
  {
    std::stringstream ss;//create a stringstream
    ss << count;//add number to the stream

    std::string s_prop = "properties_"+ ss.str();

    boost::property_tree::ptree::assoc_iterator it2 = subtree.find(s_prop); 
    boost::property_tree::ptree::assoc_iterator it_nofound2 = subtree.not_found();

    if (it2 == it_nofound2)
      return propsRetrieve;

    boost::property_tree::ptree subtree1 = (*it2).second;

    boost::property_tree::ptree::assoc_iterator itName = subtree1.find("name");
    boost::property_tree::ptree::assoc_iterator it_nofoundName = subtree1.not_found();

    if (itName == it_nofoundName)
      return propsRetrieve;

    te::layout::Properties* props = new te::layout::Properties((*itName).second.data());

    EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();

    std::string valName;
    boost::property_tree::ptree tree;
    Property prop;
    BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, subtree.get_child(s_prop)) 
    {
      if(v.first.compare("object_type") == 0)
      {
        EnumType* type = enumObj->getEnum(v.second.data());
        props->setTypeObj(type);
        continue;
      }

      if(v.first.compare("type") == 0)
      {
        prop.setName(valName);
        EnumType* tp = dataType->getEnum(v.second.data());
        Variant vt;
        vt.fromPtree(tree, tp);
        prop.setValue(vt);
        props->addProperty(prop); 
        prop.clear();
      }
      else
      {
        std::string val = v.first;
        valName = val;
        tree = v.second;
        
        retrieveSubPTree(tree, prop);
      }
    }

    propsRetrieve.push_back(props);
    count+= 1;
  }

  return propsRetrieve;
}

void te::layout::JSON::retrieveSubPTree( boost::property_tree::ptree subTree, Property& prop )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  std::string valName;
  boost::property_tree::ptree tree;
  BOOST_FOREACH( const boost::property_tree::ptree::value_type &v, subTree.get_child("") ) 
  {  
    Property proper;
    if(v.first.compare("type") == 0)
    {
      proper.setName(valName);
      EnumType* tp = dataType->getEnum(v.second.data());
      proper.setValue(tree.data(), tp);
      prop.addSubProperty(proper);
    }
    else
    {
      std::string val = v.first;
      valName = val;
      tree = v.second;
    }

    // recursive go down the hierarchy  
    retrieveSubPTree(tree, proper);
  }
}

bool te::layout::JSON::isEmpty()
{
  return m_array.empty();
}

void te::layout::JSON::loadFromPath( std::string loadPath )
{
  m_loadPath = loadPath;

  std::ifstream inputFile;

  try 
  {     
    inputFile.open(m_loadPath.c_str());

    if (!inputFile.is_open())
      return;

    boost::property_tree::json_parser::read_json(inputFile, m_array);
    inputFile.close();
  }
  catch(boost::property_tree::json_parser::json_parser_error &je)
  {
    std::string errmsg = "Error parsing: " + je.filename() + ": " + je.message();
    te::common::Exception ex(TE_TR(errmsg));
    //throw(ex);
    return;
  }
  catch (std::ifstream::failure &e) 
  {
    std::cerr << e.what() << std::endl;
    std::string errmsg = "Exception opening/reading/closing file: \n ";
    te::common::Exception ex(TE_TR(errmsg));
    //throw(ex);
    return;
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void te::layout::JSON::loadFromProperties( std::vector<te::layout::Properties*> properties )
{
  m_properties = properties;
  
  std::vector<te::layout::Properties*>::iterator it;
  std::vector<te::layout::Property>::iterator ity;
  
  boost::property_tree::ptree rootArray;
  boost::property_tree::ptree childArray;

  rootArray.push_back(std::make_pair("name", m_rootKey));
  
  int count = 0;
  for(it = m_properties.begin(); it != m_properties.end(); ++it)
  {		
    Properties* props = (*it);
    if(!props)
      continue;

    std::vector<te::layout::Property> vec = props->getProperties();

    if(vec.empty())
      continue;

    childArray.clear();
    childArray.push_back(std::make_pair("object_type", props->getTypeObj()->getName()));
    for(ity = vec.begin(); ity != vec.end(); ++ity)
    {
      Property prop = (*ity);
      childArray.push_back(std::make_pair(prop.getName(), prop.getValue().convertToString())); 
      childArray.push_back(std::make_pair("type", prop.getType()->getName())); 
      searchProperty(prop, childArray, childArray);
    }
    if(!childArray.empty())
    {
      std::stringstream ss;//create a stringstream
      ss << count;//add number to the stream

      std::string s_prop = "properties_"+ ss.str();
      rootArray.push_back(std::make_pair(s_prop, childArray));
    }
    count+= 1;
  }
  m_array.push_back(std::make_pair("template", rootArray));
}

void te::layout::JSON::searchProperty( Property& property, boost::property_tree::ptree& array, boost::property_tree::ptree& child )
{
  if(!property.getSubProperty().empty())
  {
    Property propCopy = property;

    std::vector<Property> props = property.getSubProperty();

    std::vector<Property>::iterator it;

    for(it = props.begin(); it != props.end(); ++it)
    {		
      Property prop = (*it);

      boost::property_tree::ptree childArray;
      childArray.push_back(std::make_pair(prop.getName(), prop.getValue().convertToString()));
      childArray.push_back(std::make_pair("type", prop.getType()->getName())); 

      std::string s_name = prop.getName() + "_child";

      child.push_back(std::make_pair(s_name,childArray));
      
      searchProperty(prop, array, childArray);
    }

    std::string s_nameChild = propCopy.getName() + "_child";
    if(array != child)
      array.push_back(std::make_pair(s_nameChild,child));
  }
}
