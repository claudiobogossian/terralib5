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
  \file PropertiesOutside.h
   
  \brief Properties tree for any item, MVC component, using Qt for presentation and editing.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_OUTSIDE_H 
#define __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_OUTSIDE_H

// Qt
#include <QWidget>

// QtPropertyBrowser
class QtTreePropertyBrowser;

// TerraLib
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../../geometry/Envelope.h"
#include "../../core/Config.h"

// STL
#include <vector>

class QGraphicsItem;
class QGraphicsProxyWidget;

class QtBrowserIndex;
class QLabel;
class QLineEdit;
class QToolButton;

namespace te
{
  namespace layout
  {
    class PropertyBrowser;
    class Properties;
    class SharedProperties;
    class MapItem;
    class MapModel;
    class PropertiesUtils;

    /*!
    \brief Properties tree for any item, MVC component, using Qt for presentation and editing.
	  
	    \ingroup layout

	    \sa te::layout::OutsideObserver
	  */
    class TELAYOUTEXPORT PropertiesOutside : public QWidget, public OutsideObserver
    {
	    Q_OBJECT //for slots/signals

      public:

	      PropertiesOutside(OutsideController* controller, Observable* o, PropertyBrowser* propertyBrowser = 0);

	      virtual ~PropertiesOutside();

	      virtual void updateObserver(ContextItem context);

	      virtual void setPosition(const double& x, const double& y);

	      virtual te::gm::Coord2D getPosition();

        virtual void itemsSelected(QList<QGraphicsItem*> graphicsItems, QList<QGraphicsItem*> allItems);

        virtual void refreshOutside();

      protected slots:

        virtual void onClear(std::vector<std::string> names);

      private slots:

        void onChangePropertyValue(Property property);

        void onChangePropertyValue(std::vector<Property> props);

      protected:

        virtual void	closeEvent ( QCloseEvent * event );

        virtual void createLayout();

        virtual void changeMapVisitable(Property property);

        virtual MapModel* getMapModel(std::string nameMap);

        virtual bool sendPropertyToSelectedItems(Property property);

      protected:

        PropertyBrowser* m_layoutPropertyBrowser;
      
        QList<QGraphicsItem*> m_graphicsItems;
        QList<QGraphicsItem*> m_allItems;
        QLabel* m_nameLabel;
        QLineEdit* m_propertyFilterEdit;
        QToolButton* m_configurePropertyEditor;
        bool m_updatingValues;
        SharedProperties* m_sharedProps;
        PropertiesUtils*      m_propUtils;
      };
  }
}

#endif
