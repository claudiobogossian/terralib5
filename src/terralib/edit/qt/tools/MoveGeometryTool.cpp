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
  \file terralib/edit/qt/tools/MoveGeometryTool.cpp

  \brief This class implements a concrete tool to move geometries.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"
#include "../../Feature.h"
#include "../../RepositoryManager.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "MoveGeometryTool.h"

// Qt
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QDebug>

// STL 
#include <cassert>
#include <memory>

te::edit::MoveGeometryTool::MoveGeometryTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, bool rightButtonToSave, QObject* parent)
  : GeometriesUpdateTool(display, layer.get(), parent),
    m_rightButtonToSave(rightButtonToSave),
    m_selected(false),
    m_moveStarted(false),
    m_vecFeature(0),
    m_stack(UndoStackManager::getInstance())
{
  updateCursor();

  draw();
}

te::edit::MoveGeometryTool::~MoveGeometryTool()
{
  te::common::FreeContents(m_vecFeature);
  m_vecFeature.clear();
}

bool te::edit::MoveGeometryTool::mousePressEvent(QMouseEvent* e)
{
  if (e->button() != Qt::LeftButton)
    return false;

  if (m_layer->getSelected())
  {
    if (m_layer->getSelected()->size())
    {
      std::auto_ptr<te::da::DataSet> ds(m_layer->getData(m_layer->getSelected()));
      std::size_t gpos = te::da::GetFirstSpatialPropertyPos(ds.get());

      if (!m_vecFeature.size())
      { 
        ds->moveBeforeFirst();
        while (ds->moveNext())
        {
          m_feature = PickFeature(m_layer, *ds->getGeometry(gpos)->getMBR(), m_display->getSRID(), te::edit::GEOMETRY_UPDATE);
          if (m_feature)
          {
            m_vecFeature.push_back(m_feature->clone());
            m_selected = true;
          }
        }
      }
    }
  }

  if (!m_selected)
    pickFeature(m_layer, GetPosition(e));

  if (m_vecFeature.size())
    m_moveStarted = true;

  m_origin = m_display->transform(GetPosition(e));

  m_delta *= 0;

  return true;
}

bool te::edit::MoveGeometryTool::mouseMoveEvent(QMouseEvent* e)
{
  if(!m_moveStarted)
    return false;

  QPointF currentPosition = m_display->transform(GetPosition(e));

  // Calculates the delta value
  m_delta = currentPosition - m_origin;

  // Move geometry using the current delta
  if (m_vecFeature.size())
  {
    for (std::size_t i = 0; i < m_vecFeature.size(); i++)
      MoveGeometry(m_vecFeature[i]->getGeometry(), m_delta.x(), m_delta.y());
  }

  draw();

  m_origin = currentPosition;

  return false;
}

bool te::edit::MoveGeometryTool::mouseReleaseEvent(QMouseEvent* e)
{
  if (e->button() == Qt::RightButton && m_rightButtonToSave)
    emit geometriesEdited();

  if (e->button() != Qt::LeftButton)
    return false;

  m_moveStarted = false;

  if (m_delta == QPointF(0., 0.))
    return false;

  storeFeature();

  storeUndoCommand();

  if (!m_rightButtonToSave)
    emit geometriesEdited();
  else
    draw();

  return false;
}

void te::edit::MoveGeometryTool::reset()
{
  delete m_feature;

  te::common::FreeContents(m_vecFeature);
  m_vecFeature.clear();

  m_moveStarted = false;

  m_origin *= 0;
  m_delta *= 0;
}

void te::edit::MoveGeometryTool::pickFeature(const te::map::AbstractLayerPtr& layer, const QPointF& pos)
{
  reset();

  te::gm::Envelope env = buildEnvelope(pos);

  try
  {
    m_feature = PickFeature(m_layer, env, m_display->getSRID(), te::edit::GEOMETRY_UPDATE);
    if (m_feature)
      m_vecFeature.push_back(m_feature->clone());

    draw();
  }
  catch(std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The geometry cannot be selected from the layer. Details:") + " %1.").arg(e.what()));
  }
}

te::gm::Envelope te::edit::MoveGeometryTool::buildEnvelope(const QPointF& pos)
{
  QPointF pixelOffset(4.0, 4.0);

  QRectF rect(pos - pixelOffset, pos + pixelOffset);

  // Converts rect boundary to world coordinates
  QPointF ll(rect.left(), rect.bottom());
  QPointF ur(rect.right(), rect.top());
  ll = m_display->transform(ll);
  ur = m_display->transform(ur);

  te::gm::Envelope env(ll.x(), ll.y(), ur.x(), ur.y());

  return env;
}

void te::edit::MoveGeometryTool::draw()
{
  const te::gm::Envelope& env = m_display->getExtent();
  if(!env.isValid())
    return;

  // Clear!
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

   // Initialize the renderer
  Renderer& renderer = Renderer::getInstance();
  renderer.begin(draft, env, m_display->getSRID());

  // Draw the layer edited geometries
  renderer.drawRepository(m_layer->getId(), env, m_display->getSRID());

  if (!m_vecFeature.size() || m_stack.m_currentIndex < 0)
  {
    renderer.end();
    m_display->repaint();
    return;
  }

  if (m_delta == QPointF(0., 0.))
  {
    renderer.end();
    m_display->repaint();
    return;
  }

  for (std::size_t i = 0; i < m_vecFeature.size(); i++)
  { 
    // Draw the vertexes
    if (RepositoryManager::getInstance().hasIdentify(m_layer->getId(), m_vecFeature[i]->getId()) == false)
      renderer.draw(m_vecFeature[i]->getGeometry(), true);
    else
      renderer.drawVertexes(m_vecFeature[i]->getGeometry());
  }

  renderer.end();

  m_display->repaint();
}

void te::edit::MoveGeometryTool::updateCursor()
{
  m_display->setCursor(Qt::OpenHandCursor);
}

void te::edit::MoveGeometryTool::onExtentChanged()
{
  draw();
}

void te::edit::MoveGeometryTool::storeFeature()
{
  if (!m_vecFeature.size())
    return;

  for (std::size_t i = 0; i < m_vecFeature.size(); i++)
    RepositoryManager::getInstance().addFeature(m_layer->getId(), m_vecFeature[i]->clone());

}

void te::edit::MoveGeometryTool::storeUndoCommand()
{
  if (!m_vecFeature.size())
    return;

  for (std::size_t i = 0; i < m_vecFeature.size(); i++)
  {
    m_stack.addWatch(m_vecFeature[i]->clone());

    QUndoCommand* command = new AddCommand(m_display, m_layer, m_vecFeature[i]->clone()->getId());
    m_stack.addUndoStack(command);
  }
}

void te::edit::MoveGeometryTool::resetVisualizationTool()
{
  m_feature = 0;

  te::common::FreeContents(m_vecFeature);
  m_vecFeature.clear();
}