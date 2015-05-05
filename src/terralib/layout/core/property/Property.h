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
  \file Property.h
   
   \brief A property acts like a attribute member of a object and stores the state of this attribute. A set of properties stores the state of an object.
   Any data type, not included in the convertValue method in the class te::layout::Variant, it will be by default "std::string" value.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PROPERTY_H 
#define __TERRALIB_LAYOUT_INTERNAL_PROPERTY_H

// TerraLib
#include "Variant.h"
#include "../enum/EnumType.h"
#include "../Config.h"

namespace te
{
  namespace layout
  {
    /*!
      \brief A property acts like a attribute member of a object and stores the state of this attribute. A set of properties stores the state of an object.
      Any data type, not included in the convertValue method in the class te::layout::Variant, it will be by default "std::string" value.
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT Property
    {
      public:
        
        /*!
          \brief Constructor
        */ 
        Property(int parentItemHashCode = 0);

        /*!
          \brief Destructor
        */ 
        virtual ~Property();

        /*!
          \brief Method that returns the name of this property.

          \return name of this property
        */
        std::string getName();

        /*!
          \brief Sets the name of this property.

          \param name of this property
        */
        void setName(std::string name); 
        
        /*!
          \brief Returns the type of this property 

          \return type of this property
        */
        EnumType* getType();

	      std::vector<te::layout::Property> getSubProperty();
        
        /*
          To use this method, you need to declare a variable with 
          corresponding type and pass it as the method parameter.
        */

        /*!
          \brief Stores a copy of value.

          \param value copies the value to be stored
          \param type data type
        */
        template <typename ValueType>
        void setValue(ValueType value, EnumType* type);

        /*!
          \brief Stores a copy of value.

          \param te::layout::Variant object
        */
        void setValue(Variant variant);

        /*!
          \brief Returns stored value

          \return te::layout::Variant object
        */
        Variant getValue();

        /*!
          \brief Returns true if property is editable, false otherwise. 

          \return true if property is editable, false otherwise 
        */
        bool isEditable();

        /*!
          \brief Sets true if property is editable, false otherwise 

          \param true if property is editable, false otherwise 
        */
        void setEditable(bool editable);

        /*Property Options*/

        void addOption(Variant variant);

        void removeOption(Variant variant);

        void setOptionChoice(Variant variant);

        void setOptionChoice(int numberOption);

        Variant getOptionByCurrentChoice();   

        std::vector<Variant> getOptionChoices();  
        
        //Ex.: For font information
        void addSubProperty( Property property );

        void removeSubProperty( Property property );

        /*!
          \brief Returns true if no value has been set, false otherwise.

          \return true if no value has been set, false otherwise 
        */
        bool isNull();

        /*!
          \brief Reset state of this object. Null state.  
        */
        void clear();

        /*!
          \brief Sets the label of this property.

          \param label of this property
        */
        virtual void setLabel(std::string label);

        /*!
          \brief Returns the label of this property.

          \return label of this property
        */
        virtual std::string getLabel();

        virtual bool containsSubProperty(Property subProperty);

        virtual Property containsSubProperty(std::string name);
        
        /*!
          \brief Sets true if property will be used in a menu, false otherwise.

          \param true if property will be used in a menu, false otherwise 
        */
        virtual void setMenu(bool menu);

        /*!
          \brief Returns true if property will be used in a menu, false otherwise.

          \return true if property will be used in a menu, false otherwise  
        */
        virtual bool isMenu();

        virtual void setIcon(std::string icon);

        virtual std::string getIcon();

        /*!
          \brief Return true if value is not of common C++ data type, false otherwise.

          \return true if value is not of common C++ data type, false otherwise  
        */
        virtual bool isComplex();

        /*!
          \brief Sets the visibility of this property.

          \param visibility of this property
        */
        virtual void setVisible(bool visible);

        /*!
          \brief Return true if visible, false otherwise.

          \return true if visible, false otherwise
        */
        virtual bool isVisible();

        /*!
          \brief Returns true if property is required, false otherwise. 

          \return true if property is required, false otherwise 
        */
        bool isRequired();

        /*!
          \brief Sets true if property is required, false otherwise 

          \param true if property is required, false otherwise 
        */
        void setRequired(bool required);

        /*!
          \brief Sets the hashcode of the object that owns this property. 

          \param hashcode
        */
        void setParentItemHashCode(int hashCode);

        /*!
          \brief Returns the hashcode of the object that owns this property.

          \return hashcode
        */
        int getParentItemHashCode();

        /*!
          \brief Returns true if property compose a widget, false otherwise.
          If true, and the object that owns this property has a parent, it will not appear in a property browser or a menu, it will be used within a widget.

          \return true if property compose a widget, false otherwise 
        */
        bool isComposeWidget();

        /*!
          \brief Sets true if property compose a widget, false otherwise
          If true, and the object that owns this property has a parent, it will not appear in a property browser or a menu, it will be used within a widget.

          \param true if property compose a widget, false otherwise 
        */
        void setComposeWidget(bool compose);

        /*!
          \brief Returns true if property is public, false otherwise.
          If the component, father of this property, is a child of another component, 
          then this property can be used by the parent component to display the value or call windows. It can not be edited.

          \return true if property compose a widget, false otherwise 
        */
        bool isPublic();

        /*!
          \brief Sets true if property is public, false otherwise
          If the component, father of this property, is a child of another component, 
          then this property can be used by the parent component to display the value or call windows. It can not be edited.

          \param true if property compose a widget, false otherwise 
        */
        void setPublic(bool publicProperty);
      
        bool operator ==(const Property& other); 

    protected:

      int m_parentItemHashCode; //!< hashcode of the object that owns this property
      std::string m_name; //!< name of this property
      EnumType* m_type; //!< data type of this property
      Variant m_value; //!<
      Variant m_currentChoice; //!<
      bool m_editable; //!<
      std::vector<Variant> m_options; //!<
      std::vector<te::layout::Property> m_subProperty; //!<
      std::string m_label; //!<
      bool m_menu; //!< the property will be used in a menu. 
      std::string m_icon; //!<
      bool m_visible; //!< visibility
      bool m_required; //!< required
      bool m_composeWidget; //!< compose widget
      bool m_public; //!< public property, used by another component
    };

    template <typename ValueType>
    inline void te::layout::Property::setValue(ValueType value, EnumType* type)
    {
      //Use the information type?
      Variant v;
      v.setValue(value, type);
      m_value = v;
      m_type = type;
    }

    inline bool te::layout::Property::operator ==(const Property& other) 
    { 
      Property& otherProp = const_cast<Property&>(other);

      if(getName().compare(otherProp.getName()) == 0)
      {
        return true;
      }

      return false;
    }
  }
}

#endif
