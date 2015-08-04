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
\file terralib/edit/qt/core/EditionManager.cpp

\brief This class manager the edition.
*/

// TerraLib
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"
#include "../../Feature.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "EditionManager.h"

//QT
#include <QUndoCommand>
#include <QUndoStack>

te::edit::EditionManager::EditionManager()
{

  m_repository = new te::edit::RepositoryManager();
  m_undoStack = new QUndoStack();

}

te::edit::EditionManager::~EditionManager()
{
  delete m_undoStack;
  delete m_repository;
}

void te::edit::EditionManager::addUndoStack(QUndoCommand* command)
{
  m_undoStack->push(command);
}

QUndoStack* te::edit::EditionManager::getUndoStack()
{
  return m_undoStack;
}
