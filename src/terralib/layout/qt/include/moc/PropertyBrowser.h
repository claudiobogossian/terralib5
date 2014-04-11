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
    class PropertyBrowser : public QObject
    {
	    Q_OBJECT //for slots/signals

    public:

	    PropertyBrowser(QWidget *parent = 0);
	    virtual ~PropertyBrowser();

      QtTreePropertyBrowser* getPropertyEditor();
      QtVariantPropertyManager* getVariantPropertyManager();

      void clearAll();

      virtual bool addProperty(Property property);
      virtual bool removeProperty(Property property);

      virtual Property getProperty(std::string name);
      virtual Property getPropertyFromPosition(int pos);
      
      private slots:

        void propertyEditorValueChanged(QtProperty *property, const QVariant &value);
        void onChangeFilter(const QString& filter);
        
      signals:

        void changePropertyValue(QtProperty *property, QList<QtBrowserItem*> items);

    protected:
      
      virtual void addPropertyItem(QtVariantProperty *property, const QString &id);
      virtual void updateExpandState();
      virtual void createManager();
      virtual void changeVisibility( QList<QtBrowserItem*> items, bool visible );

    protected:

      QtTreePropertyBrowser* m_propertyEditor;
      QtVariantPropertyManager* m_variantPropertyEditorManager;      
      QMap<QtProperty*, QString> m_propertyToId;
      QMap<QString, QtVariantProperty*> m_idToProperty;
      QMap<QString, bool> m_idToExpanded;
    };
  }
}

#endif
