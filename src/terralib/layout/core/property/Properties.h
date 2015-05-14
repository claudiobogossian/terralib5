/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
   
  \brief The Properties class represents a persistent set of properties. The Properties can be saved to a file (Ex.: .json) or loaded from a file (Ex.: .json).
    Also used for interaction, with user or other classes of this module, to change state of a MVC Component.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_H
#define __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_H

//Terralib
#include "Property.h"
#include "../enum/AbstractType.h"
#include "../Config.h"
#include "../enum/EnumType.h"

//STL
#include <vector>
#include <algorithm>

namespace te
{
  namespace layout
  {
    /*!
      \brief The Properties class represents a persistent set of properties. The Properties can be saved to a file (Ex.: .json) or loaded from a file (Ex.: .json).
        Also used for interaction, with user or other classes of this module, to change state of a MVC Component.
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT Properties
    {
      public:

        /*!
          \brief Constructor

          \param objectName Object name that owns these properties
          \param type Object type that owns these properties
        */ 
        Properties(std::string objectName, EnumType* type = 0, int hashCode = 0);

        /*!
          \brief Destructor
        */
        virtual ~Properties(void);

        /*!
          \brief Adds the specified property to the set of properties for this object.

          \param property specified property
          \return true if add, false otherwise 
        */
        virtual bool addProperty(Property property);

        /*!
          \brief Removes a property from the set of properties of this object.

          \param property specified property
          \return true if remove, false otherwise 
        */
        virtual bool removeProperty(std::string name);

        /*!
          \brief Clear set of properties of this object.

          \return true if clear, false otherwise
        */
        virtual bool clear();

        /*!
          \brief Returns set of all properties

          \return set of all properties
        */
        virtual std::vector<Property> getProperties();

        /*!
          \brief Returns object name that owns these properties.

          \return object name that owns these properties
        */
        virtual std::string getObjectName();

        /*!
          \brief Sets object name that owns these properties.

          \param object name that owns these properties
        */
        virtual void setObjectName(std::string name);

        /*!
          \brief Returns object type that owns these properties.

          \return object type that owns these properties
        */
        virtual EnumType* getTypeObj();

        /*!
          \brief Sets object type that owns these properties.

          \param object type that owns these properties
        */
        virtual void setTypeObj(EnumType* type);

        /*!
          \brief 

          \param 
        */
        virtual void setHasWindows(bool windows);

        /*!
          \brief 

          \return 
        */
        virtual bool hasWindows();

        /*!
          \brief Checks if the property is contained within the set of properties.

          \param property
          \return true if contained, false otherwise
        */
        virtual bool contains(Property property);

        /*!
          \brief Checks if the name is contained within the set of properties.

          \param name name of the property
          \return true if contained, false otherwise
        */
        virtual Property contains(std::string name);

        /*!
          \brief Returns the hashcode of a MVC component.

          \return hashCode
        */
        virtual int getHashCode();

        /*!
          \brief Sets the hashcode of a MVC component.

          \return hashCode
        */
        virtual void setHashCode(int hashCode);

      protected:

        std::vector<Property> m_properties; //!< set of properties for this object
        std::string m_objName; //!< Object name that owns these properties
        EnumType* m_typeObj; //!< Object type that owns these properties
        bool m_hasWindows; //!<
        int  m_hashcode;

    };

    inline Properties::Properties(std:: string objectName, te::layout::EnumType* type, int hashCode) :
      m_objName(objectName),
      m_typeObj(type),
      m_hasWindows(false),
      m_hashcode(hashCode)
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

    inline te::layout::EnumType* Properties::getTypeObj()
    {
      return m_typeObj;
    }

    inline void Properties::setTypeObj( te::layout::EnumType* type )
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
      Property property(0);
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

    inline void te::layout::Properties::setHasWindows( bool windows )
    {
	    m_hasWindows = windows;
    }

    inline bool te::layout::Properties::hasWindows()
    {
	    return m_hasWindows;
    }

    inline int te::layout::Properties::getHashCode()
    {
      return m_hashcode;
    }

    inline void te::layout::Properties::setHashCode( int hashCode )
    {
      m_hashcode = hashCode;
    }
  }
}

#endif