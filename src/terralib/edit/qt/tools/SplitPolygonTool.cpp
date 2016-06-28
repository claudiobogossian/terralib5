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

te::edit::SplitPolygonTool::SplitPolygonTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent)
  : CreateLineTool(display, layer, Qt::ArrowCursor, parent),
  m_outputPolygons(0),
  m_oidSet(0)
{
}

te::edit::SplitPolygonTool::~SplitPolygonTool()
{
  delete m_oidSet;

  te::common::FreeContents(m_outputPolygons);
  m_outputPolygons.clear();
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

    m_isFinished = true;

    splitPolygon();

    draw();

    te::edit::CreateLineTool::clear();

    std::string msg("Split done!\n\nSelected polygons : " + QString::number(m_layer->getSelected()->size()).toStdString() + " \n");
                msg += "Polygons split successfully : " + QString::number(m_outputPolygons.size()).toStdString() + " \n";
    QMessageBox::information(m_display, tr("Polygon Split"), tr(msg.c_str()));

    return true;
  }
  catch (std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("Could not split.") + " %1.").arg(e.what()));
    return false;
  }
}

void te::edit::SplitPolygonTool::splitPolygon()
{
  std::auto_ptr<te::da::DataSet> ds(m_layer->getData(m_layer->getSelected()));

  std::size_t gpos = te::da::GetFirstSpatialPropertyPos(ds.get());

  if (!ds->moveNext())
    return;

  m_feature = te::edit::PickFeature(m_layer, *ds->getGeometry(gpos)->getMBR(), m_display->getSRID(), te::edit::GEOMETRY_UPDATE);

  if (m_feature == 0)
  {
    QMessageBox::critical(m_display, tr("TerraLib Edit Qt Plugin"), tr("Pick Feature Failed."));
    return;
  }

  std::auto_ptr<te::gm::Geometry> feature_bounds;
  feature_bounds.reset(m_feature->getGeometry()->getBoundary());

  std::auto_ptr<te::gm::Geometry> blade_in;
  blade_in.reset(te::edit::CreateLineTool::buildLine());

  if (blade_in->getSRID() != feature_bounds->getSRID())
    blade_in->setSRID(feature_bounds->getSRID());

  std::auto_ptr<te::gm::Geometry> vgeoms;
  vgeoms.reset(feature_bounds->Union(blade_in.get()));

  te::gm::Polygonizer(vgeoms.get(), m_outputPolygons);

  RepositoryManager& repository = RepositoryManager::getInstance();

  m_oidSet = new te::da::ObjectIdSet();

  std::size_t i = 0;

  while (i < m_outputPolygons.size())
  {
    if (m_outputPolygons.at(i)->getSRID() != m_feature->getGeometry()->getSRID())
      m_outputPolygons.at(i)->setSRID(m_feature->getGeometry()->getSRID());

    if (m_outputPolygons.at(i)->coveredBy(m_feature->getGeometry()->buffer(0.001)))
    {
      m_outputPolygons.at(i)->setSRID(m_layer->getSRID());

      if (i > 0)
      {
        Feature* f = new Feature();
        f->setGeometry(m_outputPolygons.at(i));
        f->setOperation(te::edit::GEOMETRY_CREATE);

        m_oidSet->add(f->getId());

        repository.addFeature(m_layer->getId(), f->clone());
      }
    }
    else
    { 
      m_outputPolygons.erase(m_outputPolygons.begin() + i);
      i--;
    }

    i++;
  }

  if (m_outputPolygons.size())
  {
    if (m_outputPolygons.at(0)->coveredBy(m_feature->getGeometry()->buffer(0.001)))
    {
      m_feature->setGeometry(m_outputPolygons.at(0));
      repository.addFeature(m_layer->getId(), m_feature->clone());

      m_oidSet->add(m_feature->getId());
    }
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
  // Clear the repository
  te::edit::Repository* repo = te::edit::RepositoryManager::getInstance().getRepository(m_layer->getId());

  std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator it;
  for (it = m_oidSet->begin(); it != m_oidSet->end(); ++it)
    repo->remove((*it));

  m_feature = 0;
  m_oidSet = 0;

  te::common::FreeContents(m_outputPolygons);
  m_outputPolygons.clear();
}