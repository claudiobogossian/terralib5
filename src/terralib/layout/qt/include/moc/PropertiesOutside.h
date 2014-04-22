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

    class PropertiesOutside : public QDockWidget, public OutsideObserver
    {
	    Q_OBJECT //for slots/signals

    public:

	    PropertiesOutside(OutsideController* controller, Observable* o);
	    virtual ~PropertiesOutside();

	    virtual void updateObserver(ContextItem context);
	    virtual void setPosition(const double& x, const double& y);
	    virtual te::gm::Coord2D getPosition();

      virtual void itemsSelected(QList<QGraphicsItem*> graphicsItems);
      
    protected:
      virtual void createLayout();
          
    protected:

      PropertiesItemPropertyBrowser* m_layoutPropertyBrowser;
      
      QList<QGraphicsItem*> m_graphicsItems;
      QLabel* m_nameLabel;
      QLineEdit* m_propertyFilterEdit;
      QToolButton* m_configurePropertyEditor;
      bool m_updatingValues;
    };
  }
}


#endif
