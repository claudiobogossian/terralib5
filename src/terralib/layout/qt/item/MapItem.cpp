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
  \file MapItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapItem.h"
#include "MapController.h"

#include "../core/ItemUtils.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../qt/core/Scene.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
//#include "../../../qt/widgets/layer/explorer/AbstractTreeItem.h" //rever esta dependencia
#include "../../../qt/widgets/tools/Pan.h"
#include "../../../qt/widgets/tools/ZoomWheel.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QApplication>
#include <QEvent>
#include <QWheelEvent>
#include <QMouseEvent>

te::layout::MapItem::MapItem(AbstractItemController* controller, bool invertedMatrix)
  : AbstractItem<QGraphicsObject>(controller, invertedMatrix)
  , m_mapDisplay(0)
  , m_pan(0)
  , m_zoomWheel(0)
{
  this->setAcceptDrops(true);

  m_mapDisplay = new te::qt::widgets::MapDisplay();
  m_mapDisplay->setAcceptDrops(true);
  m_mapDisplay->setBackgroundColor(Qt::transparent);
  m_mapDisplay->setResizeInterval(0);
  m_mapDisplay->setMouseTracking(true);

  this->prepareGeometryChange();
  m_mapDisplay->resize(10, 10);

  connect(m_mapDisplay, SIGNAL(extentChanged()), this, SLOT(extentChanged()));
}

te::layout::MapItem::~MapItem()
{
  delete m_mapDisplay;
}

te::qt::widgets::MapDisplay* te::layout::MapItem::getMapDisplay()
{
  return m_mapDisplay;
}

void te::layout::MapItem::contextUpdated(const ContextObject& context)
{
  int zoom = context.getZoom();
  ((MapController *) m_controller)->setZoom(zoom);

  Utils* utils = Context::getInstance().getUtils();

  QRectF boxMM = boundingRect();

  te::gm::Envelope box(0, 0, boxMM.width(), boxMM.height());
  box = utils->viewportBox(box);

  m_mapDisplay->setOverrideDPI(context.getDpiX(), context.getDpiY());

  QSize currentSize = m_mapDisplay->size();
  QSize newSize(qRound(box.getWidth()), qRound(box.getHeight()));
  if(currentSize != newSize)
  {
    this->prepareGeometryChange();
    m_mapDisplay->resize(newSize);
  }
}

void te::layout::MapItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  QPixmap pixmap(m_mapDisplay->width(), m_mapDisplay->height());
  pixmap.fill(Qt::transparent);

  QPainter localPainter(&pixmap);
  m_mapDisplay->render(&localPainter, QPoint(), QRegion(), QWidget::DrawChildren);
  localPainter.end();

  QImage image = pixmap.toImage();
  image = image.mirrored();
  pixmap = QPixmap::fromImage(image);

  painter->save();

  painter->setClipRect(boundingRect());
  painter->drawPixmap(boundingRect(), pixmap, pixmap.rect());

  painter->restore();
}

QVariant te::layout::MapItem::itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value )
{
  if(change == QGraphicsItem::ItemSceneHasChanged)
  {
    Scene* myScene = dynamic_cast<Scene*>(this->scene());
    if(myScene != 0)
    {
      contextUpdated(myScene->getContext());
    }
  }
  return AbstractItem<QGraphicsObject>::itemChange(change, value);
}

void te::layout::MapItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
  if(m_isEditionMode == false)
  {
    AbstractItem<QGraphicsObject>::mousePressEvent(event);
    return;
  }

  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  if(!iUtils)
    return;

  QRectF rect = boundingRect();
  QPointF point = event->pos();
  QPointF remappedPoint = remapPointToViewport(point, rect, m_mapDisplay->rect());

  QMouseEvent mouseEvent(QEvent::MouseButtonPress, remappedPoint.toPoint(), event->button(),event->buttons(), event->modifiers());
  QApplication::sendEvent(m_mapDisplay, &mouseEvent);
  event->setAccepted(mouseEvent.isAccepted());

  refresh();
}

void  te::layout::MapItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
  if(m_isEditionMode == false)
  {
    AbstractItem<QGraphicsObject>::mouseMoveEvent(event);
    return;
  }

  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  if(!iUtils)
    return;

  QRectF rect = boundingRect();
  QPointF point = event->pos();
  QPointF remappedPoint = remapPointToViewport(point, rect, m_mapDisplay->rect());

  QMouseEvent mouseEvent(QEvent::MouseMove, remappedPoint.toPoint(), event->button(),event->buttons(), event->modifiers());
  QApplication::sendEvent(m_mapDisplay, &mouseEvent);
  event->setAccepted(mouseEvent.isAccepted());

  refresh();
}

void  te::layout::MapItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    if(m_isEditionMode == false)
  {
    AbstractItem<QGraphicsObject>::mouseReleaseEvent(event);
    return;
  }

  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  if(!iUtils)
    return;

  QRectF rect = boundingRect();
  QPointF point = event->pos();
  QPointF remappedPoint = remapPointToViewport(point, rect, m_mapDisplay->rect());

  QMouseEvent mouseEvent(QEvent::MouseButtonRelease, remappedPoint.toPoint(), event->button(),event->buttons(), event->modifiers());
  QApplication::sendEvent(m_mapDisplay, &mouseEvent);
  event->setAccepted(mouseEvent.isAccepted());

  refresh();
}

