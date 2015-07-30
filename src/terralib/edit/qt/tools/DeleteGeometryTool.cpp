/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/edit/qt/tools/DeleteGeometryTool.cpp

  \brief This class implements a concrete tool to move geometries.
*/

// TerraLib
#include "../../../geometry/GeometryProperty.h"
#include "../../../geometry/MultiPolygon.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../qt/widgets/Utils.h"
#include "../../Feature.h"
#include "../../RepositoryManager.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "DeleteGeometryTool.h"

// Qt
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

#include "../../../dataaccess/dataset/ObjectIdSet.h"

// STL
#include <cassert>
#include <memory>

te::edit::DeleteGeometryTool::DeleteGeometryTool(te::edit::EditionManager* editionManager, te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent)
  : AbstractTool(display, parent),
    m_layer(layer),
    m_feature(0),
    m_editionManager(editionManager)
{
  // Signals & slots
  connect(m_display, SIGNAL(extentChanged()), SLOT(onExtentChanged()));
  
  pickFeature(m_layer, QPointF());

  if (m_feature == 0)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The geometry cannot be selected from the layer.")));
    return;
  }

  storeRemovedFeature();

}

te::edit::DeleteGeometryTool::~DeleteGeometryTool()
{
  delete m_feature;
}

bool te::edit::DeleteGeometryTool::mousePressEvent(QMouseEvent* e)
{
  return false;
}

bool te::edit::DeleteGeometryTool::mouseMoveEvent(QMouseEvent* e)
{
  return false;
}

bool te::edit::DeleteGeometryTool::mouseReleaseEvent(QMouseEvent* e)
{
  return false;
}

bool te::edit::DeleteGeometryTool::mouseDoubleClickEvent(QMouseEvent* e)
{
  return false;
}

void te::edit::DeleteGeometryTool::reset()
{
  delete m_feature;
  m_feature = 0;
}

void te::edit::DeleteGeometryTool::pickFeature(const te::map::AbstractLayerPtr& layer, const QPointF& pos)
{
  reset();

  try
  {
    std::auto_ptr<te::da::DataSetType> dt(layer->getSchema());

    const te::da::ObjectIdSet* objSet = layer->getSelected();

    std::auto_ptr<te::da::DataSet> ds(layer->getData(objSet));

    te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dt.get());

    if (ds->moveNext())
    {

      std::auto_ptr<te::gm::Geometry> geom = ds->getGeometry(geomProp->getName());
      te::gm::Envelope auxEnv(*geom->getMBR());

      m_feature = PickFeature(m_editionManager, m_layer, auxEnv, m_display->getSRID());

    }

    draw();
  }
  catch (std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The geometry cannot be selected from the layer. Details:") + " %1.").arg(e.what()));
  }
}

te::gm::Envelope te::edit::DeleteGeometryTool::buildEnvelope(const QPointF& pos)
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

void te::edit::DeleteGeometryTool::draw()
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
  renderer.drawRepository(m_editionManager, m_layer->getId(), env, m_display->getSRID());

  if(m_feature == 0)
  {
    renderer.end();
    m_display->repaint();
    return;
  }

  renderer.setPolygonStyle(Qt::white, Qt::black, 1);

  renderer.draw(m_feature->getGeometry(), false);

  renderer.end();

  m_display->repaint();

}

void te::edit::DeleteGeometryTool::onExtentChanged()
{
  draw();
}

void te::edit::DeleteGeometryTool::storeRemovedFeature()
{
  m_editionManager->m_repository->addGeometry(m_layer->getId(), m_feature->getId()->clone(), dynamic_cast<te::gm::Geometry*>(m_feature->getGeometry()->clone()));

  m_editionManager->m_operation[m_feature->getId()->getValueAsString()] = m_editionManager->removeOp;

}