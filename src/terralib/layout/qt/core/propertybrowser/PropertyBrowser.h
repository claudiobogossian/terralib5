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
   
   \brief Manage properties variants values. Maps the QProperty properties (Qt) and Property (Layout) and add to a tree (QtTreePropertyBrowser) for presentation to the user, 
   allowing the modification and interactive update.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PROPERTY_BROWSER_H 
#define __TERRALIB_LAYOUT_INTERNAL_PROPERTY_BROWSER_H

// TerraLib
#include "../../../core/property/Property.h"
#include "../../../core/Config.h"
#include "../../../core/property/Properties.h"

//STL
#include <vector>

// Qt
#include <QObject>
#include <QMap>

class QtTreePropertyBrowser;
class QtStringPropertyManager;
class QtProperty;
class QtVariantPropertyManager;
class QtBrowserItem;

class QGraphicsItem;
class QWidget;

namespace te
{
  namespace layout
  {
    class EnumType;
    class VariantPropertiesBrowser;
    class DialogPropertiesBrowser;
    class FilePathManager;

    /*!
    \brief Manage properties variants values. Maps the QProperty properties (Qt) and Property (Layout) and add to a tree (QtTreePropertyBrowser) for presentation to the user, 
    allowing the modification and interactive update. 
    
    \ingroup layout
    */
    class TELAYOUTEXPORT PropertyBrowser : public QObject
    {
      Q_OBJECT //for slots/signals

      public:

        PropertyBrowser(QObject *parent = 0);

        virtual ~PropertyBrowser();

        QtTreePropertyBrowser* getPropertyEditor();

        VariantPropertiesBrowser* getVariantPropertiesBrowser();

        DialogPropertiesBrowser* getDialogPropertiesBrowser();

        virtual void clearAll();

        virtual void closeAllWindows();

        virtual QMap<QString, QtProperty*> addProperties(const Properties& properties);
      
        virtual bool removeProperty(Property property);

        virtual bool updateProperty(Property property);

        virtual void updateProperties(Properties props);

        virtual Properties getProperties();
      
        virtual void setHasWindows(bool hasWindows = false);

        virtual void selectProperty(std::string name);

        virtual QtProperty* findProperty(std::string name);

        virtual bool addSubProperty(QtProperty* prop, QtProperty* subProp);

        virtual bool addSubProperty(Property prop, Property subProp);

        /*
          \brief Verify if have some different property between te::layout::Properties and QtTreePropertyBrowser.
        */
        virtual bool equalsProperties(Properties props);
                    
      protected slots:

        void propertyEditorValueChanged(QtProperty *property, const QVariant &value);

        void onChangeFilter(const QString& filter);

        virtual void onChangeDlgProperty(Property property);

        virtual void onChangeDlgProperty(std::vector<Property> props);

        virtual void onCurrentItemChanged(QtBrowserItem* item);

      signals:

        void changePropertyValue(QtProperty *property, QList<QtBrowserItem*> items);

        void changePropertyValue(Property property);

        void changePropertyValue(std::vector<Property> props);

        void currentItemChanged(QtBrowserItem* item);

      protected:

        virtual QtProperty* addProperty(const Property& property);
      
        virtual void addPropertyItem(QtProperty *property, const QString &id);

        virtual void updateExpandState();

        virtual void createManager();

        virtual void changeVisibility( QList<QtBrowserItem*> items, bool visible );
        
        virtual void blockOpenWindows(bool block);
              
      protected:

        QtTreePropertyBrowser*      m_propertyEditor;
        VariantPropertiesBrowser*   m_variantPropertiesBrowser;
        DialogPropertiesBrowser*    m_dialogPropertiesBrowser;
        QMap<QtProperty*, QString>  m_propertyToId;
        QMap<QString, QtProperty*>  m_idToProperty;
        QMap<QString, bool>         m_idToExpanded;
        FilePathManager*            m_filePathManager;


        /* Custom Types: Dialog Window Type */
        bool                 m_hasWindows;
        bool                 m_changeQtPropertyVariantValue; // true if the change of QtPropertyVariant came from a Property and not of user interaction via Property Browser
    };
  }
}

#endif


