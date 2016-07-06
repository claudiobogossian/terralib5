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
#include "../../Repository.h"
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

te::edit::SplitPolygonTool::SplitPolygonTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, Qt::MouseButton sideToClose, QObject* parent)
  : CreateLineTool(display, layer, Qt::ArrowCursor, parent),
  m_oidSet(0),
  m_sideToClose(sideToClose),
  m_tol(0.000001)
{
}

te::edit::SplitPolygonTool::~SplitPolygonTool()
{
  delete m_oidSet;
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
  if (m_sideToClose != Qt::LeftButton || Qt::LeftButton != e->button())
    return false;

  startSplit();

  return true;
}

bool te::edit::SplitPolygonTool::mouseReleaseEvent(QMouseEvent* e)
{
  if (m_sideToClose != Qt::RightButton || Qt::RightButton != e->button())
    return false;

  startSplit();

  return true;
}

void te::edit::SplitPolygonTool::startSplit()
{
  if (m_coords.size() < 2) // Can not stop yet...
    return;

  m_isFinished = true;

  m_oidSet = new te::da::ObjectIdSet();

  std::auto_ptr<te::da::DataSet> ds(m_layer->getData(m_layer->getSelected()));
  std::size_t gpos = te::da::GetFirstSpatialPropertyPos(ds.get());

  std::vector<std::string> oidPropertyNames;
  te::da::GetOIDPropertyNames(m_layer->getSchema().get(), oidPropertyNames);

  while (ds->moveNext())
  {
    m_feature = new Feature(te::da::GenerateOID(ds.get(), oidPropertyNames), ds->getGeometry(gpos).release(), te::edit::GEOMETRY_UPDATE);
    splitPolygon();
  }

  if(m_oidSet->size() == 0)
    return;

  draw();

  te::edit::CreateLineTool::clear();

  emit splitFinished(*m_oidSet);
}

void te::edit::SplitPolygonTool::splitPolygon()
{
  if (m_feature == 0)
    return;

  std::auto_ptr<te::gm::Geometry> feature_bounds;
  std::auto_ptr<te::gm::Geometry> blade_in;
  std::auto_ptr<te::gm::Geometry> vgeoms;
  std::vector<te::gm::Polygon*> outputPolygons;

  int srid = m_feature->getGeometry()->getSRID();

  feature_bounds.reset(m_feature->getGeometry()->getBoundary());
  feature_bounds->setSRID(srid);

  blade_in.reset(te::edit::CreateLineTool::buildLine());
  blade_in->setSRID(srid);

  vgeoms.reset(feature_bounds->Union(blade_in.get()));
  vgeoms->setSRID(srid);

  te::gm::Polygonizer(vgeoms.get(), outputPolygons);

  RepositoryManager& repository = RepositoryManager::getInstance();

  std::size_t i = 1;
  while (i < outputPolygons.size())
  {
    if (!outputPolygons.at(i)->equals(m_feature->getGeometry()) &&
         outputPolygons.at(i)->coveredBy(m_feature->getGeometry()->buffer(m_tol)))
    {
      Feature* f = new Feature(GenerateId(), outputPolygons.at(i), te::edit::GEOMETRY_CREATE);

      if (outputPolygons.at(i)->getSRID() != m_display->getSRID())
        outputPolygons.at(i)->setSRID(m_display->getSRID());

      repository.addFeature(m_layer->getId(), f->clone());

      m_oidSet->add(f->getId());
    }
    i++;
  }

  if(m_oidSet->size() && outputPolygons.at(0)->coveredBy(m_feature->getGeometry()->buffer(m_tol)))
  {
    m_feature->setGeometry(outputPolygons.at(0));

    if (outputPolygons.at(0)->getSRID() != m_display->getSRID())
      outputPolygons.at(0)->setSRID(m_display->getSRID());

    repository.addFeature(m_layer->getId(), m_feature->clone());

    m_oidSet->add(m_feature->getId());
  }

  emit geometriesEdited();
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

  // Draw the layer edited geometries
  renderer.drawRepository(m_layer->getId(), env, m_display->getSRID());

  renderer.end();

  m_display->repaint();
}

void te::edit::SplitPolygonTool::resetVisualizationTool()
{
  te::edit::Repository* repo = te::edit::RepositoryManager::getInstance().getRepository(m_layer->getId());

  if (m_oidSet)
  {
    std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator it;
    for (it = m_oidSet->begin(); it != m_oidSet->end(); ++it)
    {
      if (repo->hasIdentifier(*it))
        repo->remove((*it));
    }
  }

  te::edit::CreateLineTool::clear();

  m_feature = 0;
  m_oidSet = 0;
}