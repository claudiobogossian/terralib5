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
  \file PropertyBrowser.h
   
   \brief Manage properties variants values. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_VARIANT_PROPERTIES_BROWSER_H 
#define __TERRALIB_LAYOUT_INTERNAL_VARIANT_PROPERTIES_BROWSER_H

// TerraLib
#include "AbstractPropertiesBrowser.h"
#include "../../../core/property/Property.h"
#include "../../../core/Config.h"

// Qt
#include <QObject>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtProperty>
#include <QtPropertyBrowser/QtVariantPropertyManager>
#include <QtPropertyBrowser/QtVariantEditorFactory>
#include <QtPropertyBrowser/QtVariantProperty>

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
    class TELAYOUTEXPORT VariantPropertiesBrowser : public AbstractPropertiesBrowser
    {
      Q_OBJECT //for slots/signals

      public:

        VariantPropertiesBrowser(QObject *parent = 0);

        virtual ~VariantPropertiesBrowser();

        virtual QtVariantProperty* addProperty(Property property);

        virtual bool updateProperty(Property property);

        virtual Property getProperty(std::string name);

        virtual EnumType* getLayoutType(QVariant::Type type, std::string name = "");

        virtual int getVariantType(EnumType* dataType);

        virtual bool changeQtVariantPropertyValue(QtVariantProperty* vproperty, Property property);

        QtVariantPropertyManager* getVariantPropertyManager();

        QtVariantEditorFactory* getVariantEditorFactory();

      protected:

        virtual void createManager();

        virtual void addAttribute(QtVariantProperty* vproperty, Property property);

        QtVariantPropertyManager* m_variantPropertyEditorManager; 
        QtVariantEditorFactory*   m_variantPropertyEditorFactory;
    };
  }
}

#endif


