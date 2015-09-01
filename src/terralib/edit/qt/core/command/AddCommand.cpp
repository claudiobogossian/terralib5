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
  setText(QObject::tr("Add Geometry to %1").arg(QString(m_addItems[0]->getId()->getValueAsString().c_str())));
}

te::edit::AddCommand::~AddCommand()
{}

void  te::edit::AddCommand::undo()
{
  std::size_t count = 0;

  if (m_addItems.empty())
    return;

  std::string lastOid = m_addItems[m_addItems.size() - 1]->getId()->getValueAsString();

  for (std::size_t i = 0; i < m_addItems.size(); i++)
  {
    if (m_addItems[i]->getId()->getValueAsString() == lastOid)
      count++;
  }

  if (count == 1)
    RepositoryManager::getInstance().removeFeature(m_layer->getId(), m_addItems[m_addItems.size() - 1]->getId());

  m_previousFeature = m_addItems.size() - 2;

  if (m_previousFeature < 0) m_previousFeature = 0;

  if (RepositoryManager::getInstance().hasIdentify(m_layer->getId(), m_addItems[m_previousFeature]->getId()) == true)
  {

    Feature* f = new Feature(m_addItems[m_previousFeature]->getId(), m_addItems[m_previousFeature]->getGeometry(), m_addItems[m_previousFeature]->getOperationType());

    RepositoryManager::getInstance().addFeature(m_layer->getId(), f->clone());

  }

  draw();

}

void te::edit::AddCommand::redo()
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

  m_nextFeature = m_addItems.size() - 1;

  //no makes redo while the command is not on the stack
  if (resultFound)
  {

    Feature* f = new Feature(m_addItems[m_nextFeature]->getId(), m_addItems[m_nextFeature]->getGeometry(), m_addItems[m_nextFeature]->getOperationType());

    RepositoryManager::getInstance().addFeature(m_layer->getId(), f->clone());

    draw();

  }

  
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