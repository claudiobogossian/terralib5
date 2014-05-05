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
  \file ObjectInspectorOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ObjectInspectorOutside.h"
#include "Context.h"
#include "AbstractScene.h"
#include "Scene.h"
#include "OutsideModelObservable.h"
#include "ItemObserver.h"
#include "OutsideObserver.h"
#include "OutsideController.h"
#include "../../../../geometry/Envelope.h"
#include "ObjectInspectorPropertyBrowser.h"

//Qt
#include <QGraphicsWidget>

te::layout::ObjectInspectorOutside::ObjectInspectorOutside( OutsideController* controller, Observable* o ) :
	QDockWidget("", 0, 0),
	OutsideObserver(controller, o)
{
	te::gm::Envelope box = m_model->getBox();	
	setBaseSize(box.getWidth(), box.getHeight());
	setVisible(false);
	setWindowTitle("Layout - Inspetor de Objetos");
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);

  m_layoutPropertyBrowser = new ObjectInspectorPropertyBrowser;

  setWidget(m_layoutPropertyBrowser->getPropertyEditor());
}

te::layout::ObjectInspectorOutside::~ObjectInspectorOutside()
{

}

void te::layout::ObjectInspectorOutside::updateObserver( ContextItem context )
{
	setVisible(context.getShow());
	if(context.getShow() == true)
		show();
	else
		hide();
}

void te::layout::ObjectInspectorOutside::setPosition( const double& x, const double& y )
{
	move(x,y);
	refresh();
}

te::gm::Coord2D te::layout::ObjectInspectorOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::ObjectInspectorOutside::itemsInspector(QList<QGraphicsItem*> graphicsItems)
{
  m_layoutPropertyBrowser->clearAll();

  m_graphicsItems = graphicsItems;

  if(m_graphicsItems.empty())
    return;
    
  //QtVariantProperty *property;
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {			
      QGraphicsWidget* outside = dynamic_cast<QGraphicsWidget*>(item);

      if(outside)
        continue;

      int zValue = 0;

      QGraphicsItem* parentItem = item->parentItem();          
      zValue = item->zValue();

      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {
        Properties* properties = const_cast<Properties*>(lItem->getProperties());

        foreach(Property p, properties->getProperties())
        {
          QGraphicsObject* itemObj = dynamic_cast<QGraphicsObject*>(item);         

          if(!itemObj)
            break;

          /*
            Ex. de Hierarquia:
              task1->addSubProperty(priority);
              task1->addSubProperty(reportType)
          */

          /*property = m_variantInspectorObjectManager->addProperty(QVariant::String, tr(itemObj->metaObject()->className()));
          property->setValue(p.getLabel().c_str());
          addProperty(property, QLatin1String(itemObj->objectName().toStdString().c_str()));*/

          zValue = itemObj->zValue();
        }
      }
    }
  }

  update();
}
