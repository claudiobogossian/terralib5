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
  \file MoveCommand.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MoveCommand.h"
#include "../../../../core/pattern/mvc/ItemObserver.h"
#include "../../../../core/enum/EnumType.h"
#include "../../../../core/pattern/mvc/Observable.h"
#include "../../Scene.h"

// Qt
#include <QGraphicsItem>

te::layout::MoveCommand::MoveCommand( QGraphicsItem* item, const QPointF oldPos, 
  QUndoCommand *parent /*= 0*/ ) :
  m_item(item)
{
  m_newPos = m_item->scenePos();
  m_oldPos = oldPos;
}

te::layout::MoveCommand::~MoveCommand()
{
  
}

void te::layout::MoveCommand::undo()
{
  m_item->setPos(m_oldPos);
  m_item->scene()->update();
  setText(QObject::tr("Move %1")
    .arg(createCommandString(m_item, m_oldPos)));
}

void te::layout::MoveCommand::redo()
{
  m_item->setPos(m_newPos);
  setText(QObject::tr("Move %1")
    .arg(createCommandString(m_item, m_newPos)));
}

QString te::layout::MoveCommand::createCommandString( QGraphicsItem* item, const QPointF &pos )
{
  if(!m_item)
    return QObject::tr("%1");

  ItemObserver* obs = dynamic_cast<ItemObserver*>(item);

  if(!obs)
    return QObject::tr("%1");

  return QObject::tr("%1 at (%2, %3)")
    .arg(obs->getModel()->getType()->getName().c_str())
    .arg(pos.x()).arg(pos.y());
}

QGraphicsItem* te::layout::MoveCommand::getItem()
{
  return m_item;
}