void te::layout::MapItem::dragEnterEvent( QGraphicsSceneDragDropEvent * event )
{
  //Copy the map from layer tree
  Qt::DropActions actions = event->dropAction();
  if(actions != Qt::CopyAction)
    return;

  const QMimeData* mime = event->mimeData();
  QString s = mime->data("application/x-terralib;value=\"DraggedItems\"").constData();
  if(s.isEmpty())
    return;

  event->acceptProposedAction();
}

void te::layout::MapItem::dragLeaveEvent( QGraphicsSceneDragDropEvent * event )
{
  AbstractItem<QGraphicsObject>::dragLeaveEvent(event);
}

void te::layout::MapItem::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
{
  AbstractItem<QGraphicsObject>::dragMoveEvent(event);
}

void te::layout::MapItem::dropEvent( QGraphicsSceneDragDropEvent * event )
{
  /*
  event->setDropAction(Qt::CopyAction);

  QString s = event->mimeData()->data("application/x-terralib;value=\"DraggedItems\"").constData();
  unsigned long v = s.toULongLong();
  std::vector<te::qt::widgets::AbstractTreeItem*>* vecItems = reinterpret_cast<std::vector<te::qt::widgets::AbstractTreeItem*>*>(v);

  if(vecItems->empty())
    return;

  std::list<te::map::AbstractLayerPtr> listLayers;
  for(size_t i = 0 ; i < vecItems->size(); ++i)
  {
    te::qt::widgets::AbstractTreeItem* treeItem = vecItems->operator[](i);
    te::map::AbstractLayerPtr layer = treeItem->getLayer();
    listLayers.push_back(layer);
  }

  MapController* mapController = dynamic_cast<MapController*>(m_controller);
  if(mapController != 0)
  {
    mapController->addLayers(listLayers);
  }
  */
}

void te::layout::MapItem::wheelEvent ( QGraphicsSceneWheelEvent * event )
{
  if(m_isEditionMode == false)
  {
    AbstractItem<QGraphicsObject>::wheelEvent(event);
    return;
  }

  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  if(!iUtils)
    return;

  QRectF rect = boundingRect();
  QPointF point = event->pos();
  QPointF remappedPoint = remapPointToViewport(point, rect, m_mapDisplay->rect());

  QWheelEvent wheelEvent(remappedPoint.toPoint(), event->delta(),event->buttons(), event->modifiers());
  QApplication::sendEvent(m_mapDisplay, &wheelEvent);
  event->setAccepted(wheelEvent.isAccepted());

  QGraphicsItem::update();
}

void te::layout::MapItem::enterEditionMode()
{
  //we now install the visualization tools in the map display and forward all the mouse and keyboards events to it
  if(m_pan == 0)
  {
    m_pan = new te::qt::widgets::Pan(m_mapDisplay, Qt::OpenHandCursor, Qt::ClosedHandCursor);
  }
  if(m_zoomWheel == 0)
  {
    m_zoomWheel = new te::qt::widgets::ZoomWheel(m_mapDisplay, 1.25, false);
  }

  m_mapDisplay->installEventFilter(m_pan);
  m_mapDisplay->installEventFilter(m_zoomWheel);

  this->setCursor(Qt::OpenHandCursor);
}

void te::layout::MapItem::leaveEditionMode()
{
  //we now unistall the visualization tools from the map display and no more events will be forward to it
  if(m_pan != 0)
  {
    m_mapDisplay->removeEventFilter(m_pan);
    delete m_pan;
    m_pan = 0;
  }

  if(m_zoomWheel != 0)
  {
    m_mapDisplay->removeEventFilter(m_zoomWheel);
    delete m_zoomWheel;
    m_zoomWheel = 0;
  }

  this->setCursor(Qt::ArrowCursor);
}

QPointF te::layout::MapItem::remapPointToViewport(const QPointF& point, const QRectF& item, const QRectF& widget) const
{
  double resX = widget.width() / item.width();
  double resY = widget.height() / item.height();

  QMatrix matrix;
  matrix.scale(resX, -resY);
  matrix.translate(-item.bottomLeft().x(), -item.bottomLeft().y());

  QPointF remappedPoint = matrix.map(point);
  return remappedPoint;
}

void te::layout::MapItem::extentChanged()
{
  MapController* mapController = dynamic_cast<MapController*>(m_controller);
  if(mapController != 0)
  {
    mapController->extentChanged(m_mapDisplay->getExtent(), m_mapDisplay->getScale(), m_mapDisplay->getSRID());
  }
}

void te::layout::MapItem::resized()
{
  Scene* myScene = dynamic_cast<Scene*>(this->scene());
  if(myScene != 0)
  {
    contextUpdated(myScene->getContext());
  }
}

void te::layout::MapItem::doRefresh()
{
  m_mapDisplay->refresh();
}
