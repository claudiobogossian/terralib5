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

te::edit::UpdateCommand::UpdateCommand(std::map<te::da::ObjectId*, std::vector<te::gm::Geometry*> > items, te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer,
  QUndoCommand *parent) :
  QUndoCommand(parent)
  , m_display(display)
  , m_layer(layer)
  , m_updateItems(items)
{

  m_pos = 1;

  //setText(QObject::tr("Update %1").arg(createCommandString(m_updateItems[m_updateItems.size() - 1]->getId()->getValueAsString().c_str())));

}

te::edit::UpdateCommand::~UpdateCommand()
{}

void  te::edit::UpdateCommand::undo()
{

  if (m_updateItems.empty())
    return;

  std::map<te::da::ObjectId*, std::vector<te::gm::Geometry*>>::iterator it;

  for (it = m_updateItems.begin(); it != m_updateItems.end(); ++it)
  {
    if(RepositoryManager::getInstance().hasIdentify(m_layer->getId(), it->first->clone()) == true)
    {
      //RepositoryManager::getInstance().removeFeature(m_layer->getId(), m_updateItems[m_updateItems.size() - m_pos]->getId()->clone());
      
      //Feature* f = new Feature(it->first->clone(), geom, operation);

      //RepositoryManager::getInstance().addFeature(m_layer->getId(),)
    }

    draw();

    break;
  }

 // if (RepositoryManager::getInstance().hasIdentify(m_layer->getId(), m_updateItems[m_updateItems.size() - m_pos]->getId()->clone()) == true)
  //{
    //RepositoryManager::getInstance().removeFeature(m_layer->getId(), m_updateItems[m_updateItems.size() - m_pos]->getId()->clone());
 // }



  m_pos++;

  draw();

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

  //no makes redo while the command is not on the stack
  if (resultFound)
  {
    m_pos--;

    //RepositoryManager::getInstance().addFeature(m_layer->getId(), m_updateItems[m_updateItems.size() - m_pos]->clone());

    draw();

  }

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
  renderer.drawRepository(m_layer->getId(), env, m_display->getSRID());

  if (m_updateItems.size() >= m_pos)
  {
    //renderer.draw(m_updateItems[m_updateItems.size() - m_pos]->getGeometry(), false);

    //RepositoryManager::getInstance().addFeature(m_layer->getId(), m_updateItems[m_updateItems.size() - m_pos]->clone());
  }

  renderer.end();

  m_display->repaint();

}

QString te::edit::UpdateCommand::createCommandString(QString oid)
{
  return QObject::tr("%1")
    .arg(oid);
}