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
  \file Utils.h
   
  \brief Utility functions for STLoader module.  
 */

//TerraLib XML
#include "../xml/Reader.h"

//STLoader
#include "Utils.h"

#include <stdlib.h>

namespace te
{
  namespace stloader
  {
     //auxiliary function
     bool GetNextValueNode(te::xml::Reader* xmlReader, const std::string& nodeName, std::string& nodeValue)
     {
       while( xmlReader->getNodeType() != te::xml::VALUE ||  
              xmlReader->getElementLocalName() != nodeName )
       {
         if(!xmlReader->next())
           return false;
       }
     
       nodeValue = xmlReader->getElementValue();
       return true;
     }         
 
     //auxiliary function
     bool GetNextStartNode(te::xml::Reader* xmlReader, const std::string& nodeName)
     {
       while( xmlReader->getNodeType() != te::xml::START_ELEMENT ||  
               xmlReader->getElementLocalName() != nodeName )
       {
          if(!xmlReader->next())
            return false;
       }
       return true;
     }
   }
}