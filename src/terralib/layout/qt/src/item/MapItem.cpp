/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
#include "Context.h"
#include "AbstractScene.h"
#include "ItemModelObservable.h"
#include "ItemObserver.h"
#include "ItemController.h"
#include "../../../color/RGBAColor.h"
#include "../../../../qt/widgets/Utils.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../common/STLUtils.h"
#include "../../../../qt/widgets/canvas/Canvas.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../../qt/widgets/canvas/MultiThreadMapDisplay.h"
#include "../../../../common/TreeItem.h"
#include "../../../../srs/Converter.h"
#include "../../../../qt/widgets/tools/ZoomWheel.h"
#include "../../../../maptools/Utils.h"

// STL
#include <vector>
#include <memory>

// Qt
#include <QCursor>
#include <QPixmap>
#include <QMessageBox>
#include <QWidget>
#include <QDropEvent>
#include <QBoxLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPointer>
#include <QDebug>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include "MapGridController.h"

te::layout::MapItem::MapItem( ItemController* controller, Observable* o ) :
  QGraphicsProxyWidget(0),
  ItemObserver(controller, o),
  m_mapDisplay(0),
  m_grabbedByWidget(false),
  m_treeItem(0)
{
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges 
    | QGraphicsItem::ItemIgnoresTransformations);
  
  setAcceptDrops(true);
  
  Utils* utils = Context::getInstance()->getUtils();
  te::gm::Envelope box = utils->viewportBox(m_model->getBox());

  m_mapDisplay = new te::qt::widgets::MultiThreadMapDisplay(QSize(box.getWidth(), box.getHeight()), true);
  m_mapDisplay->setAcceptDrops(true);
  m_mapDisplay->setBackgroundColor(Qt::gray);
  m_mapDisplay->setResizeInterval(0);
  m_mapDisplay->setMouseTracking(true);

  connect(m_mapDisplay,SIGNAL(drawLayersFinished(const QMap<QString, QString>&)),
    this,SLOT(onDrawLayersFinished(const QMap<QString, QString>&)));
    
  te::qt::widgets::ZoomWheel* zoom = new te::qt::widgets::ZoomWheel(m_mapDisplay);
  m_mapDisplay->installEventFilter(zoom);
  
  setWidget(m_mapDisplay);
  
  QGraphicsItem* item = this;
  Context::getInstance()->getScene()->insertItem((ItemObserver*)item);

  m_mapDisplay->show();
}

te::layout::MapItem::~MapItem()
{
  if(m_mapDisplay)
  {
    setWidget(0);
    delete m_mapDisplay;
    m_mapDisplay = 0;
  }
}

void te::layout::MapItem::updateObserver( ContextItem context )
{
  if(!m_model)
    return;

  te::color::RGBAColor** rgba = context.getPixmap();

  if(!rgba)
    return;

  Utils* utils = Context::getInstance()->getUtils();

  if(!utils)
    return;

  te::gm::Envelope box = utils->viewportBox(m_model->getBox());

  if(!box.isValid())
    return;

  QPixmap pixmap;
  QImage* img = 0;

  if(rgba)
  {
    img = te::qt::widgets::GetImage(rgba, box.getWidth(), box.getHeight());
    pixmap = QPixmap::fromImage(*img);
  }

  te::common::Free(rgba, box.getHeight());
  if(img)
    delete img;

  setPixmap(pixmap);
  update();
}

void te::layout::MapItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  QGraphicsProxyWidget::paint(painter, option, widget);

  if(!m_pixmap.isNull())
  {
    QRectF boundRect;
    boundRect = boundingRect();

    painter->save();
    painter->translate( -boundRect.bottomLeft().x(), -boundRect.topRight().y() );
    painter->drawPixmap(boundRect, m_pixmap, QRectF( 0, 0, m_pixmap.width(), m_pixmap.height() ));
    painter->restore(); 
  }

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }

}

void te::layout::MapItem::drawSelection( QPainter* painter)
{
  if(!painter)
  {
    return;
  }

  qreal penWidth = painter->pen().widthF();

  const qreal adj = penWidth / 2;
  const QColor fgcolor(255,255,255);
  const QColor backgroundColor(0,0,0);

  painter->setPen(QPen(backgroundColor, 0, Qt::SolidLine));
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(boundingRect().adjusted(adj, adj, -adj, -adj));

  painter->setPen(QPen(fgcolor, 0, Qt::DashLine));
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(boundingRect().adjusted(adj, adj, -adj, -adj));
}

