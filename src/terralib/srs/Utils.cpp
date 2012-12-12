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


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include "Exception.h"
#include "SpatialReferenceSystemManager.h"
#include "Utils.h"


void te::srs::InitManagerFromJSON()
{ 
  try
  {
    std::ifstream f;
    std::string jsonf = TERRALIB_INIT_FILES_LOCATION;
    jsonf += "/srsjson.txt";

    f.open(jsonf.c_str());
    if (!f.is_open())
      return;
    
    boost::property_tree::ptree pt;
    boost::property_tree::json_parser::read_json(f,pt);
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("SRSs"))
    {
      SpatialReferenceSystemManager::getInstance().add(v.second.get<std::string>("name"), 
                                                       v.second.get<std::string>("pj4txt"), 
                                                       v.second.get<std::string>("wkt"), 
                                                       v.second.get<unsigned int>("srid"));
    }
    f.close();
  }
  catch(boost::property_tree::json_parser::json_parser_error &je)
  {
    std::string errmsg = "Error parsing: " + je.filename() + ": " + je.message();
    te::srs::Exception ex(TR_SRS(errmsg));
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return;
}




