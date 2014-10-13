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
  \file JSONTemplate.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "JSONTemplate.h"
#include "../property/Properties.h"
#include "../serialization/JSON.h"
#include "../enum/Enums.h"

// STL
#include <iostream>
#include <fstream>

te::layout::JSONTemplate::JSONTemplate(std::string path) :
  AbstractTemplate(path)
{
  m_type = Enums::getInstance().getEnumTemplateType()->getJsonType();
}

te::layout::JSONTemplate::~JSONTemplate()
{

}

bool te::layout::JSONTemplate::exportTemplate( std::vector<te::layout::Properties*> properties )
{
  bool is_save = false;
  /*If the file is opened for output operations and it already existed, 
  its previous content is deleted */
  std::ofstream outputFile;
  outputFile.open(m_path.c_str(), std::ios::trunc);
  outputFile.close();

  JSON* json = new JSON;
  json->loadFromProperties(properties);
  json->setSerializationPath(m_path);  
  
  is_save = json->serialize();

  if(json)
  {
    delete json;
    json = 0;
  }
  
  return is_save;
}

std::vector<te::layout::Properties*> te::layout::JSONTemplate::importTemplate()
{
  JSON* json = new JSON;
  json->loadFromPath(m_path);
  json->setSerializationPath(m_path); 
  
  std::vector<te::layout::Properties*> props = json->retrieve();

  if(json)
  {
    delete json;
    json = 0;
  }

  return props;
}

bool te::layout::JSONTemplate::deleteTemplate()
{
  return true;
}


