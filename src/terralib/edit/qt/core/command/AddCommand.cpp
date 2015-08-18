/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

This file is part of the TerraLib - a Framework for building GIS enabled applications.

TerraLib is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.

TerraLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See them_initialPosition
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with TerraLib. See COPYING. If not, write to
TerraLib Team at <terralib-team@terralib.org>.
*/

/*!
\file AddCommand.cpp

\brief

*/

// TerraLib
#include "../../../../dataaccess/dataset/ObjectId.h"
#include "../../../../geometry/Geometry.h"
#include "../../../Utils.h"
#include "../../../RepositoryManager.h"
#include "../../Utils.h"
#include "../../Renderer.h"
#include "../UndoStackManager.h"
#include "AddCommand.h"

te::edit::AddCommand::AddCommand(std::map<std::string, Feature*> items, Feature* item, te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer,
  QUndoCommand *parent) :
  QUndoCommand(parent)
, m_display(display)
, m_item(item)
, m_layer(layer)
, m_addItems(items)
{

  if (!m_item)
    return;

  m_initialPosition = m_item->getGeometry()->getMBR()->getCenter();

  setText(QObject::tr("Add %1").arg(createCommandString(m_initialPosition)));

}

te::edit::AddCommand::~AddCommand()
{}

void  te::edit::AddCommand::undo()
{
  
  if (m_item)
  {
    setText(QObject::tr("Add %1")
      .arg(createCommandString(m_initialPosition)));
  }
  
  if (m_addItems.empty())
    return;

  std::map<std::string, Feature*>::iterator it;

  for (it = m_addItems.begin(); it != m_addItems.end(); ++it)
  {
    if (it->first == m_item->getId()->getValueAsString()) 
    {
      RepositoryManager::getInstance().removeFeature(m_layer->getId(), m_item->getId()->clone());

      draw();

      break;
    }
  }
  
}

void te::edit::AddCommand::redo()
{
  bool resultFound = false;

  if (m_item)
  {
    setText(QObject::tr("Add %1")
      .arg(createCommandString(m_initialPosition)));
  }
  
  if (m_addItems.empty())
    return;

  if (!UndoStackManager::getInstance().getUndoStack())
    return;
  
  for (int i = 0; i < UndoStackManager::getInstance().getUndoStack()->count(); ++i)
  {
    const QUndoCommand* cmd = UndoStackManager::getInstance().getUndoStack()->command(i);
    if (cmd == this)
    {
      resultFound = true;
    }
  }

  //no makes redo while the command is not on the stack
  if (resultFound)
  {

    std::map<std::string, Feature*>::iterator it;

    for (it = m_addItems.begin(); it != m_addItems.end(); ++it)
    {
      if (it->first == m_item->getId()->getValueAsString())
      {
        RepositoryManager::getInstance().addFeature(m_layer->getId(), m_item->clone());

        draw();

        break;
      }
    }

  }
  
}

QString te::edit::AddCommand::createCommandString(const te::gm::Coord2D &pos)
{
  if (!m_item)
    return QObject::tr("%1");

  return QObject::tr("%1: %2 at (%3, %4)")
    .arg(m_item->getGeometry()->getGeometryType().c_str())
    .arg(m_item->getId()->getValueAsString().substr(0, 5).c_str() + QString("..."))
    .arg(pos.getX()).arg(pos.getY());
}

void te::edit::AddCommand::draw()
{
  const te::gm::Envelope& env = m_display->getExtent();
  if (!env.isValid())
    return;

  // Clear!
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  // Initialize the renderer
  Renderer& renderer = Renderer::getInstance();
  renderer.begin(draft, env, m_display->getSRID());

  // Draw the layer edited geometries
  renderer.drawRepository(m_layer->getId(), env, m_display->getSRID());

  renderer.end();

  m_display->repaint();

}