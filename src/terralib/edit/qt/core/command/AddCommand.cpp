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
#include "../UndoStackManager.h"
#include "AddCommand.h"

// STL
#include <set>

te::edit::AddCommand::AddCommand(std::vector<Feature*>& items, int& currentIndex,
  const te::map::AbstractLayerPtr& layer, QUndoCommand *parent) :
  QUndoCommand(parent)
, m_layer(layer)
, m_addItems(items)
, m_currentIndex(currentIndex)
{
}

te::edit::AddCommand::~AddCommand()
{
}

void  te::edit::AddCommand::undo()
{
  int pos = 0;

  if (m_addItems.empty())
    return;

  m_currentIndex--;

  if (m_currentIndex < 0)
    m_currentIndex = -1;

  pos = (m_currentIndex < 0) ? 0 : m_currentIndex;

  emit geometryAcquired(dynamic_cast<te::gm::Geometry*>(m_addItems[pos]->getGeometry()->clone()), m_addItems.at(pos)->clone()->getCoords());
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
      resultFound = true;
  }

  //no makes redo while the command is not on the stack
  if (resultFound)
  {
    m_currentIndex++;

    if (m_currentIndex >= (int)m_addItems.size())
    {
      m_currentIndex = (int)m_addItems.size() - 1;
      return;
    }

    emit geometryAcquired(dynamic_cast<te::gm::Geometry*>(m_addItems[m_currentIndex]->getGeometry()->clone()), m_addItems.at(m_currentIndex)->clone()->getCoords());

  }

}