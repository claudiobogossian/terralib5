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
#include "../../core/pattern/singleton/Context.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/ItemModelObservable.h"
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../../qt/widgets/canvas/Canvas.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../qt/widgets/canvas/MultiThreadMapDisplay.h"
#include "../../../common/TreeItem.h"
#include "../../../srs/Converter.h"
#include "../../../qt/widgets/tools/ZoomWheel.h"
#include "../../../maptools/Utils.h"
#include "../../item/MapController.h"
#include "../../../qt/widgets/tools/Pan.h"
#include "../../../qt/widgets/tools/ZoomArea.h"
#include "../../../qt/widgets/tools/ZoomClick.h"
#include "../core/ItemUtils.h"
#include "../../item/MapModel.h"
#include "../../../common/StringUtils.h"
#include "../../core/enum/Enums.h"
#include "../core/Scene.h"
#include "../../core/pattern/proxy/AbstractProxyProject.h"

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
#include <QEvent>
#include <QPoint>
#include <QMimeData>
#include <QColor>

#include <QTextEdit>

te::layout::MapItem::MapItem( ItemController* controller, Observable* o ) :
  QGraphicsProxyWidget(0),
  ItemObserver(controller, o),
  m_mapDisplay(0),
  m_grabbedByWidget(false),
  m_treeItem(0),
  m_tool(0),
  m_hMargin(0),
  m_wMargin(0),
  m_layer(0)
{
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges
    | QGraphicsItem::ItemIsFocusable);
        
  setAcceptDrops(true);

  m_invertedMatrix = true;

  m_nameClass = std::string(this->metaObject()->className());
  
  Utils* utils = Context::getInstance().getUtils();
  te::gm::Envelope box;
  
  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(model)
  {
    box = utils->viewportBox(model->getMapBox());
  }
  else
  {
    box = utils->viewportBox(m_model->getBox());
  }
    
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
    
  QTextEdit* txt = new QTextEdit("MAP_ITEM_EDIT");
  //setWidget(txt);

  QGraphicsItem* item = this;
  Context::getInstance().getScene()->insertItem((ItemObserver*)item);
  
  calculateFrameMargin();
      
  m_mapDisplay->show();
}

te::layout::MapItem::~MapItem()
{
  if(m_tool)
  {
    delete m_tool;
    m_tool = 0;
  }

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

  Utils* utils = context.getUtils();

  if(!utils)
    return;

  te::gm::Envelope box = utils->viewportBox(m_model->getBox());
  
  if(!box.isValid())
    return;
  
  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(model)
  {
    te::gm::Envelope mapBox = utils->viewportBox(model->getMapBox());
    double w = mapBox.getWidth();
    double h = mapBox.getHeight();

    /* This item ignores the transformations of the scene, so comes with no zoom. 
    His transformation matrix is the inverse scene, understanding the pixel 
    coordinates, and its position can only be given in the scene coordinates(mm). 
    For these reasons, it is necessary to resize it.*/
    if(w != m_mapDisplay->getWidth() 
      || h != m_mapDisplay->getHeight())
    {
      QRect rectGeom = m_mapDisplay->geometry();
      m_mapDisplay->setGeometry(rectGeom.x(), rectGeom.y(), w, h);
    }

    te::color::RGBAColor clr = model->getMapBackgroundColor();
    QColor qcolor;
    qcolor.setRed(clr.getRed());
    qcolor.setGreen(clr.getGreen());
    qcolor.setBlue(clr.getBlue());
    qcolor.setAlpha(clr.getAlpha());
    m_mapDisplay->setBackgroundColor(qcolor);
    m_mapDisplay->refresh();

    calculateFrameMargin();
  }

  te::color::RGBAColor** rgba = context.getPixmap();

  if(!rgba)
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
  QRectF boundRect;
  boundRect = boundingRect();

  painter->save();
  QRectF rtSource( 0, 0, m_pixmap.width(), m_pixmap.height() );
  painter->drawPixmap(boundRect, m_pixmap, rtSource);
  painter->restore(); 

  QGraphicsProxyWidget::paint(painter, option, widget);

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
  const QColor fgcolor(0,255,0);
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

  te::map::AbstractLayerPtr al = m_treeItem->getLayer();
  m_mapDisplay->changeData(al);
  m_layer = al;
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
}

