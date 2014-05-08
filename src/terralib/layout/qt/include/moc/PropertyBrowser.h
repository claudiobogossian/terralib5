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
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PROPERTY_BROWSER_H 
#define __TERRALIB_LAYOUT_INTERNAL_PROPERTY_BROWSER_H

// TerraLib
#include "Property.h"

// Qt
#include <QObject>
#include "../../../../../../third-party/qt/propertybrowser/qttreepropertybrowser.h"
#include "../../../../../../third-party/qt/propertybrowser/qtpropertybrowser.h"
#include "../../../../../../third-party/qt/propertybrowser/qtpropertymanager.h"
#include <QList>

class QGraphicsItem;
class QWidget;

class QtVariantProperty;
class QtProperty;
class QtVariantPropertyManager;
class QtBrowserIndex;

namespace te
{
  namespace layout
  {
    class Properties;

    class PropertyBrowser : public QObject
    {
	    Q_OBJECT //for slots/signals

    public:

	    PropertyBrowser(QObject *parent = 0);
	    virtual ~PropertyBrowser();

      QtTreePropertyBrowser* getPropertyEditor();
      QtVariantPropertyManager* getVariantPropertyManager();

      virtual void clearAll();

      virtual bool addProperty(Property property);
      
      virtual bool removeProperty(Property property);

      virtual Property getProperty(std::string name);

      virtual Properties* getProperties();

      virtual LayoutPropertyDataType getLayoutType(QVariant::Type type, std::string name = "");

      virtual QVariant::Type getVariantType(LayoutPropertyDataType dataType);

      virtual std::string getPropGridSettingsName();

      virtual void setHasGridWindows(bool hasWindows = false);
            
      private slots:

        void propertyEditorValueChanged(QtProperty *property, const QVariant &value);
        void onChangeFilter(const QString& filter);
        virtual void onSetDlg(QWidget *parent, QtProperty * prop) = 0;
        
      signals:

        void changePropertyValue(QtProperty *property, QList<QtBrowserItem*> items);
        void changePropertyValue(Property property);

    protected:
      
      virtual void addPropertyItem(QtProperty *property, const QString &id);
      virtual void updateExpandState();
      virtual void createManager();
      virtual void changeVisibility( QList<QtBrowserItem*> items, bool visible );
      virtual QVariant findPropertyValue(std::string name);
      virtual QtProperty* findProperty(std::string name);
      virtual void blockOpenGridWindows(bool block);
      virtual void addAttribute(QtVariantProperty* vproperty, Property property);
      virtual QVariant checkComplexType(QtVariantProperty* property);
      
    protected:

      QtTreePropertyBrowser* m_propertyEditor;
      QtVariantPropertyManager* m_variantPropertyEditorManager; 
      QtStringPropertyManager*  m_strDlgManager;
      QMap<QtProperty*, QString> m_propertyToId;
      QMap<QString, QtProperty*> m_idToProperty;
      QMap<QString, bool> m_idToExpanded;

      /* Custom Types: Dialog Window Type */
      std::string          m_propGridSettingsName;
      bool                 m_hasGridWindows;
    };
  }
}

#endif
