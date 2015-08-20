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
  \file AddCommand.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AddCommand.h"
#include "../../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../../core/pattern/mvc/AbstractItemModel.h"
#include "../../../../core/enum/EnumType.h"
#include "../../../../core/pattern/mvc/Observable.h"
#include "../../Scene.h"

// Qt
#include <QGraphicsScene>
#include <QGraphicsItem>

te::layout::AddCommand::AddCommand( QGraphicsItem* item, QUndoCommand *parent /*= 0*/ ) :
  QUndoCommand(parent),
  m_scene(0),
  m_item(item)
{
  if(!m_item)
    return;

  m_scene = item->scene();

  m_initialPosition = m_item->scenePos();
  setText(QObject::tr("Add %1").arg(createCommandString(m_item, m_initialPosition)));
}

te::layout::AddCommand::~AddCommand()
{
  
}

void te::layout::AddCommand::undo()
{
  Scene* scene = dynamic_cast<Scene*>(m_scene);

  if(m_item->scene() != scene)
    return;

  m_scene->removeItem(m_item);
  scene->addItemStackWithoutScene(m_item);
  m_scene->update();
}

void te::layout::AddCommand::redo()
{
  Scene* scene = dynamic_cast<Scene*>(m_scene);

  if(!scene || !m_item)
    return;

  /* Checks if the item is already 
     added to the scene */
  if(m_item->scene() == m_scene)
    return;
    
  scene->insertItem(m_item);
  scene->removeItemStackWithoutScene(m_item);

  m_item->setPos(m_initialPosition);
  m_scene->clearSelection();
  m_scene->update();
}

QString te::layout::AddCommand::createCommandString( QGraphicsItem* item, const QPointF &pos )
{
  if(!m_item)
    return QObject::tr("%1");

  AbstractItemView* obs = dynamic_cast<AbstractItemView*>(item);

  if(!obs)
    return QObject::tr("%1");
  
  return QObject::tr("%1 at (%2, %3)")
    .arg(obs->getController()->getModel()->getType()->getName().c_str())
    .arg(pos.x()).arg(pos.y());
}



