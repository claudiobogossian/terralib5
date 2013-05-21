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
#include "../../../geometry/Geometry.h"
#include "../../../geometry/Utils.h"
#include "../../../maptools/MarkRendererManager.h"
#include "../../../se/Fill.h"
#include "../../../se/Stroke.h"
#include "../../../se/Mark.h"
#include "../../../se/Utils.h"
#include "../../../srs/Config.h"
#include "../canvas/Canvas.h"
#include "../canvas/MapDisplay.h"
#include "../utils/ScopedCursor.h"
#include "Selection.h"

// Qt
#include <QtCore/QString>
#include <QtGui/QKeyEvent>
#include <QtGui/QMessageBox>
#include <QtGui/QMouseEvent>

// STL
#include <cassert>
#include <exception>
#include <memory>

te::qt::widgets::Selection::Selection(te::qt::widgets::MapDisplay* display, const std::list<te::map::AbstractLayerPtr>& layers, const QColor& selectionColor, QObject* parent) 
  : RubberBand(display, parent),
    m_layers(layers),
    m_selectionStarted(false),
    m_keepPreviousSelection(false),
    m_selectionByPointing(false),
    m_selectionColor(selectionColor)
{
  // Setups the rubber band style
  m_pen.setStyle(Qt::DashLine);
  m_pen.setColor(QColor(255, 255, 100));
  m_pen.setWidth(2);
  m_brush = QColor(255, 255, 100, 80);

  m_display->setFocusPolicy(Qt::ClickFocus);
}

te::qt::widgets::Selection::~Selection()
{
}

bool te::qt::widgets::Selection::eventFilter(QObject* watched, QEvent* e)
{
  switch(e->type())
  {
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    {
      QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);
      if(keyEvent->key() == Qt::Key_Control)
        m_keepPreviousSelection = !m_keepPreviousSelection;
    }
    break;

    default:
    break;
  }

  return RubberBand::eventFilter(watched, e);
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
  if(m_layers.empty())
    return false;

  ScopedCursor cursor(Qt::WaitCursor);

  m_selectionStarted = false;

  if(e->button() != Qt::LeftButton)
    return false;

  RubberBand::mouseReleaseEvent(e);

  m_selectionByPointing = false;

  if(m_rect.isNull())
  {
    m_selectionByPointing = true;
    QPointF pixelOffset(4.0, 4.0);
    m_rect = QRectF(e->posF() - pixelOffset, e->posF() + pixelOffset);
  }

  // Converts rect boundary to world coordinates
  QPointF ll(m_rect.left(), m_rect.bottom());
  QPointF ur(m_rect.right(), m_rect.top());
  ll = m_display->transform(ll);
  ur = m_display->transform(ur);

  te::gm::Envelope envelope(ll.x(), ll.y(), ur.x(), ur.y());

  // For while, select the last layer
  const te::map::AbstractLayerPtr& layer = *(m_layers.rbegin());
  executeSelection(layer, envelope);
  drawSelection(layer);

  return true;
}

void te::qt::widgets::Selection::executeSelection(const te::map::AbstractLayerPtr& layer, const te::gm::Envelope& e)
{
  if(layer->getVisibility() != te::map::VISIBLE)
    return;

  bool needRemap = false;
  te::gm::Envelope reprojectedEnvelope(e);

  if((layer->getSRID() != TE_UNKNOWN_SRS) && (m_display->getSRID() != TE_UNKNOWN_SRS) && (layer->getSRID() != m_display->getSRID()))
  {
    needRemap = true;
    reprojectedEnvelope.transform(m_display->getSRID(), layer->getSRID());
  }

  // Clear previous selection?
  if(!m_keepPreviousSelection)
    layer->clearSelected();

  if(!reprojectedEnvelope.intersects(layer->getExtent()))
    return;

  try
  {
    // Gets the dataset
    std::auto_ptr<te::da::DataSet> dataset(layer->getData(reprojectedEnvelope, te::gm::INTERSECTS));
    assert(dataset.get());

    // Let's generate the oids
    std::auto_ptr<const te::map::LayerSchema> schema(layer->getSchema());
    te::da::ObjectIdSet* oids = te::da::GenerateOIDSet(dataset.get(), schema.get());
    assert(oids);

    // Adjusts the layer selection
    layer->select(oids);
  }
  catch(std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The selection cannot be retrieved from the layer. Details:") + " %1.").arg(e.what()));
    // TODO: catch the exceptions...
  }
}

