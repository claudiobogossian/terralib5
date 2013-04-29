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
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Geometry.h"
#include "../canvas/Canvas.h"
#include "../canvas/MapDisplay.h"
#include "Selection.h"

// Qt
#include <QtCore/QString>
#include <QtGui/QKeyEvent>
#include <QtGui/QMessageBox>
#include <QtGui/QMouseEvent>

// STL
#include <cassert>
#include <memory>

te::qt::widgets::Selection::Selection(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent) 
  : RubberBand(display, parent),
    m_layer(layer),
    m_selectionStarted(false),
    m_keepPreviousSelection(false)
{
  assert(layer.get());

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
  m_selectionStarted = false;

  if(e->button() != Qt::LeftButton)
    return false;

  RubberBand::mouseReleaseEvent(e);

  if(m_rect.isNull())
    return false;

  // Converts rect boundary to world coordinates
  QPointF ll(m_rect.left(), m_rect.bottom());
  QPointF ur(m_rect.right(), m_rect.top());
  ll = m_display->transform(ll);
  ur = m_display->transform(ur);

  te::gm::Envelope envelope(ll.x(), ll.y(), ur.x(), ur.y());

  try
  {
    // Gets the dataset
    std::auto_ptr<te::da::DataSet> dataset(m_layer->getData(envelope, te::gm::INTERSECTS));
    assert(dataset.get());

    // Let's generate the oids
    std::auto_ptr<const te::map::LayerSchema> schema(m_layer->getSchema());
    te::da::ObjectIdSet* oids = te::da::GenerateOIDSet(dataset.get(), schema.get());
    assert(oids);

    //if(!m_keepPreviousSelection)
      //m_layer->clearSelected(); // TODO: te::map::AbstractLayer::clearSelected() method

    // Adjusts the layer selection
    m_layer->select(oids);
  }
  catch(...)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The dataset cannot be retrieved from the layer") + " %1.").arg(m_layer->getTitle().c_str()));
    return false;
  }

  return true;
}
