/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file QMapLayoutItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "QMapLayoutItem.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "LayoutItemModelObservable.h"
#include "LayoutItemObserver.h"
#include "LayoutItemController.h"
#include "../../../color/RGBAColor.h"
#include "../../../../qt/widgets/Utils.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../common/STLUtils.h"
#include "../../../../qt/widgets/canvas/Canvas.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../../qt/widgets/canvas/MultiThreadMapDisplay.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../../qt/af/connectors/MapDisplay.h"
#include "../../../../qt/af/ApplicationController.h"
#include "../../../../qt/af/Project.h"
#include "../../../../qt/af/Utils.h"
#include "../../../../qt/af/events/LayerEvents.h"
#include "../../../../qt/widgets/layer/explorer/AbstractTreeItem.h"
#include "../../../../common/TreeItem.h"
#include "../../../../srs/Converter.h"

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

te::layout::QMapLayoutItem::QMapLayoutItem( LayoutItemController* controller, LayoutObservable* o ) :
  QGraphicsProxyWidget(0),
  LayoutItemObserver(controller, o),
  m_mapDisplay(0),
  m_grabbedByWidget(false)
{
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges);

  setAcceptDrops(true);
  
  m_mapDisplay = new te::qt::widgets::MultiThreadMapDisplay(QSize(m_model->getBox().getWidth(), m_model->getBox().getHeight()), true);
 /* m_mapDisplay->setAcceptDrops(true);
  m_mapDisplay->setBackgroundColor(Qt::gray);
  m_mapDisplay->setResizeInterval(0);
  m_mapDisplay->setMouseTracking(true);*/
  //m_mapDisplay->installEventFilter(this);

  te::qt::af::MapDisplay* listenerDisplay = new te::qt::af::MapDisplay(m_mapDisplay);
  te::qt::af::ApplicationController::getInstance().addListener(listenerDisplay);
  
  m_mapDisplay->setAcceptDrops(true);
  m_mapDisplay->setBackgroundColor(Qt::gray);
  m_mapDisplay->setResizeInterval(0);
  m_mapDisplay->setMouseTracking(true);

  setWidget(m_mapDisplay);
  
  QGraphicsItem* item = this;
  LayoutContext::getInstance()->getScene()->insertItem((LayoutItemObserver*)item);

  m_mapDisplay->show();
}

te::layout::QMapLayoutItem::~QMapLayoutItem()
{
  if(m_mapDisplay)
  {
    setWidget(0);
    delete m_mapDisplay;
    m_mapDisplay = 0;
  }
}

void te::layout::QMapLayoutItem::updateObserver( ContextLayoutItem context )
{
  te::color::RGBAColor** rgba = context.getPixmap();

  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();

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

void te::layout::QMapLayoutItem::dropEvent( QGraphicsSceneDragDropEvent * event )
{
  //Copy the map from layer tree
  Qt::DropActions actions = event->dropAction();
  if(actions != Qt::CopyAction)
    return;

  QDropEvent dropEvent(QPoint(event->pos().x(), event->pos().y()), event->possibleActions(), event->mimeData(), event->buttons(), event->modifiers());
  QApplication::sendEvent(m_mapDisplay, &dropEvent);
  event->setAccepted(dropEvent.isAccepted());
}

void te::layout::QMapLayoutItem::dragEnterEvent( QGraphicsSceneDragDropEvent * event )
{
  //Copy the map from layer tree
  Qt::DropActions actions = event->dropAction();
  if(actions != Qt::CopyAction)
    return;

  event->acceptProposedAction();
}

void te::layout::QMapLayoutItem::dragLeaveEvent( QGraphicsSceneDragDropEvent * event )
{
}

void te::layout::QMapLayoutItem::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
{

}

void te::layout::QMapLayoutItem::setPixmap( const QPixmap& pixmap )
{
  m_pixmap = pixmap;

  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();
  QPointF point = pos();

  LayoutItemModelObservable* model = (LayoutItemModelObservable*)m_controller->getModel();
  te::gm::Envelope box = model->getBox();

  //If you modify the boundingRect value, you need to inform Graphics View about it by calling QGraphicsItem::prepareGeometryChange();
  QGraphicsObject::prepareGeometryChange();
  setRect(QRectF(0, 0, box.getWidth(), box.getHeight()));
  update();
}

void te::layout::QMapLayoutItem::setRect( QRectF rect )
{
  if (rect.isEmpty() && !rect.isNull())
    return;

  m_rect = rect;
  update(rect);
}

te::gm::Coord2D te::layout::QMapLayoutItem::getPosition()
{
  QPointF posF = scenePos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::QMapLayoutItem::setPosition( const double& x, const double& y )
{
  setPos(x,y);
}

void te::layout::QMapLayoutItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
  QGraphicsItem::mouseMoveEvent(event);
}

void te::layout::QMapLayoutItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
  QGraphicsItem::mousePressEvent(event);
}

void te::layout::QMapLayoutItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
  QGraphicsItem::mouseReleaseEvent(event);
}

