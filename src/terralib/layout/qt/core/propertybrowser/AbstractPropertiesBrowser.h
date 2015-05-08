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
  \file PropertyBrowser.h
   
   \brief Manage properties variants values. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_PROPERTIES_BROWSER_H 
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_PROPERTIES_BROWSER_H

// TerraLib
#include "../../../core/property/Property.h"
#include "../../../core/Config.h"

// STL
#include <string>

// Qt
#include <QObject>
#include <QVariant>
#include <QString>
#include <QMap>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtVariantProperty>
#include <QtPropertyBrowser/QtProperty>

class QGraphicsItem;
class QWidget;

namespace te
{
  namespace layout
  {
    class Properties;
    class EnumType;

    /*!
    \brief Manage properties variants values.  
	  
	  \ingroup layout
	  */
    class TELAYOUTEXPORT AbstractPropertiesBrowser : public QObject
    {
      public:

	      AbstractPropertiesBrowser(QObject *parent = 0);

	      virtual ~AbstractPropertiesBrowser();

        virtual QtProperty* addProperty(Property property) = 0;

        virtual bool updateProperty(Property property) = 0;

        virtual void clearAll();
        
        virtual Property getProperty(std::string name) = 0;

        virtual EnumType* getLayoutType(QVariant::Type type, std::string name = "") = 0;

        virtual int getVariantType(EnumType* dataType) = 0;
        
        virtual QVariant findPropertyValue(std::string name);

        virtual QtProperty* findProperty(std::string name);

        virtual bool removeProperty(QtProperty* prop);
                          
      protected:

        virtual void createManager() = 0;

        virtual void addPropertyItem(QtProperty *property, const QString &id);

        virtual QVariant checkComplexType(QtVariantProperty* property);

        bool                        m_changeProperty;
        QMap<QtProperty*, QString>  m_propertyToId;
        QMap<QString, QtProperty*>  m_idToProperty;
    };
  }
}

#endif




