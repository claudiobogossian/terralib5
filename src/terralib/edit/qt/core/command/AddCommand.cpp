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
#include "AddCommand.h"

te::edit::AddCommand::AddCommand(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, te::da::ObjectId* id,
  QUndoCommand *parent) :
  QUndoCommand(parent)
  , m_display(display)
  , m_layer(layer)
  , m_id(id)
  , m_stack(UndoStackManager::getInstance())
{
}

te::edit::AddCommand::~AddCommand()
{
}

void  te::edit::AddCommand::undo()
{
  if (m_stack.getAddWatches().empty())
    return;

  m_stack.m_currentIndex--;

  if (m_stack.m_currentIndex < 0)
    m_stack.m_currentIndex = -1;

  std::size_t pos = (m_stack.m_currentIndex < 0) ? 0 : m_stack.m_currentIndex;

  draw();

  emit undoFeedback(m_stack.getAddWatches()[pos]->getCoords());
}

void te::edit::AddCommand::redo()
{
  bool resultFound = false;

  if (!m_stack.getUndoStack())
    return;

  for (int i = 0; i < m_stack.getUndoStack()->count(); ++i)
  {
    const QUndoCommand* cmd = m_stack.getUndoStack()->command(i);
    if (cmd == this)
      resultFound = true;
  }

  //no makes redo while the command is not on the stack
  if (resultFound)
  {
    m_stack.m_currentIndex++;

    if (m_stack.m_currentIndex >= (int)m_stack.getAddWatches().size())
    {
      m_stack.m_currentIndex = (int)m_stack.getAddWatches().size() - 1;
      return;
    }

    draw();

    emit undoFeedback(m_stack.getAddWatches()[m_stack.m_currentIndex]->getCoords());
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

  RepositoryManager& repo = RepositoryManager::getInstance();

  std::size_t pos = (m_stack.m_currentIndex < 0) ? 0 : m_stack.m_currentIndex;

  if (m_stack.m_currentIndex > -1)
  {
    repo.addFeature(m_layer->getId(), m_stack.getAddWatches()[pos]->clone());

    if (pos < m_stack.getAddWatches().size() - 1)
    {
      if (m_stack.getAddWatches()[pos]->clone()->getId()->getValueAsString() != m_id->getValueAsString())
      {
        repo.removeFeature(m_layer->getId(), m_id);

        if (countFeaturesById(m_id, pos) > 1 && pos < m_stack.m_currentIndex)
          repo.addFeature(m_layer->getId(), m_stack.getAddWatches()[pos]->clone());
      }
    }

  }
  else if (repo.hasIdentify(m_layer->getId(), m_stack.getAddWatches()[pos]->getId()))
  {
    repo.removeFeature(m_layer->getId(), m_stack.getAddWatches()[pos]->getId());
  }

  // Draw the layer edited geometries
  renderer.drawRepository(m_layer->getId(), env, m_display->getSRID());

  if (m_stack.m_currentIndex < 0)
  {
    renderer.end();
    m_display->repaint();
    return;
  }

  // Draw the geometry
  renderer.draw(m_stack.getAddWatches()[m_stack.m_currentIndex]->clone()->getGeometry(), true);

  renderer.end();

  m_display->repaint();
}

std::size_t te::edit::AddCommand::countFeaturesById(te::da::ObjectId* id, std::size_t& lastPos)
{
  std::size_t count = 0;
  std::vector<std::size_t> vecPos;

  if (m_stack.getAddWatches().empty())
    return 0;

  for (std::size_t i = 0; i <m_stack.getAddWatches().size(); ++i)
  {
    if (m_stack.getAddWatches()[i]->isEquals(id))
    {
      count++;
      vecPos.push_back(i);
    }
  }

  if (vecPos.size() > 1)
    lastPos = vecPos.at(vecPos.size() - 2);

  return count;
}