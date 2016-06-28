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
  \file terralib/edit/qt/tools/SplitPolygonTool.cpp

  \brief This class implements a concrete tool to split polygons.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../geometry/Envelope.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"
#include "../../Feature.h"
#include "../../RepositoryManager.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "SplitPolygonTool.h"

// Qt
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

// STL
#include <cassert>
#include <memory>

te::edit::SplitPolygonTool::SplitPolygonTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent)
  : CreateLineTool(display, layer, Qt::ArrowCursor, parent),
  m_inputPolygons(0)
{
}

te::edit::SplitPolygonTool::~SplitPolygonTool()
{
  te::common::FreeContents(m_inputPolygons);
  m_inputPolygons.clear();
}

void te::edit::SplitPolygonTool::splitPolygon()
{
  std::auto_ptr<te::da::DataSet> ds(m_layer->getData(m_layer->getSelected()));

  std::size_t gpos = te::da::GetFirstSpatialPropertyPos(ds.get());

  if (!ds->moveNext())
    return;

  te::gm::Geometry* g1_bounds = 0;
  
  m_feature = new Feature();
  m_feature->setOperation(te::edit::GEOMETRY_UPDATE);
  m_feature->setGeometry(convertGeomType(m_layer,ds->getGeometry(gpos).release()));

  g1_bounds = m_feature->getGeometry()->getBoundary();

  te::gm::Geometry* blade_in = te::edit::CreateLineTool::buildLine();

  if (blade_in->getSRID() != g1_bounds->getSRID())
    blade_in->setSRID(g1_bounds->getSRID());

  std::auto_ptr<te::gm::Geometry> vgeoms;
  vgeoms.reset(g1_bounds->Union(blade_in));

  te::gm::Polygonizer(vgeoms.get(), m_inputPolygons);

  draw();
}

bool te::edit::SplitPolygonTool::mousePressEvent(QMouseEvent* e)
{
  if (e->button() != Qt::LeftButton)
    return false;

  if (m_isFinished) // Is Finished?! So, start again...
  {
    resetVisualizationTool();
    m_isFinished = false;
  }

  return te::edit::CreateLineTool::mousePressEvent(e);
}

bool te::edit::SplitPolygonTool::mouseMoveEvent(QMouseEvent* e)
{
  return te::edit::CreateLineTool::mouseMoveEvent(e);
}

bool te::edit::SplitPolygonTool::mouseDoubleClickEvent(QMouseEvent* e)
{
  try
  {
    if (e->button() != Qt::LeftButton)
      return false;

    if (m_coords.size() < 2) // Can not stop yet...
      return false;

    splitPolygon();

    m_isFinished = true;

    return true;
  }
  catch (std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("Could not split.") + " %1.").arg(e.what()));
    return false;
  }
}

void te::edit::SplitPolygonTool::draw()
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

  // Draw the current geometry and the vertexes
  for (std::size_t i = 0; i < m_inputPolygons.size(); i++)
  {
      if (m_inputPolygons.at(i)->getSRID() != m_feature->getGeometry()->getSRID())
        m_inputPolygons.at(i)->setSRID(m_feature->getGeometry()->getSRID());

      if (m_inputPolygons.at(i)->coveredBy(m_feature->getGeometry()->buffer(0.001)))
      {
        m_inputPolygons.at(i)->setSRID(m_layer->getSRID());

        Feature* f = new Feature();
        f->setGeometry(m_inputPolygons.at(i));
        f->setOperation(te::edit::GEOMETRY_CREATE);

        RepositoryManager::getInstance().addFeature(m_layer->getId(), f->clone());
      }
  }

  // Draw the layer edited geometries
  renderer.drawRepository(m_layer->getId(), env, m_display->getSRID());

  renderer.end();

  m_display->repaint();
}

void te::edit::SplitPolygonTool::resetVisualizationTool()
{
  m_feature = 0;

  te::edit::CreateLineTool::clear();

  RepositoryManager::getInstance().removeAll();

  te::common::FreeContents(m_inputPolygons);
  m_inputPolygons.clear();
}