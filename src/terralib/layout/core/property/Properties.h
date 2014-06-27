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
#include "AbstractType.h"

//STL
#include <vector>
#include <algorithm>

namespace te
{
  namespace layout
  {
    class Properties
    {
      public:
        Properties(std:: string objectName, LayoutAbstractObjectType type = TPObjectUnknown);
        virtual ~Properties(void);

        virtual bool addProperty(Property property);
        virtual bool removeProperty(std::string name);
        virtual bool clear();
        virtual std::vector<Property> getProperties();

        virtual std::string getObjectName();

        virtual void setObjectName(std::string name);

        virtual LayoutAbstractObjectType getTypeObj();

        virtual void setTypeObj(LayoutAbstractObjectType type);

        virtual void setHasGridWindows(bool windows);

        virtual bool hasGridWindows();

        virtual bool contains(Property property);

        virtual Property contains(std::string name);

      protected:
        std::vector<Property> m_properties;
        std::string m_objName;
        LayoutAbstractObjectType m_typeObj;
        bool m_hasGridWindows;

    };

    inline Properties::Properties(std:: string objectName, LayoutAbstractObjectType type) :
      m_objName(objectName),
      m_typeObj(type),
      m_hasGridWindows(false)
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
      bool result = false;

      for(std::vector<Property>::iterator it = m_properties.begin(); it != m_properties.end(); it++)
      {
        if((*it).getName().compare(name) == 0)
        {
          m_properties.erase(it);
          result = true;
          break;
        }
      }
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

    inline te::layout::LayoutAbstractObjectType Properties::getTypeObj()
    {
      return m_typeObj;
    }

    inline void Properties::setTypeObj( LayoutAbstractObjectType type )
    {
      m_typeObj = type;
    }

    inline bool Properties::contains( Property property )
    {
      bool is_present = false;
      
      if(std::find(m_properties.begin(), m_properties.end(), property) != m_properties.end())
      {
        is_present = true;
      }

      return is_present;
    }

    inline te::layout::Property Properties::contains( std::string name )
    {
      Property property;
      property.setName(name);

      if(std::find(m_properties.begin(), m_properties.end(), property) != m_properties.end())
      {
        std::vector<Property>::iterator it = std::find(m_properties.begin(), m_properties.end(), property);

        property = (*it);
      }
      else
        property.setName("");

      return property;
    }

    inline void te::layout::Properties::setHasGridWindows( bool windows )
    {
	    m_hasGridWindows = windows;
    }

    inline bool te::layout::Properties::hasGridWindows()
    {
	    return m_hasGridWindows;
    }
  }
}

#endif