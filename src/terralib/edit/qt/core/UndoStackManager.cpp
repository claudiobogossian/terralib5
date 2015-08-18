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
\file terralib/edit/qt/core/UndoStackManager.cpp

\brief This class manager the undo stack.
*/

// TerraLib
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"
#include "../../Feature.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "UndoStackManager.h"

//QT
#include <QUndoCommand>
#include <QUndoStack>


void te::edit::UndoStackManager::addUndoStack(QUndoCommand* command)
{
  //  Repository* repository = getRepository(source);
  m_undoStack->push(command);
}

QUndoStack* te::edit::UndoStackManager::getUndoStack()
{
  if (m_undoStack == 0)
  {
    m_undoStack = new QUndoStack();
  }

  return m_undoStack;
}

te::edit::UndoStackManager::UndoStackManager()
{}

te::edit::UndoStackManager::~UndoStackManager()
{
  delete m_undoStack;
}