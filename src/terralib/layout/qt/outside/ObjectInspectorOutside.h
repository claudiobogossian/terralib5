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
  \file ObjectInspectorOutside.h
   
  \brief Tree of names of all the items entered on the scene, MVC components, using Qt to present the name of each item and its class. Object Inspector.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OBJECTINSPECTOR_OUTSIDE_H 
#define __TERRALIB_LAYOUT_INTERNAL_OBJECTINSPECTOR_OUTSIDE_H

// TerraLib
#include "../core/propertybrowser/PropertyBrowser.h"
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../../geometry/Envelope.h"
#include "../../core/Config.h"

// STL
#include <string>

// Qt
#include <QWidget>

class QtProperty;

class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class ItemObserver;
    /*!
    \brief Tree of names of all the items entered on the scene, MVC components, using Qt to present the name of each item and its class. Object Inspector.
	  
	    \ingroup layout

	    \sa te::layout::OutsideObserver
	  */
    class TELAYOUTEXPORT ObjectInspectorOutside : public QWidget, public OutsideObserver
    {
	    Q_OBJECT //for slots/signals

    public:

	    ObjectInspectorOutside(OutsideController* controller, Observable* o, PropertyBrowser* propertyBrowser = 0);
	    
      virtual ~ObjectInspectorOutside();

	    virtual void updateObserver(ContextItem context);
	    
      virtual void setPosition(const double& x, const double& y);
	    
      virtual te::gm::Coord2D getPosition();

      virtual void itemsInspector(QList<QGraphicsItem*> graphicsItems);

      virtual void selectItems(QList<QGraphicsItem*> graphicsItems);

      virtual PropertyBrowser* getObjectInspector();
            
    protected slots:
      
      virtual void onRemoveProperties(std::vector<std::string> names);

      virtual bool hasMoveItemGroup(QList<QGraphicsItem*> graphicsItems);

    protected:
      
      virtual QtProperty* addProperty(QGraphicsItem* item);

      virtual Property createProperty(ItemObserver* item);

      virtual void createSubProperty(QGraphicsItem* item, QtProperty* prop);

      virtual bool hasProperty(Property property);
      
      PropertyBrowser* m_layoutPropertyBrowser;
      QList<QGraphicsItem*> m_graphicsItems;
    };
  }
}

#endif
