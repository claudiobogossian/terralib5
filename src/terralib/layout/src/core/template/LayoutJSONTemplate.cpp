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
  \file LayoutJSONTemplate.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LayoutJSONTemplate.h"
#include "LayoutProperties.h"
#include "LayoutJSON.h"

// STL
#include <iostream>
#include <fstream>

te::layout::LayoutJSONTemplate::LayoutJSONTemplate(std::string path) :
  LayoutTemplate(path),
  m_type(TPJSONTemplate)
{

}

te::layout::LayoutJSONTemplate::~LayoutJSONTemplate()
{

}

bool te::layout::LayoutJSONTemplate::exportTemplate( std::vector<te::layout::LayoutProperties*> properties )
{
  bool is_save = false;
  /*If the file is opened for output operations and it already existed, 
  its previous content is deleted */
  std::ofstream outputFile;
  outputFile.open(m_path, std::ios::trunc);
  outputFile.close();

  LayoutJSON* json = new LayoutJSON;
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

std::vector<te::layout::LayoutProperties*> te::layout::LayoutJSONTemplate::importTemplate()
{
  LayoutJSON* json = new LayoutJSON;
  json->loadFromPath(m_path);
  json->setSerializationPath(m_path); 
  
  std::vector<te::layout::LayoutProperties*> props = json->retrieve();

  if(json)
  {
    delete json;
    json = 0;
  }

  return props;
}

bool te::layout::LayoutJSONTemplate::deleteTemplate()
{
  return true;
}

te::layout::LayoutTemplateType te::layout::LayoutJSONTemplate::getType()
{
  return m_type;
}

