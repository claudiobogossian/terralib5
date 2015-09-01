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

// STL
#include <set>

te::edit::AddCommand::AddCommand(std::vector<Feature*> items, te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer,
  QUndoCommand *parent) :
  QUndoCommand(parent)
, m_display(display)
, m_layer(layer)
, m_addItems(items)
, m_nextFeature(0)
, m_previousFeature(0)
{
  //std::set<std::string> ids;
  //for (std::size_t i = 0; i < items.size(); i++)
    //ids.insert(items[i]->getId()->getValueAsString())

  //m_initialPosition = m_item->getGeometry()->getMBR()->getCenter();

  setText(QObject::tr("Add %1").arg(createCommandString(m_initialPosition)));

}

te::edit::AddCommand::~AddCommand()
{}

void  te::edit::AddCommand::undo()
{
  
  //if (m_item)
  //{
  //  setText(QObject::tr("Add %1")
  //    .arg(createCommandString(m_initialPosition)));
  //}
  
  if (m_addItems.empty())
    return;

  m_previousFeature = m_addItems.size() - 2;

  if (RepositoryManager::getInstance().hasIdentify(m_layer->getId(), m_addItems[m_previousFeature]->getId()) == true)
  {
    std::size_t count = 0;
    Feature* f = new Feature(m_addItems[m_previousFeature]->getId(), m_addItems[m_previousFeature]->getGeometry(), m_addItems[m_previousFeature]->getOperationType());

    for (std::size_t i = 0; i < m_addItems.size(); i++)
    {
      if (m_addItems[i]->getId()->getValueAsString() == f->getId()->getValueAsString())
        count++;
    }

    if (count == 2)
      RepositoryManager::getInstance().removeFeature(m_layer->getId(), m_addItems[m_previousFeature]->getId());
    else
      RepositoryManager::getInstance().addFeature(m_layer->getId(), f->clone());

    draw();

  }

}

void te::edit::AddCommand::redo()
{
  bool resultFound = false;

  //if (m_item)
  //{
  //  setText(QObject::tr("Add %1")
  //    .arg(createCommandString(m_initialPosition)));
  //}
  
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

  m_nextFeature = m_addItems.size() - 1;

  //no makes redo while the command is not on the stack
  if (resultFound)
  {

    Feature* f = new Feature(m_addItems[m_nextFeature]->getId(), m_addItems[m_nextFeature]->getGeometry(), m_addItems[m_nextFeature]->getOperationType());

    RepositoryManager::getInstance().addFeature(m_layer->getId(), f->clone());

    draw();

  }

  
}

QString te::edit::AddCommand::createCommandString(const te::gm::Coord2D &pos)
{
  //if (!m_item)
    return QObject::tr("%1");

  //return QObject::tr("%1: %2 at (%3, %4)")
    //.arg(m_item->getGeometry()->getGeometryType().c_str())
    //.arg(m_item->getId()->getValueAsString().substr(0, 5).c_str() + QString("..."))
    //.arg(pos.getX()).arg(pos.getY());
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