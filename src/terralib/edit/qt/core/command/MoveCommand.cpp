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
\file MoveCommand.cpp

\brief

*/

// TerraLib
#include "../../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../../dataaccess/dataset/ObjectId.h"
#include "../../../../geometry/Geometry.h"
#include "../../../Feature.h"
#include "../../../Utils.h"
#include "../../Utils.h"
#include "../../Renderer.h"
#include "MoveCommand.h"

te::edit::MoveCommand::MoveCommand(te::edit::EditionManager* editionManager, std::map<std::string, QList<QPointF> > items, Feature* item, te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer,
  QUndoCommand *parent) :
  QUndoCommand(parent)
, m_display(display)
, m_layer(layer)
, m_item(item)
, m_moveItems(items)
, m_editionManager(editionManager)
{

  std::map<std::string, QList<QPointF> >::iterator it;

  for (it = items.begin(); it != items.end(); it++)
  {

    for (int i = 0; i < it->second.size(); i++)
      m_itemsPoints.push_back(it->second[i]);

  }

}

te::edit::MoveCommand::~MoveCommand()
{}

void  te::edit::MoveCommand::undo()
{
  if (m_item)
  {
    m_oldPos = m_itemsPoints.at(m_itemsPoints.size() - 1);

    setText(QObject::tr("Move %1")
      .arg(createCommandString(m_oldPos)));
  }

  if (m_moveItems.empty())
    return;

  std::map<std::string, QList<QPointF> >::iterator it;

  for (it = m_moveItems.begin(); it != m_moveItems.end(); ++it)
  {
    QString id = it->first.c_str();

    if (it->first == m_item->getId()->getValueAsString()) 
    {

      MoveGeometry(m_item->getGeometry(), it->second[it->second.size() - 1].x(), it->second[it->second.size() - 1].y());

      m_oldPos = it->second[it->second.size() - 1];

      draw(m_item);

      break;
    }
  }

}

void te::edit::MoveCommand::redo()
{
  if (m_item)
  {
    m_newPos = m_itemsPoints.at(m_itemsPoints.size() - 1);

    setText(QObject::tr("Move %1")
      .arg(createCommandString(m_newPos)));
  }

  if (m_moveItems.empty())
    return;

  if(!m_editionManager->getUndoStack())
    return;
  
  bool resultFound = false;

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

    std::map<std::string, QList<QPointF> >::iterator it;

    for (it = m_moveItems.begin(); it != m_moveItems.end(); ++it)
    {
      QString id = it->first.c_str();

      if (it->first == m_item->getId()->getValueAsString())
      {

        QPointF ptNew = it->second[it->second.size() - 1];

        MoveGeometry(m_item->getGeometry(), -ptNew.x(), -ptNew.y());

        m_newPos = it->second[it->second.size() - 1];

        draw(m_item);

        break;
      }
    }

  }

}

QString te::edit::MoveCommand::createCommandString(const QPointF &pos)
{
  if (!m_item)
    return QObject::tr("%1");

  return QObject::tr("(%1, %2) %3: %4")
    .arg(pos.x()).arg(pos.y())
    .arg(m_item->getGeometry()->getGeometryType().c_str())
    .arg(m_item->getId()->getValueAsString().c_str());

}

void te::edit::MoveCommand::draw(Feature* feat)
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

  m_editionManager->m_repository->addFeature(m_layer->getId(), feat->clone());

  if (m_editionManager->m_repository->hasIdentify(m_layer->getId(), feat->getId()) == true)
  {
    m_editionManager->m_repository->removeFeature(m_layer->getId(), feat->getId());
  }

  // Draw the layer edited geometries
  renderer.drawRepository(m_editionManager, m_layer->getId(), env, m_display->getSRID());

  renderer.draw(feat->getGeometry(), true);

  renderer.end();

  m_display->repaint();

  m_editionManager->m_repository->addFeature(m_layer->getId(), feat->clone());

}