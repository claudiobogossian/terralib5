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
  \file MoveCommand.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MoveCommand.h"
#include "../../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../../core/pattern/mvc/AbstractItemModel.h"
#include "../../../../core/enum/EnumType.h"
#include "../../Scene.h"
#include "../../../../core/pattern/singleton/Context.h"

// Qt
#include <QGraphicsItem>

te::layout::MoveCommand::MoveCommand( QGraphicsItem* item, const QPointF oldPos, 
  QUndoCommand *parent /*= 0*/ ) :
  QUndoCommand(parent),
  m_item(item)
{
  m_newPos = m_item->scenePos();
  m_oldPos = oldPos;
}

te::layout::MoveCommand::MoveCommand( std::map<QGraphicsItem*, QPointF> items, 
  QUndoCommand *parent /*= 0*/ ) :
  QUndoCommand(parent),
  m_item(0),
  m_moveItems(items)
{
  std::map<QGraphicsItem*, QPointF>::iterator it;
  for (it = m_moveItems.begin(); it != m_moveItems.end(); ++it)
  {
    QGraphicsItem* item = it->first;
    QPointF ptNew = item->scenePos();
    m_itemsPoints.push_back(ptNew);
  }
}

te::layout::MoveCommand::~MoveCommand()
{
  
}

void te::layout::MoveCommand::undo()
{
  if(m_item)
  {
    m_item->setPos(m_oldPos);
    m_item->scene()->update();
    setText(QObject::tr("Move %1")
      .arg(createCommandString(m_item, m_oldPos)));
  }

  if(m_moveItems.empty())
    return;

  int size = m_moveItems.size();

  std::map<QGraphicsItem*, QPointF>::iterator it;
  for (it = m_moveItems.begin(); it != m_moveItems.end(); ++it)
  {
    QGraphicsItem* item = it->first;
    QPointF ptOld = it->second;
    item->setPos(ptOld);
  }

  Scene* sc = dynamic_cast<Scene*>(Context::getInstance().getScene());
  sc->update();
  setText(QObject::tr("Move %1").arg(size));
}

void te::layout::MoveCommand::redo()
{
  if(m_item)
  {
    m_item->setPos(m_newPos);
    setText(QObject::tr("Move %1")
      .arg(createCommandString(m_item, m_newPos)));
  }

  if(m_moveItems.empty())
    return;

  if(m_moveItems.size() != m_itemsPoints.size())
    return;

  Scene* sc = dynamic_cast<Scene*>(Context::getInstance().getScene());
  if(!sc)
    return;

  if(!sc->getUndoStack())
    return;

  bool resultFound = false;
  
  for(int i = 0 ; i < sc->getUndoStack()->count() ; ++i)
  {
    const QUndoCommand* cmd = sc->getUndoStack()->command(i);
    if(cmd == this)
    {
      resultFound = true; 
    }
  }

  int size = m_moveItems.size();

  //no makes redo while the command is not on the stack
  if(resultFound)
  {
    int index = 0;

    std::map<QGraphicsItem*, QPointF>::iterator it;
    for (it = m_moveItems.begin(); it != m_moveItems.end(); ++it)
    {
      QGraphicsItem* item = it->first;
      QPointF ptNew = m_itemsPoints[index];
      item->setPos(ptNew);
      index++;
    }
  }

  sc->update();
  setText(QObject::tr("Move %1").arg(size));
}

QString te::layout::MoveCommand::createCommandString( QGraphicsItem* item, const QPointF &pos )
{
  if(!m_item)
    return QObject::tr("%1");

  AbstractItemView* obs = dynamic_cast<AbstractItemView*>(item);

  if(!obs)
    return QObject::tr("%1");

  return QObject::tr("%1 at (%2, %3)")
    .arg(obs->getController()->getProperties().getTypeObj()->getName().c_str())
    .arg(pos.x()).arg(pos.y());
}