void te::layout::MapItem::dropEvent( QGraphicsSceneDragDropEvent * event )
{
  //Copy the map from layer tree
  Qt::DropActions actions = event->dropAction();
  if(actions != Qt::CopyAction)
    return;

  getMimeData(event->mimeData());

  QDropEvent dropEvent(QPoint(event->pos().x(), event->pos().y()), event->possibleActions(), event->mimeData(), event->buttons(), event->modifiers());
  QApplication::sendEvent(m_mapDisplay, &dropEvent);
  event->setAccepted(dropEvent.isAccepted());
}

void te::layout::MapItem::dragEnterEvent( QGraphicsSceneDragDropEvent * event )
{
  //Copy the map from layer tree
  Qt::DropActions actions = event->dropAction();
  if(actions != Qt::CopyAction)
    return;

  event->acceptProposedAction();
}

void te::layout::MapItem::dragLeaveEvent( QGraphicsSceneDragDropEvent * event )
{
}

void te::layout::MapItem::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
{

}

void te::layout::MapItem::setPixmap( const QPixmap& pixmap )
{
  m_pixmap = pixmap;

  if(m_pixmap.isNull())
    return;

  Utils* utils = Context::getInstance()->getUtils();
  QPointF point = pos();

  ItemModelObservable* model = (ItemModelObservable*)m_controller->getModel();
  te::gm::Envelope box = model->getBox();

  //If you modify the boundingRect value, you need to inform Graphics View about it by calling QGraphicsItem::prepareGeometryChange();
  QGraphicsObject::prepareGeometryChange();
  setRect(QRectF(0, 0, box.getWidth(), box.getHeight()));
  update();
}

void te::layout::MapItem::setRect( QRectF rect )
{
  if (rect.isEmpty() && !rect.isNull())
    return;

  m_rect = rect;
  update(rect);
}

te::gm::Coord2D te::layout::MapItem::getPosition()
{
  QPointF posF = scenePos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::MapItem::setPos( const QPointF &pos )
{
  QGraphicsItem::setPos(pos);

  refresh();
}

void te::layout::MapItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
  QGraphicsItem::mouseMoveEvent(event);
}

void te::layout::MapItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
  QGraphicsItem::mousePressEvent(event);
}

void te::layout::MapItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
  QGraphicsItem::mouseReleaseEvent(event);

  refresh();
}

void te::layout::MapItem::resizeEvent( QGraphicsSceneResizeEvent * event )
{
  QGraphicsProxyWidget::resizeEvent(event);
}

void te::layout::MapItem::getMimeData( const QMimeData* mime )
{
  QString s = mime->data("application/x-terralib;value=\"DraggedItems\"").constData();
  unsigned long v = s.toULongLong();
  std::vector<te::qt::widgets::AbstractTreeItem*>* draggedItems = reinterpret_cast<std::vector<te::qt::widgets::AbstractTreeItem*>*>(v);

  if(draggedItems->empty())
    return;

  m_treeItem = draggedItems->operator[](0);  
}

std::list<te::map::AbstractLayerPtr> te::layout::MapItem::getVisibleLayers()
{
  std::list<te::map::AbstractLayerPtr> visibleLayers;

  if(!m_treeItem)
    return visibleLayers;

  te::map::AbstractLayerPtr al = m_treeItem->getLayer();

  if(al.get() == 0)
    return visibleLayers;

  std::list<te::map::AbstractLayerPtr> vis;
  te::map::GetVisibleLayers(al, vis);
  // remove folders
  std::list<te::map::AbstractLayerPtr>::iterator vit;
  for(vit = vis.begin(); vit != vis.end(); ++vit)
  {
    if((*vit)->getType() == "DATASETLAYER" || 
      (*vit)->getType() == "QUERYLAYER" ||
      (*vit)->getType() == "RASTERLAYER")
    {
      visibleLayers.push_front(*vit);
    }
  }

  return visibleLayers;
}

te::map::AbstractLayerPtr te::layout::MapItem::getLayer()
{
  te::map::AbstractLayerPtr layer;

  if(!m_treeItem)
    return layer;

  layer = m_treeItem->getLayer();

  return layer;
}

void te::layout::MapItem::onDrawLayersFinished( const QMap<QString, QString>& errors )
{
  if(!errors.empty())
    return;

  te::map::AbstractLayerPtr layer = getLayer();

  if(!m_controller)
    return;

  MapGridController* controller = dynamic_cast<MapGridController*>(m_controller);
  if(controller)
  {
    bool result = controller->refreshLayer(layer);
    if(result)
    {
      redraw();
    }
  }
}
