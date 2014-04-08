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
  \file QObjectInspectorWindowOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "QObjectInspectorWindowOutside.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "QLayoutScene.h"
#include "LayoutOutsideModelObservable.h"
#include "LayoutItemObserver.h"
#include "LayoutOutsideObserver.h"
#include "LayoutOutsideController.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../../../third-party/qt/propertybrowser/qtvariantproperty.h"
#include "../../../../../../third-party/qt/propertybrowser/qttreepropertybrowser.h"

//Qt
#include <QGraphicsWidget>

te::layout::QObjectInspectorWindowOutside::QObjectInspectorWindowOutside( LayoutOutsideController* controller, LayoutObservable* o ) :
	QDockWidget("", 0, 0),
	LayoutOutsideObserver(controller, o)
{
	te::gm::Envelope box = m_model->getBox();	
	setBaseSize(box.getWidth(), box.getHeight());
	setVisible(false);
	setWindowTitle("Layout - Inspetor de Objetos");
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);

  //
  m_variantInspectorObjectManager = new QtVariantPropertyManager(this);
  connect(m_variantInspectorObjectManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
  this, SLOT(propertyEditorValueChanged(QtProperty *, const QVariant &)));

  QtVariantEditorFactory* variantObjectInspectorFactory = new QtVariantEditorFactory(this);
  m_objectInspector = new QtTreePropertyBrowser(this);
  m_objectInspector->setFactoryForManager(m_variantInspectorObjectManager, variantObjectInspectorFactory);
  m_objectInspector->setResizeMode(QtTreePropertyBrowser::ResizeToContents);

  setWidget(m_objectInspector);
}

te::layout::QObjectInspectorWindowOutside::~QObjectInspectorWindowOutside()
{

}

void te::layout::QObjectInspectorWindowOutside::updateObserver( ContextLayoutItem context )
{
	setVisible(context.getShow());
	if(context.getShow() == true)
		show();
	else
		hide();
}

void te::layout::QObjectInspectorWindowOutside::setPosition( const double& x, const double& y )
{
	move(x,y);
	refresh();
}

te::gm::Coord2D te::layout::QObjectInspectorWindowOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::QObjectInspectorWindowOutside::propertyEditorValueChanged( QtProperty *property, const QVariant &value )
{
 
}

void te::layout::QObjectInspectorWindowOutside::updateExpandState()
{
  QList<QtBrowserItem *> list = m_objectInspector->topLevelItems();
  QListIterator<QtBrowserItem *> it(list);
  while (it.hasNext()) {
    QtBrowserItem *item = it.next();
    QtProperty *prop = item->property();
    m_idToExpanded[m_propertyToId[prop]] = m_objectInspector->isExpanded(item);
  }
}

void te::layout::QObjectInspectorWindowOutside::itemsInspector(QList<QGraphicsItem*> graphicsItems)
{
  updateExpandState();

  QMap<QtProperty *, QString>::ConstIterator itProp = m_propertyToId.constBegin();
  while (itProp != m_propertyToId.constEnd()) 
  {
    delete itProp.key();
    itProp++;
  }
  m_propertyToId.clear();
  m_idToProperty.clear();
  m_graphicsItems = graphicsItems;
    
  QtVariantProperty *property;
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

      LayoutItemObserver* lItem = dynamic_cast<LayoutItemObserver*>(item);
      if(lItem)
      {
        LayoutProperties* properties = const_cast<LayoutProperties*>(lItem->getProperties());

        foreach(LayoutProperty p, properties->getProperties())
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

void te::layout::QObjectInspectorWindowOutside::addProperty(QtVariantProperty *property, const QString &id)
{
  m_propertyToId[property] = id;
  m_idToProperty[id] = property;
  QtBrowserItem *item = m_objectInspector->addProperty(property);
  if (m_idToExpanded.contains(id))
    m_objectInspector->setExpanded(item, m_idToExpanded[id]);
}
