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
#include "../core/propertybrowser/VariantPropertiesBrowser.h"
#include "../core/propertybrowser/DialogPropertiesBrowser.h"
#include "../item/MovingItemGroup.h"
#include "../core/propertybrowser/PropertyBrowser.h"

//Qt
#include <QGraphicsWidget>
#include <QVBoxLayout>
#include <QGroupBox>

#include <QtPropertyBrowser/QtProperty>
#include <QtPropertyBrowser/QtTreePropertyBrowser>

te::layout::ObjectInspectorOutside::ObjectInspectorOutside( OutsideController* controller, Observable* o, PropertyBrowser* propertyBrowser ) :
  QWidget(0),
  OutsideObserver(controller, o)
{
  te::gm::Envelope box = m_model->getBox();
  setBaseSize(box.getWidth(), box.getHeight());
  setVisible(false);
  setWindowTitle("Object Inspector");
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  if(!propertyBrowser)
    m_layoutPropertyBrowser = new PropertyBrowser;
  else
    m_layoutPropertyBrowser = propertyBrowser;

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->addWidget(m_layoutPropertyBrowser->getPropertyEditor());
  
  QGroupBox* groupBox = new QGroupBox;
  groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  groupBox->setLayout(layout);

  QVBoxLayout* layoutAll = new QVBoxLayout(this);
  layoutAll->setMargin(0);

  layoutAll->addWidget(groupBox);

  setLayout(layoutAll);

  m_layoutPropertyBrowser->getPropertyEditor()->setAlternatingRowColors(false);
}

te::layout::ObjectInspectorOutside::~ObjectInspectorOutside()
{

}

void te::layout::ObjectInspectorOutside::updateObserver( ContextItem context )
{
  setVisible(context.isShow());
  if(context.isShow() == true)
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
  if(hasMoveItemGroup(graphicsItems))
  {
    return;
  }

  m_layoutPropertyBrowser->clearAll();

  m_graphicsItems = graphicsItems;

  if(m_graphicsItems.empty())
    return;

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  
  foreach(QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
      if(lItem)
      {        
        if(!lItem->getModel())
        {
          continue;
        }

        if(lItem->getModel()->getType() == enumObj->getPaperItem())
        {
          continue;
        }

        // No add MoveItemGroup, because is alive just for mouse move
        MovingItemGroup* moving =  dynamic_cast<MovingItemGroup*>(item);
        if(moving)
        {
          continue;
        }

        addProperty(item);    
      }
    }
  }

  update();
}

void te::layout::ObjectInspectorOutside::onRemoveProperties( std::vector<std::string> names )
{
  std::vector<std::string>::iterator it;

  for(it = names.begin() ; it != names.end() ; ++it)
  {
    Property prop = m_layoutPropertyBrowser->getVariantPropertiesBrowser()->getProperty(*it);

    if(prop.isNull())
    {
      prop = m_layoutPropertyBrowser->getDialogPropertiesBrowser()->getProperty(*it);
    }

    m_layoutPropertyBrowser->removeProperty(prop);
  }
}

void te::layout::ObjectInspectorOutside::selectItems( QList<QGraphicsItem*> graphicsItems )
{
  foreach(QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      ItemObserver* iOb = dynamic_cast<ItemObserver*>(item);
      if(iOb)
      {
        if(iOb->getModel())
        {
          m_layoutPropertyBrowser->selectProperty(iOb->getModel()->getName());
        }
      }
    }
  }
}

te::layout::PropertyBrowser* te::layout::ObjectInspectorOutside::getObjectInspector()
{
  return m_layoutPropertyBrowser;
}

bool te::layout::ObjectInspectorOutside::hasMoveItemGroup( QList<QGraphicsItem*> graphicsItems )
{
  bool result = false;

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();

  if(!enumObj)
  {
    return result;
  }

  foreach(QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      ItemObserver* movingItem = dynamic_cast<ItemObserver*>(item);

      if(movingItem->getModel()->getType() == enumObj->getMovingItemGroup())
      {
        result = true;
        break;
      }
    }
  }

  return result;
}

QtProperty* te::layout::ObjectInspectorOutside::addProperty( QGraphicsItem* item )
{
  QtProperty* prop = 0;

  if(!item)
  {
    return prop;
  }

  ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
  if(!lItem)
  {
    return prop;
  }
  
  Property pro_class = createProperty(lItem);

  if(hasProperty(pro_class))
  {
    return prop;
  }

  if(item->parentItem())
  {
    return prop;
  }

  prop = m_layoutPropertyBrowser->addProperty(pro_class); 

  if(!item->childItems().isEmpty())
  {
     createSubProperty(item, prop);
  }

  return prop;
}

void te::layout::ObjectInspectorOutside::createSubProperty( QGraphicsItem* item, QtProperty* prop )
{
  if(!item || !prop)
  {
    return;
  }

  foreach(QGraphicsItem *item, item->childItems()) 
  {
    if(!item)
    {
      continue;
    }

    ItemObserver* lItem = dynamic_cast<ItemObserver*>(item);
    if(!lItem)
    {
      continue;
    }

    Property pro_class = createProperty(lItem);
    if(hasProperty(pro_class))
    {
      continue;
    }

    QtProperty* subProp = m_layoutPropertyBrowser->addProperty(pro_class); 
    if(!subProp)
    {
      continue;
    }

    m_layoutPropertyBrowser->addSubProperty(prop, subProp);
  }  
}

te::layout::Property te::layout::ObjectInspectorOutside::createProperty( ItemObserver* item )
{
  Property pro_class;
  if(!item)
  {
    return pro_class;
  }

  if(!item->getModel())
  {
    return pro_class;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  if(!dataType)
  {
    return pro_class;
  }

  pro_class.setName(item->getModel()->getName());
  pro_class.setValue(item->getNameClass(), dataType->getDataTypeString());
  pro_class.setEditable(false);

  return pro_class;
}

bool te::layout::ObjectInspectorOutside::hasProperty( Property property )
{
  std::string name = property.getName();
  std::string label = property.getLabel();

  QtProperty* prop = m_layoutPropertyBrowser->findProperty(name);
  if(!prop)
  {
    prop = m_layoutPropertyBrowser->findProperty(label);
  }

  if(!prop)
  {
    return false;
  }

  return true;
}








