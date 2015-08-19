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
  \file Observable.h
   
  \brief Abstract class to represent an observable. "Model" part of MVC component. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACTITEMMODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACTITEMMODEL_H

// TerraLib
#include "../observer/Subject.h"
#include "../../Config.h"
#include "../../property/Properties.h"

namespace te
{
  namespace layout
  {

    class Property;

    /*!
      \brief Abstract class to represent an observable. "Model" part of MVC component. 
    
      \ingroup layout
    */
    class TELAYOUTEXPORT AbstractItemModel : public Subject
    {
      public:

        /*!
          \brief Constructor
        */ 
        AbstractItemModel();

        /*!
          \brief Destructor
        */
        virtual ~AbstractItemModel();

        /*!
          \brief Gets the property from the given propertyName
        */
        virtual const Property& getProperty(const std::string& propertyName) const;

        /*!
          \brief Sets the new value of the given property
        */
        virtual void setProperty(const Property& property);

        /*!
          \brief Gets all the properties
        */
        virtual const Properties& getProperties() const;

        /*!
          \brief Sets the new values of the given properties
        */
        virtual void setProperties(const Properties& properties);

      protected:

        Properties            m_properties; //!< The properties of the item
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_ABSTRACTITEMMODEL_H
