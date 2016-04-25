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
\file UpdateCommand.cpp

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
#include "UpdateCommand.h"

te::edit::UpdateCommand::UpdateCommand(std::vector<Feature*> items, te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer,
  QUndoCommand *parent) :
  QUndoCommand(parent)
  , m_display(display)
  , m_layer(layer)
  , m_updateItems(items)
  , m_nextFeature(0)
  , m_previousFeature(0)
  , m_redoCommandType(1)
  , m_undoCommandType(2)
{

  setText(QObject::tr("Update %1").arg(createCommandString(m_updateItems[m_updateItems.size() - 1]->getId()->getValueAsString().c_str())));

}

te::edit::UpdateCommand::~UpdateCommand()
{
}

void  te::edit::UpdateCommand::undo()
{

  if (m_updateItems.empty())
    return;

  m_previousFeature = m_updateItems.size() - 2;

  if (RepositoryManager::getInstance().hasIdentify(m_layer->getId(), m_updateItems[m_previousFeature]->getId()) == true)
  {

    Feature* f = new Feature(m_updateItems[m_previousFeature]->getId(), m_updateItems[m_previousFeature]->getGeometry(), m_updateItems[m_previousFeature]->getOperationType());

    RepositoryManager::getInstance().addFeature(m_layer->getId(), f->clone());

    draw(m_undoCommandType);

    emit geometryAcquired(f->clone()->getGeometry());

  }

}

void te::edit::UpdateCommand::redo()
{
  bool resultFound = false;

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

  m_nextFeature = m_updateItems.size() - 1;

  //no makes redo while the command is not on the stack
  if (resultFound)
  {

    Feature* f = new Feature(m_updateItems[m_nextFeature]->getId(), m_updateItems[m_nextFeature]->getGeometry(), m_updateItems[m_nextFeature]->getOperationType());

    RepositoryManager::getInstance().addFeature(m_layer->getId(), f->clone());

    draw(m_redoCommandType);

    emit geometryAcquired(f->clone()->getGeometry());

  }

}

void te::edit::UpdateCommand::draw(const int commandType)
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

  if (commandType == m_undoCommandType)
    renderer.draw(m_updateItems[m_previousFeature]->getGeometry(), true);
  else
    renderer.draw(m_updateItems[m_nextFeature]->getGeometry(), true);

  renderer.end();

  m_display->repaint();

}

QString te::edit::UpdateCommand::createCommandString(QString oid)
{
  return QObject::tr("%1")
    .arg(oid);
}