void te::qt::widgets::Selection::drawSelection(const te::map::AbstractLayerPtr& layer) const
{
  if(layer->getVisibility() != te::map::VISIBLE)
    return;

  const te::da::ObjectIdSet* oids = layer->getSelected();
  if(oids == 0 || oids->size() == 0)
    return;

  bool needRemap = false;

  if((layer->getSRID() != TE_UNKNOWN_SRS) && (m_display->getSRID() != TE_UNKNOWN_SRS) && (layer->getSRID() != m_display->getSRID()))
    needRemap = true;

  // Try retrieves the layer selection
  std::auto_ptr<te::da::DataSet> selected;
  try
  {
    selected.reset(layer->getData(oids));
  }
  catch(std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The layer selection cannot be drawn. Details:") + " %1.").arg(e.what()));
    return;
  }

  std::size_t gpos = te::da::GetFirstPropertyPos(selected.get(), te::dt::GEOMETRY_TYPE);

  QPixmap* draft = m_display->getDraftPixmap();

  const te::gm::Envelope& displayExtent = m_display->getExtent();

  Canvas canvas(draft);
  canvas.setWindow(displayExtent.m_llx, displayExtent.m_lly, displayExtent.m_urx, displayExtent.m_ury);

  te::gm::GeomType currentGeomType = te::gm::UnknownGeometryType;

  while(selected->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> g(selected->getGeometry(gpos));

    if(needRemap)
    {
      g->setSRID(layer->getSRID());
      g->transform(m_display->getSRID());
    }

    if(currentGeomType != g->getGeomTypeId())
    {
      currentGeomType = g->getGeomTypeId();

      switch(currentGeomType)
      {
        case te::gm::PolygonType:
        case te::gm::PolygonZType:
        case te::gm::PolygonMType:
        case te::gm::PolygonZMType:
        case te::gm::MultiPolygonType:
        case te::gm::MultiPolygonZType:
        case te::gm::MultiPolygonMType:
        case te::gm::MultiPolygonZMType:
        {
          canvas.setPolygonContourWidth(2);
          canvas.setPolygonContourColor(te::color::RGBAColor(0, 0, 0, TE_OPAQUE));
          canvas.setPolygonFillColor(te::color::RGBAColor(m_selectionColor.red(), m_selectionColor.green(), m_selectionColor.blue(), 128));
        }
        break;

        case te::gm::LineStringType:
        case te::gm::LineStringZType:
        case te::gm::LineStringMType:
        case te::gm::LineStringZMType:
        case te::gm::MultiLineStringType:
        case te::gm::MultiLineStringZType:
        case te::gm::MultiLineStringMType:
        case te::gm::MultiLineStringZMType:
        {
          canvas.setLineColor(te::color::RGBAColor(m_selectionColor.red(), m_selectionColor.green(), m_selectionColor.blue(), 128));
          canvas.setLineWidth(6);
        }
        break;

        case te::gm::PointType:
        case te::gm::PointZType:
        case te::gm::PointMType:
        case te::gm::PointZMType:
        case te::gm::MultiPointType:
        case te::gm::MultiPointZType:
        case te::gm::MultiPointMType:
        case te::gm::MultiPointZMType:
        {
          std::size_t size = 24;

          te::se::Stroke* stroke = te::se::CreateStroke(m_selectionColor.name().toStdString(), "2","0.6");
          te::se::Fill* fill = te::se::CreateFill(m_selectionColor.name().toStdString(), "0.3");
          te::se::Mark* mark = te::se::CreateMark("square", stroke, fill);

          te::color::RGBAColor** rgba= te::map::MarkRendererManager::getInstance().render(mark, size);

          canvas.setPointColor(te::color::RGBAColor(0, 0, 0, TE_TRANSPARENT));
          canvas.setPointPattern(rgba, size, size);

          te::common::Free(rgba, size);
          delete mark;
        }
        break;

        default:
          return;
      }
    }

    canvas.draw(g.get());
  }

  m_display->repaint();
}
