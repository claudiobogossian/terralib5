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
#include "../../../../maptools/AbstractLayer.h"
#include "../../../../qt/widgets/layer/explorer/AbstractTreeItem.h"
#include "../../../../common/TreeItem.h"
#include "../../../../srs/Converter.h"
#include "../../../../qt/widgets/tools/ZoomWheel.h"

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

te::layout::MapItem::MapItem( ItemController* controller, Observable* o ) :
  QGraphicsProxyWidget(0),
  ItemObserver(controller, o),
  m_mapDisplay(0),
  m_grabbedByWidget(false)
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
  te::color::RGBAColor** rgba = context.getPixmap();

  Utils* utils = Context::getInstance()->getUtils();

  te::gm::Envelope box = utils->viewportBox(m_model->getBox());

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

void te::layout::MapItem::dropEvent( QGraphicsSceneDragDropEvent * event )
{
  //Copy the map from layer tree
  Qt::DropActions actions = event->dropAction();
  if(actions != Qt::CopyAction)
    return;

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

void te::layout::MapItem::setPosition( const double& x, const double& y )
{
  setPos(x,y);
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
}

void te::layout::MapItem::resizeEvent( QGraphicsSceneResizeEvent * event )
{
  QGraphicsProxyWidget::resizeEvent(event);
}