void te::layout::MapItem::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
{

}

void te::layout::MapItem::setPixmap( const QPixmap& pixmap )
{
  m_pixmap = pixmap;

  if(m_pixmap.isNull())
    return;

  QPointF point = pos();

  te::gm::Envelope box = m_model->getBox();
  
  //If you modify the boundingRect value, you need to inform Graphics View about it by calling QGraphicsItem::prepareGeometryChange();
  QGraphicsObject::prepareGeometryChange();

  setRect(QRectF(0, 0, box.getWidth(), box.getHeight()));
  update();
}

te::gm::Coord2D te::layout::MapItem::getPosition()
{
  double x = 0;
  double y = 0;
  
  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(model)
  {
    x = model->getDisplacementX();
    y = model->getDisplacementY();
  }

  QPointF posF = scenePos();
  qreal valuex = posF.x() - x;
  qreal valuey = posF.y() - y;
      
  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::MapItem::setPos( const QPointF &pos )
{
  QPointF pt(pos.x() - transform().dx(), pos.y() - transform().dy());
  QGraphicsItem::setPos(pt);
  refresh();
}

void te::layout::MapItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  if(!iUtils)
    return;

  if(!iUtils->isCurrentMapTools())
  {
    clearCurrentTool();
    QGraphicsItem::mouseMoveEvent(event);
  }
  else
  {
    QMouseEvent mouseEvent(QEvent::MouseMove,QPoint(event->pos().x(), event->pos().y()),
      event->button(),event->buttons(), event->modifiers());
    QApplication::sendEvent(m_mapDisplay, &mouseEvent);
    event->setAccepted(mouseEvent.isAccepted());
  }
}

void te::layout::MapItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  if(!iUtils)
    return;

  if(!iUtils->isCurrentMapTools())
  {
    clearCurrentTool();
    QGraphicsItem::mousePressEvent(event);
  }
  else
  {
    QMouseEvent mouseEvent(QEvent::MouseButtonPress,QPoint(event->pos().x(), event->pos().y()),
    event->button(),event->buttons(), event->modifiers());
    QApplication::sendEvent(m_mapDisplay, &mouseEvent);
    event->setAccepted(mouseEvent.isAccepted());
  }
}

void te::layout::MapItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  if(!iUtils)
    return;

  if(!iUtils->isCurrentMapTools())
  {
    clearCurrentTool();
    QGraphicsItem::mouseReleaseEvent(event); 
  }
  else
  {
    QMouseEvent mouseEvent(QEvent::MouseButtonRelease,QPoint(event->pos().x(), event->pos().y()),
    event->button(),event->buttons(), event->modifiers());
    QApplication::sendEvent(m_mapDisplay, &mouseEvent);
    event->setAccepted(mouseEvent.isAccepted());
  }
  
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

  MapController* controller = dynamic_cast<MapController*>(m_controller);
  if(controller)
  {
    bool result = controller->refreshLayer(layer);
    if(result)
    {
      redraw();
    }
  }

  generateMapPixmap();
}

void te::layout::MapItem::setZValue( qreal z )
{
  QGraphicsItem::setZValue(z);
  setZValueItem(z);

}

int te::layout::MapItem::getZValueItem()
{
  return QGraphicsItem::zValue();
}

void te::layout::MapItem::setCurrentTool( te::qt::widgets::AbstractTool* tool )
{
  m_tool = tool;
  
  m_mapDisplay->installEventFilter(m_tool);
}

void te::layout::MapItem::clearCurrentTool()
{
  if(m_tool)
  {
    m_mapDisplay->removeEventFilter(m_tool);

    delete m_tool;
    m_tool = 0;
  }
}

