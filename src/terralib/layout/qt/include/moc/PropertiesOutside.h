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
  \file PropertiesOutside.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_OUTSIDE_H 
#define __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_OUTSIDE_H

// Qt
#include <QDockWidget>
#include "../../../../../../third-party/qt/propertybrowser/qtpropertybrowser.h"

// TerraLib
#include "OutsideObserver.h"
#include "../../../../geometry/Envelope.h"

// STL
#include <vector>

class QGraphicsItem;
class QWidget;
class QGraphicsProxyWidget;

class QtTreePropertyBrowser;
class QtVariantProperty;
class QtProperty;
class QtVariantPropertyManager;

class QtBrowserIndex;
class QLabel;
class QLineEdit;
class QToolButton;
class PropertyBrowser;

namespace te
{
  namespace layout
  {
    class PropertiesItemPropertyBrowser;
    class Properties;
    class SharedProperties;
    class MapItem;
    class MapModel;

    class PropertiesOutside : public QDockWidget, public OutsideObserver
    {
	    Q_OBJECT //for slots/signals

    public:

	    PropertiesOutside(OutsideController* controller, Observable* o);
	    virtual ~PropertiesOutside();

	    virtual void updateObserver(ContextItem context);
	    virtual void setPosition(const double& x, const double& y);
	    virtual te::gm::Coord2D getPosition();

      virtual void itemsSelected(QList<QGraphicsItem*> graphicsItems, QList<QGraphicsItem*> allItems);

      private slots:

        void onChangePropertyValue(Property property);

    protected:

      virtual void	closeEvent ( QCloseEvent * event );
      virtual void createLayout();
      virtual Properties* intersection(QList<QGraphicsItem*> graphicsItems, bool& gridWindow);
      virtual Properties* sameProperties(QList<QGraphicsItem*> graphicsItems, bool& gridWindow);
      virtual void contains(std::vector<Properties*>::iterator itend, 
        std::vector<Properties*>::iterator it, std::string name, bool& result);
      virtual std::vector<Properties*> getAllProperties(QList<QGraphicsItem*> graphicsItems, bool& gridWindow);
      virtual std::vector<MapItem*> getMapList(QList<QGraphicsItem*> graphicsItems, std::string currentName = "");
      virtual void addDynamicOptions(Property& property, std::vector<std::string> list);
      virtual void checkDynamicProperty(Property& property, QList<QGraphicsItem*> graphicsItems);
      virtual void mapNameDynamicProperty(Property& property, QList<QGraphicsItem*> graphicsItems);
      virtual void changeVisitor(Property property);
      virtual MapModel* getMapModel(std::string nameMap);

    protected:

      PropertiesItemPropertyBrowser* m_layoutPropertyBrowser;
      
      QList<QGraphicsItem*> m_graphicsItems;
      QList<QGraphicsItem*> m_allItems;
      QLabel* m_nameLabel;
      QLineEdit* m_propertyFilterEdit;
      QToolButton* m_configurePropertyEditor;
      bool m_updatingValues;
      SharedProperties* m_sharedProps;
    };
  }
}


#endif
