/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file Properties.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_H
#define __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_H

//Terralib
#include "Property.h"

//STL
#include <vector>

namespace te
{
  namespace layout
  {
    class Properties
    {
      public:
        Properties(std:: string objectName);
        virtual ~Properties(void);

        virtual bool addProperty(Property property);
        virtual bool removeProperty(std::string name);
        virtual bool clear();
        virtual std::vector<Property> getProperties();

        virtual std::string getObjectName();

        virtual void setObjectName(std::string name);

      protected:
        std::vector<Property> m_properties;
        std::string m_objName;

    };

    inline Properties::Properties(std:: string objectName) :
      m_objName(objectName)
    {
    }

    inline Properties::~Properties( void )
    {
    }

    inline bool Properties::addProperty(Property property)
    {
      unsigned int total = m_properties.size();
      m_properties.push_back(property);   

      if(m_properties.size() > total)
        return true;
      return false;
    }

    inline bool Properties::removeProperty(std::string name)
    {
      bool result = true;
      return result;
    }

    inline std::vector<Property> Properties::getProperties()
    {
      return m_properties;
    }

    inline bool Properties::clear()
    {
	    m_properties.clear();
      return m_properties.empty();
    }

    inline std::string Properties::getObjectName()
    {
      return m_objName;
    }

    inline void Properties::setObjectName( std::string name )
    {
      m_objName = name;
    }

  }
}

#endif