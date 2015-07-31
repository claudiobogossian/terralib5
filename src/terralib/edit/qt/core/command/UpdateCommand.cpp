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
#include "../../Utils.h"
#include "../../Renderer.h"
#include "UpdateCommand.h"

te::edit::UpdateCommand::UpdateCommand(te::edit::EditionManager* editionManager, std::vector<Feature*> items, te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer,
  QUndoCommand *parent) :
  QUndoCommand(parent)
<<<<<<< HEAD
, m_display(display)
, m_layer(layer)
=======
, m_item(item)
>>>>>>> e23035ac569bba01a015a5343ac6e9f2dd4061f7
, m_updateItems(items)
, m_display(display)
, m_layer(layer)
, m_editionManager(editionManager)
{

  m_pos = 1;

  setText(QObject::tr("Update %1").arg(createCommandString(m_updateItems[m_updateItems.size() - 1]->getId()->getValueAsString().c_str())));

}

te::edit::UpdateCommand::~UpdateCommand()
{}

void  te::edit::UpdateCommand::undo()
{

  if (m_updateItems.empty())
    return;

  if (m_editionManager->m_repository->hasIdentify(m_layer->getId(), m_updateItems[m_updateItems.size() - m_pos]->getId()->clone()) == true)
  {
    m_editionManager->m_repository->removeFeature(m_layer->getId(), m_updateItems[m_updateItems.size() - m_pos]->getId()->clone());
  }

  m_pos++;

  draw();

}

void te::edit::UpdateCommand::redo()
{
  bool resultFound = false;

  if(!m_editionManager->getUndoStack())
    return;
  
  for (int i = 0; i < m_editionManager->getUndoStack()->count(); ++i)
  {
    const QUndoCommand* cmd = m_editionManager->getUndoStack()->command(i);
    if (cmd == this)
    {
      resultFound = true;
    }
  }

  //no makes redo while the command is not on the stack
  if (resultFound)
  {
    m_pos--;

    m_editionManager->m_repository->addFeature(m_layer->getId(), m_updateItems[m_updateItems.size() - m_pos]->clone());

    draw();

  }
  
}

QString te::edit::UpdateCommand::createCommandString(QString oid)
{
  return QObject::tr("%1")
    .arg(oid);
}

void te::edit::UpdateCommand::draw()
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
  renderer.drawRepository(m_editionManager,m_layer->getId(), env, m_display->getSRID());

  if (m_updateItems.size() >= m_pos)
    renderer.draw(m_updateItems[m_updateItems.size() - m_pos]->getGeometry(), false);

  renderer.end();

  m_display->repaint();

}