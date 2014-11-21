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
#include "../../core/pattern/singleton/Context.h"
#include "../../core/AbstractScene.h"
#include "../core/Scene.h"
#include "../../core/pattern/mvc/OutsideModelObservable.h"
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../core/pattern/mvc/OutsideController.h"
#include "../../../geometry/Envelope.h"
#include "../../core/enum/Enums.h"

//Qt
#include <QGraphicsWidget>
#include <QVBoxLayout>
#include <QGroupBox>

te::layout::ObjectInspectorOutside::ObjectInspectorOutside( OutsideController* controller, Observable* o ) :
	QWidget(0),
	OutsideObserver(controller, o)
{
	te::gm::Envelope box = m_model->getBox();	
	setBaseSize(box.getWidth(), box.getHeight());
	setVisible(false);
	setWindowTitle("Layout - Inspetor de Objetos");
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  m_layoutPropertyBrowser = new ObjectInspectorPropertyBrowser;
    
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->addWidget(m_layoutPropertyBrowser->getPropertyEditor());

  connect(m_layoutPropertyBrowser->getPropertyEditor(), SIGNAL(currentItemChanged (QtBrowserItem *)), 
          this, SLOT(onCurrentItemChanged (QtBrowserItem *)));
  
  m_layoutPropertyBrowser->getPropertyEditor()->installEventFilter(this);

  QGroupBox* groupBox = new QGroupBox;
  groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  groupBox->setLayout(layout);

  QVBoxLayout* layoutAll = new QVBoxLayout(this);
  layoutAll->setMargin(0);

  layoutAll->addWidget(groupBox);

  setLayout(layoutAll);
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
    
  int zValue = 0;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {			
      zValue = 0;

      QGraphicsItem* parentItem = item->parentItem();          
      zValue = item->zValue();

      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {        

        if(lItem->getModel()->getType() == enumObj->getPaperItem())
        {
          continue;
        }

        QGraphicsObject* itemObj = dynamic_cast<QGraphicsObject*>(item);         

        if(!itemObj)
          break;

        Property pro_class;
        pro_class.setName(lItem->getName());
        pro_class.setId("");
        pro_class.setValue(lItem->getNameClass(), dataType->getDataTypeString());
        
        m_layoutPropertyBrowser->addProperty(pro_class);
     
        zValue = itemObj->zValue();        
      }
    }
  }

  update();
}

void te::layout::ObjectInspectorOutside::onCurrentItemChanged(QtBrowserItem *current)
{
  if(!current)
    return;

  QtProperty* proper = current->property();
  if(!proper)
    return;

  Property prop = m_layoutPropertyBrowser->getProperty(proper->propertyName().toStdString());

  if(prop.isNull())
    return;

  emit currentItemChanged(prop);
  emit currentItemChanged(prop.getName());
}

void te::layout::ObjectInspectorOutside::onRemoveProperties( std::vector<std::string> names )
{
  std::vector<std::string>::iterator it;

  for(it = names.begin() ; it != names.end() ; ++it)
  {
    Property prop = m_layoutPropertyBrowser->getProperty(*it);
    m_layoutPropertyBrowser->removeProperty(prop);
  }
}

void te::layout::ObjectInspectorOutside::selectItems( QList<QGraphicsItem*> graphicsItems )
{
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      ItemObserver* iOb = dynamic_cast<ItemObserver*>(item);
      if(iOb)
      {
        m_layoutPropertyBrowser->selectProperty(iOb->getName());
      }
    }
  }
}
