/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file ChangePropertyCommand.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ChangePropertyCommand.h"
#include "../../../../core/pattern/mvc/ItemObserver.h"
#include "../../../../core/pattern/mvc/Observable.h"
#include "../../../../core/pattern/mvc/ItemModelObservable.h"
#include "../../../outside/PropertiesOutside.h"
#include "../../../../core/property/Properties.h"

// Qt
#include <QGraphicsItem>
#include <QPointF>

te::layout::ChangePropertyCommand::ChangePropertyCommand( QGraphicsItem* item, Properties* oldProperties, 
  Properties* newProperties, PropertiesOutside* outside /*= 0*/, QUndoCommand *parent /*= 0 */ ) :
  QUndoCommand(parent),
  m_item(item),
  m_oldProperties(oldProperties),
  m_newProperties(newProperties),
  m_outside(outside)
{

}

te::layout::ChangePropertyCommand::ChangePropertyCommand( std::vector<QGraphicsItem*> items, std::vector<Properties*> allOld, 
  std::vector<Properties*> allNew, PropertiesOutside* outside /*= 0*/, QUndoCommand *parent /*= 0 */ ) :
  QUndoCommand(parent),
  m_item(0),
  m_oldProperties(0),
  m_newProperties(0),
  m_outside(outside),
  m_items(items),
  m_allOldProperties(allOld),
  m_allNewProperties(allNew)
{

}

te::layout::ChangePropertyCommand::~ChangePropertyCommand()
{
  if(m_item)
  {
    if(!m_item->scene())
    {
      delete m_item;
      m_item = 0;
    }
  }

  if(m_oldProperties)
  {
    delete m_oldProperties;
    m_oldProperties = 0;
  }

  if(m_newProperties)
  {
    delete m_newProperties;
    m_newProperties = 0;
  }

  if(!m_allOldProperties.empty())
  {
    std::vector<Properties*>::iterator ito;
    for(ito = m_allOldProperties.begin() ; ito != m_allOldProperties.end() ; ++ito)
    { 
      if(*ito)
      {
        Properties* props = *ito;
        if(props)
        {
          delete props;
          props = 0;
        }
      }
    }
    m_allOldProperties.clear();
  }

  if(!m_allNewProperties.empty())
  {
    std::vector<Properties*>::iterator itn;
    for(itn = m_allNewProperties.begin() ; itn != m_allNewProperties.end() ; ++itn)
    { 
      if(*itn)
      {
        Properties* props = *itn;
        if(props)
        {
          delete props;
          props = 0;
        }
      }
    }
    m_allNewProperties.clear();
  }
}

void te::layout::ChangePropertyCommand::undo()
{
  if(m_item)
  {
    if(!checkItem(m_item, m_oldProperties))
    {
      return;
    }
  }

  if(!checkVectors())
  {
    return;
  }
  
  for(unsigned int i = 0 ; i < m_items.size() ; ++i)
  {
    QGraphicsItem* item = m_items[i];
    Properties* props = m_allOldProperties[i];
    if (item)
    {
      checkItem(item, props);
    }
  }
    
  if(m_outside)
  {
    m_outside->refreshOutside();
  }

  setText(QObject::tr("Change Properties %1")
    .arg(createCommandString(m_item)));
}

void te::layout::ChangePropertyCommand::redo()
{
  if(m_item)
  {
    if(!checkItem(m_item, m_newProperties))
    {
      return;
    }
  }
  
  if(!checkVectors())
  {
    return;
  }

  for(unsigned int i = 0 ; i < m_items.size() ; ++i)
  {
    QGraphicsItem* item = m_items[i];
    Properties* props = m_allNewProperties[i];
    if (item)
    {
      checkItem(item, props);
    }
  }
  
  if(m_outside)
  {
    m_outside->refreshOutside();
  }

  setText(QObject::tr("Change Properties %1")
    .arg(createCommandString(m_item)));
}

QString te::layout::ChangePropertyCommand::createCommandString( QGraphicsItem* item )
{
  if(!m_item)
  {
    if(!m_items.empty())
    {
       return QObject::tr("%1 %2").arg(m_items.size()).arg("items");
    }
    return QObject::tr("%1");
  }
    
  ItemObserver* obs = dynamic_cast<ItemObserver*>(item);

  if(!obs)
    return QObject::tr("%1");

  QPointF pos = m_item->scenePos();

  return QObject::tr("%1 at (%2, %3)")
    .arg(obs->getModel()->getType()->getName().c_str())
    .arg(pos.x()).arg(pos.y());
}

bool te::layout::ChangePropertyCommand::equals( Properties* props1, Properties* props2 )
{
  bool result = true;

  if(props1->getProperties().size() != props2->getProperties().size())
    return false;

  std::vector<Property> prop1 = props1->getProperties();
  std::vector<Property> prop2 = props2->getProperties();

  for(unsigned int i = 0 ; i < props1->getProperties().size() ; ++i)
  {
    Property proper1 = prop1[i];
    Property proper2 = prop2[i];

    if(proper1 == proper2)
    {
      if(proper1.getValue() == proper2.getValue())
      {
        continue;
      }
      else
      {
        result = false;
        break;
      }
    }
  }
  return result;
}

bool te::layout::ChangePropertyCommand::checkItem( QGraphicsItem* item, Properties* props )
{
  if(!item)
    return false;

  ItemObserver* obs = dynamic_cast<ItemObserver*>(item);

  if(!obs)
    return false;

  ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(obs->getModel());

  if(!model)
    return false;

  Properties* propsModel = model->getProperties();  
  if(equals(props, propsModel))
    return false;

  obs->updateProperties(props);
  return true;
}

bool te::layout::ChangePropertyCommand::checkVectors()
{
  if(m_allNewProperties.size() != m_items.size() ||
    m_allOldProperties.size() != m_items.size())
  {
    return false;
  }

  return true;
}