void te::layout::MapItem::changeCurrentTool( EnumType* mode )
{
  clearCurrentTool();

  EnumModeType* type = Enums::getInstance().getEnumModeType();

  if(mode == type->getModeMapPan())
  {
    te::qt::widgets::Pan* pan = new te::qt::widgets::Pan(m_mapDisplay, Qt::OpenHandCursor, Qt::ClosedHandCursor);
    setCurrentTool(pan);
  }
  if(mode == type->getModeMapZoomIn())
  {
    //Zoom In
    std::string icon_path_zoom_area = "layout-map-zoom-in";
    QCursor zoomAreaCursor(QIcon::fromTheme(icon_path_zoom_area.c_str()).pixmap(QSize(10,10)));
    te::qt::widgets::ZoomArea* zoomArea = new te::qt::widgets::ZoomArea(m_mapDisplay, zoomAreaCursor);
    setCurrentTool(zoomArea);
  }
  if(mode == type->getModeMapZoomOut())
  {
    //Zoom Out
    std::string icon_path_zoom_out = "layout-map-zoom-out";
    QCursor zoomOutCursor(QIcon::fromTheme(icon_path_zoom_out.c_str()).pixmap(QSize(10,10)));
    te::qt::widgets::ZoomClick* zoomOut = new te::qt::widgets::ZoomClick(m_mapDisplay, zoomOutCursor, 2.0, te::qt::widgets::Zoom::Out);
    setCurrentTool(zoomOut);
  }
}

QVariant te::layout::MapItem::itemChange( GraphicsItemChange change, const QVariant & value )
{
  return QGraphicsProxyWidget::itemChange(change, value);
}

void te::layout::MapItem::applyRotation()
{
  if(!m_model)
    return;

  ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
  if(!model)
    return;

  double angle = model->getAngle();

  if(angle == 0.)
    return;

  QPointF center = boundingRect().center();

  double centerX = center.x();
  double centerY = center.y();
  
  setTransform(QTransform().translate(centerX, centerY).rotate(angle).translate(-centerX, -centerY));
}

te::color::RGBAColor** te::layout::MapItem::getImage()
{
  QImage img = generateImage();
  te::color::RGBAColor** teImg = te::qt::widgets::GetImage(&img);
  return teImg;
}

QImage te::layout::MapItem::generateImage()
{
  QColor color(0, 0, 255, 0);

  QImage generator(m_pixmap.width(), m_pixmap.height(), QImage::Format_ARGB32);
  generator.fill(color);

  QPainter painter;
  painter.begin(&generator);
  
  QPoint pt(m_wMargin, m_hMargin);
  widget()->render(&painter, pt);

  if(!m_pixmap.isNull())
  {
    QRectF rectF(0, 0, m_pixmap.width(), m_pixmap.height());
    painter.save();
    painter.drawPixmap(rectF, m_pixmap, rectF);
    painter.restore(); 
  }
  
  painter.end();
  return generator;
}

void te::layout::MapItem::calculateFrameMargin()
{
  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(!model)
    return;

  Utils* utils = Context::getInstance().getUtils();

  if(!utils)
    return;

  te::gm::Envelope box = utils->viewportBox(m_model->getBox());
  te::gm::Envelope mapBox = utils->viewportBox(model->getMapBox());

  m_wMargin = (box.getWidth() - mapBox.getWidth()) / 2.;
  m_hMargin = (box.getHeight() - mapBox.getHeight()) / 2.;
}

void te::layout::MapItem::generateMapPixmap()
{
  QRegion srcRegion( 0, 0, this->widget()->width(), this->widget()->height());

  QColor color(255, 255, 255, 0);
  QPixmap img(this->widget()->width(), this->widget()->height());
  img.fill(Qt::transparent);

  QPainter ptr(&img);
  QPoint pt(0, 0);
  this->widget()->render(&ptr, pt, srcRegion);
  
  m_mapPixmap = img; 
}

QRectF te::layout::MapItem::boundingRect() const
{
  return QGraphicsProxyWidget::boundingRect();
  //return m_rect;
}

void te::layout::MapItem::setRect( QRectF rect )
{
  if (rect.isEmpty() && !rect.isNull())
    return;

  m_rect = rect;
  update(rect);
}

void te::layout::MapItem::updateProperties( te::layout::Properties* properties )
{
  if(!m_controller)
    return;

  m_controller->updateProperties(properties);

  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(!model)
    return;

  if(model->isLoadedLayer())
  {
    redraw();
    return;
  }
  
  std::string name = model->getNameLayer();

  if(name.compare("") == 0)
    return;

  AbstractProxyProject* project = Context::getInstance().getProxyProject();
  if(!project)
    return;

  te::map::AbstractLayerPtr layer = project->contains(name);
  m_mapDisplay->changeData(layer);

  MapController* controller = dynamic_cast<MapController*>(m_controller);
  if(!controller)
    return;

  bool result = controller->refreshLayer(layer);
  if(result)
  {
    redraw();
  }
}
