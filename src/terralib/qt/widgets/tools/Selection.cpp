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
  \file terralib/qt/widgets/tools/Selection.cpp

  \brief This class implements a concrete tool to select layer features using an extent.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../geometry/Point.h"
#include "../../../geometry/Utils.h"
#include "../../../srs/Config.h"
#include "../canvas/MapDisplay.h"
#include "../utils/ScopedCursor.h"
#include "Selection.h"

// Qt
#include <QtCore/QString>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>

// STL
#include <cassert>
#include <exception>
#include <memory>

te::qt::widgets::Selection::Selection(te::qt::widgets::MapDisplay* display, const QCursor& cursor, const std::list<te::map::AbstractLayerPtr>& layers, QObject* parent) 
  : RubberBand(display, parent),
    m_layers(layers),
    m_selectionStarted(false),
    m_keepPreviousSelection(false),
    m_selectionByPointing(false)
{
  setCursor(cursor);

  // Setups the rubber band style
  m_pen.setStyle(Qt::DashLine);
  m_pen.setColor(QColor(255, 255, 100));
  m_pen.setWidth(2);
  m_brush = QColor(255, 255, 100, 80);
}

te::qt::widgets::Selection::~Selection()
{
}

bool te::qt::widgets::Selection::mousePressEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  m_selectionStarted = true;
  m_rect = QRectF();

  return RubberBand::mousePressEvent(e);
}

bool te::qt::widgets::Selection::mouseMoveEvent(QMouseEvent* e)
{
  if(!m_selectionStarted)
    return false;

  return RubberBand::mouseMoveEvent(e);
}

bool te::qt::widgets::Selection::mouseReleaseEvent(QMouseEvent* e)
{
  ScopedCursor cursor(Qt::WaitCursor);

  m_selectionStarted = false;

  if(e->button() != Qt::LeftButton)
    return false;

  RubberBand::mouseReleaseEvent(e);

  m_keepPreviousSelection = false;

  Qt::KeyboardModifiers keys = e->modifiers();

  if(keys & Qt::ControlModifier || keys & Qt::ShiftModifier)
    m_keepPreviousSelection = true;

  m_selectionByPointing = false;

  if(m_rect.isNull())
  {
    m_selectionByPointing = true;
    QPointF pixelOffset(4.0, 4.0);
#if QT_VERSION >= 0x050000
    m_rect = QRectF(e->localPos() - pixelOffset, e->localPos() + pixelOffset);
#else
    m_rect = QRectF(e->posF() - pixelOffset, e->posF() + pixelOffset);
#endif
  }

  // Converts rect boundary to world coordinates
  QPointF ll(m_rect.left(), m_rect.bottom());
  QPointF ur(m_rect.right(), m_rect.top());
  ll = m_display->transform(ll);
  ur = m_display->transform(ur);

  te::gm::Envelope envelope(ll.x(), ll.y(), ur.x(), ur.y());

  // Select the layers
  std::list<te::map::AbstractLayerPtr>::iterator it;
  for(it = m_layers.begin(); it != m_layers.end(); ++it)
    executeSelection(*it, envelope);

  return true;
}

void te::qt::widgets::Selection::setLayers(const std::list<te::map::AbstractLayerPtr>& layers)
{
  m_layers = layers;
}

void te::qt::widgets::Selection::executeSelection(const te::map::AbstractLayerPtr& layer, const te::gm::Envelope& e)
{
  if(layer->getVisibility() != te::map::VISIBLE || !layer->isValid())
    return;

  std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();

  if(!dsType->hasGeom())
    return;

  te::gm::Envelope reprojectedEnvelope(e);

  if((layer->getSRID() != TE_UNKNOWN_SRS) && (m_display->getSRID() != TE_UNKNOWN_SRS) && (layer->getSRID() != m_display->getSRID()))
    reprojectedEnvelope.transform(m_display->getSRID(), layer->getSRID());

  // Clear previous selection?
  if(!m_keepPreviousSelection)
  {
    layer->clearSelected();
    emit layerSelectedObjectsChanged(layer);
  }

  if(!reprojectedEnvelope.intersects(layer->getExtent()))
    return;

  try
  {
    // Gets the layer schema
    std::auto_ptr<const te::map::LayerSchema> schema(layer->getSchema());
    if(!schema->hasGeom())
      return;

    te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(schema.get());

    // Gets the dataset
    std::auto_ptr<te::da::DataSet> dataset = layer->getData(gp->getName(), &reprojectedEnvelope, te::gm::INTERSECTS);
    assert(dataset.get());

    // Let's generate the oids
    te::da::ObjectIdSet* oids = 0;
    te::da::GetEmptyOIDSet(schema.get(), oids);
    assert(oids);

    std::vector<std::string> pnames;
    te::da::GetOIDPropertyNames(schema.get(), pnames);

    // Generates a geometry from the given extent. It will be used to refine the results
    std::auto_ptr<te::gm::Geometry> geometryFromEnvelope(te::gm::GetGeomFromEnvelope(&reprojectedEnvelope, layer->getSRID()));

    if(m_selectionByPointing == false)
    {
      while(dataset->moveNext())
      {
        std::auto_ptr<te::gm::Geometry> g(dataset->getGeometry(gp->getName()));

        if(!g->intersects(geometryFromEnvelope.get()))
          continue;

        // Feature found!
        oids->add(te::da::GenerateOID(dataset.get(), pnames));
      }
    }
    else
    {
      // The restriction point. It will be used to refine the results
      te::gm::Coord2D center = reprojectedEnvelope.getCenter();
      te::gm::Point point(center.x, center.y, layer->getSRID());

      while(dataset->moveNext())
      {
        std::auto_ptr<te::gm::Geometry> g(dataset->getGeometry(gp->getName()));

        if(g->contains(&point) || g->crosses(geometryFromEnvelope.get()) || geometryFromEnvelope->contains(g.get()))
        {
          // Feature found!
          oids->add(te::da::GenerateOID(dataset.get(), pnames));
        }
      }
    }

    assert(oids);

    // Adjusts the layer selection

    const te::da::ObjectIdSet* currentOids = layer->getSelected();
    if(currentOids == 0 || !m_keepPreviousSelection)
    {
      layer->select(oids);
      emit layerSelectedObjectsChanged(layer);
      return;
    }

    te::da::ObjectIdSet* finalSelected = currentOids->clone();
    finalSelected->symDifference(oids);

    delete oids;

    layer->clearSelected();

    layer->select(finalSelected);

    emit layerSelectedObjectsChanged(layer);
  }
  catch(std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The selection cannot be retrieved from the layer. Details:") + " %1.").arg(e.what()));
    // TODO: catch the exceptions...
  }
}
