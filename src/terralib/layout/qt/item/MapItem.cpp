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
#include "../../../qt/widgets/layer/explorer/AbstractTreeItem.h"
#include "GridMapItem.h"

// STL
#include <vector>
#include <memory>
#include <algorithm>

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

te::layout::MapItem::MapItem( ItemController* controller, Observable* o, bool invertedMatrix ) :
  ParentItem<QGraphicsProxyWidget>(controller, o, invertedMatrix),
  m_mime(0),
  m_mapDisplay(0),
  m_grabbedByWidget(false),
  m_tool(0),
  m_wMargin(0),
  m_hMargin(0),
  m_pixmapIsDirty(false)
{    
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

  setFlag(QGraphicsItem::ItemClipsChildrenToShape);

  m_mapSize = QSize(box.getWidth(), box.getHeight());
  m_mapDisplay = new te::qt::widgets::MultiThreadMapDisplay(m_mapSize, true);
  m_mapDisplay->setSynchronous(true);
  m_mapDisplay->setAcceptDrops(true);

  m_mapDisplay->setBackgroundColor(Qt::transparent);
  m_mapDisplay->setResizeInterval(0);
  m_mapDisplay->setMouseTracking(true);

  connect(m_mapDisplay,SIGNAL(drawLayersFinished(const QMap<QString, QString>&)),
    this,SLOT(onDrawLayersFinished(const QMap<QString, QString>&)));

  m_zoomWheel = new te::qt::widgets::ZoomWheel(m_mapDisplay);
  m_mapDisplay->installEventFilter(m_zoomWheel);

  setWidget(m_mapDisplay);
    
  calculateFrameMargin();

  setWindowFrameMargins(m_wMargin, m_hMargin, m_wMargin, m_hMargin);

  m_mapDisplay->show();

  /*
    MapItem will handle all the events. 
    For example, the event of mouse click on the child item 
    won't be handled by child item.
  */
  setHandlesChildEvents(true);
}

te::layout::MapItem::~MapItem()
{
  clearCurrentTool();
  if(m_zoomWheel)
  {
    m_mapDisplay->removeEventFilter(m_zoomWheel);
    delete m_zoomWheel;
    m_zoomWheel = 0;
  }
}

void te::layout::MapItem::updateObserver( ContextItem context )
{
  if(!m_model)
    return;

  Utils* utils = Context::getInstance().getUtils();

  if(!utils)
    return;

  calculateFrameMargin();
  setWindowFrameMargins(m_wMargin, m_hMargin, m_wMargin, m_hMargin);
      
  updateMapDisplay();

  reloadLayers();

  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(model)
  {
    te::gm::Envelope mapBox = utils->viewportBox(model->getMapBox());
    double w = mapBox.getWidth();
    double h = mapBox.getHeight();

    /* resize */
    if(w != m_mapDisplay->getWidth() 
      || h != m_mapDisplay->getHeight())
    {
      QPointF pt = scenePos();
      m_mapDisplay->setGeometry(pt.x(), pt.y(), w, h);
    }

    m_mapDisplay->refresh();

    calculateFrameMargin();
  }

  refresh();

  update();
}

QPointF remapPointToViewport(const QPointF& point, const QRectF& item, const QRectF& widget)
{
  double resX = widget.width() / item.width();
  double resY = widget.height() / item.height();

  QMatrix matrix;
  matrix.scale(resX, -resY);
  matrix.translate(-item.bottomLeft().x(), -item.bottomLeft().y());

  QPointF remappedPoint = matrix.map(point);
  return remappedPoint;
}

void te::layout::MapItem::drawItem( QPainter * painter )
{
  if(!m_mapDisplay || !painter)
    return;

  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(!model)
  {
    return;
  }

  QRectF boundRect;

  if(model->getBox().getWidth() < model->getMapBox().getWidth()
    || model->getBox().getHeight() < model->getMapBox().getHeight())
  {
    boundRect = boundingRect();
  }
  else
  {
    double x = model->getDisplacementX();
    double y = model->getDisplacementY();
    boundRect = QRectF(x, y, model->getMapBox().getWidth(), model->getMapBox().getHeight());
  }

  if( m_pixmapIsDirty == true)
  {
    m_pixmapIsDirty = false;
    generateMapPixmap();
  }

  painter->save();
  painter->setClipRect(boundRect);
  painter->drawPixmap(boundRect, m_pixmap, m_pixmap.rect());
  
  painter->restore();
}

void te::layout::MapItem::dropEvent( QGraphicsSceneDragDropEvent * event )
{
  //Copy the map from layer tree
  Qt::DropActions actions = event->dropAction();
  if(actions != Qt::CopyAction)
    return;

  getMimeData(event->mimeData());

  reloadLayers(false);

  redraw();
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
}

te::gm::Coord2D te::layout::MapItem::getPosition()
{
  /* Correctly position the object and change the origin for bottomLeft */
  double x = 0;
  double y = 0;
  MapModel* model = dynamic_cast<MapModel*>(m_model);
  te::gm::Envelope box;
  if(model)
  {
    box = model->getMapBox();
    x = model->getDisplacementX() * 2;
    y = model->getDisplacementY() * 2;
  }
  else
  {
    box = m_model->getBox();
  }

  QPointF posF = scenePos();

  qreal valuex = posF.x() - x;
  qreal valuey = posF.y() - y;

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::MapItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  if(!iUtils)
    return;

  if(!iUtils->isCurrentMapTools())
  {
    clearCurrentTool();
    ParentItem::mouseMoveEvent(event);
  }
  else
  {
    QRectF rect = boundingRect();
    QPointF point = event->pos();
    QPointF remappedPoint = remapPointToViewport(point, rect, m_mapDisplay->rect());

    QMouseEvent mouseEvent(QEvent::MouseMove, remappedPoint.toPoint(),
    event->button(),event->buttons(), event->modifiers());
    QApplication::sendEvent(m_mapDisplay, &mouseEvent);
    event->setAccepted(mouseEvent.isAccepted());

    this->update();
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
    ParentItem::mousePressEvent(event);
  }
  else
  {
    QRectF rect = boundingRect();
    QPointF point = event->pos();
    QPointF remappedPoint = remapPointToViewport(point, rect, m_mapDisplay->rect());

    QMouseEvent mouseEvent(QEvent::MouseButtonPress, remappedPoint.toPoint(),
    event->button(),event->buttons(), event->modifiers());
    QApplication::sendEvent(m_mapDisplay, &mouseEvent);
    event->setAccepted(mouseEvent.isAccepted());

    this->update();
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
    ParentItem::mouseReleaseEvent(event); 
  }
  else
  {
    QRectF rect = boundingRect();
    QPointF point = event->pos();
    QPointF remappedPoint = remapPointToViewport(point, rect, m_mapDisplay->rect());

    QMouseEvent mouseEvent(QEvent::MouseButtonRelease, remappedPoint.toPoint(),
    event->button(),event->buttons(), event->modifiers());
    QApplication::sendEvent(m_mapDisplay, &mouseEvent);
    event->setAccepted(mouseEvent.isAccepted());

    this->update();
  }
  refresh();
}

void te::layout::MapItem::getMimeData( const QMimeData* mime )
{
  QString s = mime->data("application/x-terralib;value=\"DraggedItems\"").constData();
  unsigned long v = s.toULongLong();
  std::vector<te::qt::widgets::AbstractTreeItem*>* draggedItems = reinterpret_cast<std::vector<te::qt::widgets::AbstractTreeItem*>*>(v);

  if(draggedItems->empty())
    return;

  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(!model)
  {
    return;
  }

  for(unsigned int i = 0 ; i < draggedItems->size() ; ++i)
  {
    te::qt::widgets::AbstractTreeItem* treeItem = draggedItems->operator[](i);  
    model->addLayer(treeItem->getLayer());
  }
}

std::list<te::map::AbstractLayerPtr> te::layout::MapItem::getVisibleLayers()
{
  std::list<te::map::AbstractLayerPtr> visibleLayers;

  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(!model)
  {
    return visibleLayers;
  }

  if(!model->isLoadedLayer())
    return visibleLayers;

  std::list<te::map::AbstractLayerPtr> al = model->getLayers();
  
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

void te::layout::MapItem::onDrawLayersFinished( const QMap<QString, QString>& errors )
{
  if(!errors.empty())
    return;

  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(!model)
  {
    return;
  }

  if(!model->isLoadedLayer())
    return;
  
  m_pixmapIsDirty = true;

  update();
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

  te::layout::Context::getInstance().setMode(mode);
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

te::color::RGBAColor** te::layout::MapItem::getRGBAColorImage(int &w, int &h)
{
  te::color::RGBAColor** teImg = 0;

  QImage img = m_pixmap.toImage();
  img = img.mirrored();

  if(img.isNull())
  {
    return teImg;
  }

  w = img.width();
  h = img.height();

  teImg = te::qt::widgets::GetImage(&img);
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
    generator.mirrored();
  }

  painter.end();
  return generator;
}

void te::layout::MapItem::calculateFrameMargin()
{
  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(!model)
    return;
  
  m_wMargin = model->getDisplacementX();
  m_hMargin = model->getDisplacementY();
}

void te::layout::MapItem::generateMapPixmap()
{
  m_pixmap = QPixmap(m_mapDisplay->width(), m_mapDisplay->height());
  m_pixmap.fill(Qt::transparent);

  QPainter localPainter(&m_pixmap);
  m_mapDisplay->render(&localPainter);
  localPainter.end();

  QImage image = m_pixmap.toImage();
  image = image.mirrored();

  m_pixmap = QPixmap::fromImage(image);
}

void te::layout::MapItem::updateMapDisplay()
{
  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(!model)
  {
    return;
  }
  
  if(model->isLoadedLayer())
  {
    return;
  }
  
  AbstractProxyProject* project = Context::getInstance().getProxyProject();
  if(!project)
    return;

  std::list<te::map::AbstractLayerPtr> layerList;

  std::vector<std::string> names = model->getLayerNames();

  std::vector<std::string>::const_iterator it = names.begin();

  for( ; it != names.end() ; ++it)
  {
    std::string name = (*it);
    te::map::AbstractLayerPtr layer = project->contains(name);
    layerList.push_back(layer);
    model->addLayer(layer);
  }

  m_mapDisplay->setLayerList(layerList);

  std::list<te::map::AbstractLayerPtr>::const_iterator itl = layerList.begin(); 
  if(itl != layerList.end())
  {
    te::map::AbstractLayerPtr al = (*itl);
    te::gm::Envelope e = al->getExtent();

    m_mapDisplay->setSRID(al->getSRID(), false);
    m_mapDisplay->setExtent(e, true);
  }
}

void te::layout::MapItem::recalculateBoundingRect()
{
  MapModel* model = dynamic_cast<MapModel*>(m_model);
  te::gm::Envelope box = model->getBox();

  if(m_rect.width() != box.getWidth()
    || m_rect.height() != box.getHeight())
  {
    prepareGeometryChange();
    m_rect = QRectF(0., 0., box.getWidth(), box.getHeight()); 

    //update model
    te::gm::Envelope env(m_model->getBox());
    env.m_urx = env.m_llx + m_rect.width();
    env.m_ury = env.m_lly + m_rect.height();
    m_controller->setBox(env);
  }
}

QRectF te::layout::MapItem::boundingRect() const
{
  MapModel* model = dynamic_cast<MapModel*>(m_model);
  te::gm::Envelope box = model->getBox();

  QRectF rect(0., 0., box.getWidth(), box.getHeight()); 
  return rect;
}

void te::layout::MapItem::drawBackground( QPainter* painter )
{
  if ( !painter )
  {
    return;
  }

  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(!model)
    return;

  te::color::RGBAColor clrBack = model->getBackgroundColor();
  QColor backColor;
  backColor.setRed(clrBack.getRed());
  backColor.setGreen(clrBack.getGreen());
  backColor.setBlue(clrBack.getBlue());
  backColor.setAlpha(clrBack.getAlpha());

  double x = model->getDisplacementX();
  double y = model->getDisplacementY();
  QRectF boundRect = QRectF(x, y, model->getMapBox().getWidth(), model->getMapBox().getHeight());

  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setBrush(QBrush(backColor));
  painter->setBackground(QBrush(backColor));
  painter->setRenderHint( QPainter::Antialiasing, true );
  painter->drawRect(boundRect);
  painter->restore();
}

void te::layout::MapItem::drawSelection( QPainter* painter )
{
  if(!painter)
  {
    return;
  }

  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(!model)
    return;

  double x = model->getDisplacementX();
  double y = model->getDisplacementY();
  QRectF boundRect = QRectF(x, y, model->getMapBox().getWidth(), model->getMapBox().getHeight());

  painter->save();

  qreal penWidth = painter->pen().widthF();

  const qreal adj = penWidth / 2;
  const QColor fgcolor(0,255,0);
  const QColor backgroundColor(0,0,0);

  QRectF rtAdjusted = boundRect.adjusted(adj, adj, -adj, -adj);

  QPen penBackground(backgroundColor, 0, Qt::SolidLine);
  painter->setPen(penBackground);
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(rtAdjusted);

  QPen penForeground(fgcolor, 0, Qt::DashLine);
  painter->setPen(penForeground);
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(rtAdjusted);

  painter->setPen(Qt::NoPen);
  QBrush brushEllipse(fgcolor);
  painter->setBrush(fgcolor);

  double w = 2.0;
  double h = 2.0;
  double half = 1.0;

  painter->drawRect(rtAdjusted.center().x() - half, rtAdjusted.center().y() - half, w, h); // center
  painter->drawRect(rtAdjusted.bottomLeft().x() - half, rtAdjusted.bottomLeft().y() - half, w, h); // left-top
  painter->drawRect(rtAdjusted.bottomRight().x() - half, rtAdjusted.bottomRight().y() - half, w, h); // right-top
  painter->drawRect(rtAdjusted.topLeft().x() - half, rtAdjusted.topLeft().y() - half, w, h); // left-bottom
  painter->drawRect(rtAdjusted.topRight().x() - half, rtAdjusted.topRight().y() - half, w, h); // right-bottom

  painter->restore();
}

void te::layout::MapItem::drawBorder( QPainter* painter )
{
  if ( !painter )
  {
    return;
  }

  if(!m_model)
    return;

  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(!model)
    return;

  if(!model->isBorder())
    return;

  te::color::RGBAColor clrBack = model->getBorderColor();
  QColor borderColor;
  borderColor.setRed(clrBack.getRed());
  borderColor.setGreen(clrBack.getGreen());
  borderColor.setBlue(clrBack.getBlue());
  borderColor.setAlpha(clrBack.getAlpha());

  double x = model->getDisplacementX();
  double y = model->getDisplacementY();
  QRectF boundRect = QRectF(x, y, model->getMapBox().getWidth(), model->getMapBox().getHeight());

  painter->save();
  QPen penBackground(borderColor, 0, Qt::SolidLine);
  painter->setPen(penBackground);
  painter->setBrush(Qt::NoBrush);
  painter->setRenderHint( QPainter::Antialiasing, true );
  painter->drawRect(boundRect);
  painter->restore();
}

void te::layout::MapItem::reloadLayers(bool draw)
{
  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(!model)
  {
    return;
  }

  std::list<te::map::AbstractLayerPtr> layerList = model->getLayers();

  if(!m_oldLayers.empty())
  {
    if(!hasListLayerChanged())
    {
      return;
    }
  }

  m_oldLayers = layerList;
  m_pixmapIsDirty = true;

  m_mapDisplay->setLayerList(layerList);

  if(layerList.empty() == true)
  {
    return;
  }

  std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

  te::map::AbstractLayerPtr al = (*it);

  te::gm::Envelope e = model->maxLayerExtent();  


  m_mapDisplay->setSRID(al->getSRID(), false);
  m_mapDisplay->setExtent(e, draw);

  m_pixmapIsDirty = true;
}

bool te::layout::MapItem::hasListLayerChanged()
{
  bool result = false;
  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(!model)
  {
    return false;
  }

  std::list<te::map::AbstractLayerPtr> layerList = model->getLayers();
  std::list<te::map::AbstractLayerPtr>::const_iterator it = layerList.begin();

  if(layerList.size() != m_oldLayers.size())
  {
    return true;
  }

  for( ; it != layerList.end() ; ++it)
  {
    te::map::AbstractLayerPtr layer = (*it);
    if(std::find(m_oldLayers.begin(), m_oldLayers.end(), layer) == m_oldLayers.end())
    {
      result = true;
      break;
    }
  }

  return result;
}

void te::layout::MapItem::redraw( bool bRefresh /*= true*/ )
{
  if(m_oldLayers.empty())
  {
    return;
  }

  ContextItem context;
  updateObserver(context);
}

bool te::layout::MapItem::checkTouchesCorner( const double& x, const double& y )
{  
  bool result = true;

  MapModel* model = dynamic_cast<MapModel*>(m_model);
  if(!model)
  {
    return result;
  }

  te::gm::Envelope boxMM = model->getMapBox();
  
  QRectF bRect(m_wMargin, m_hMargin, boxMM.getWidth() + m_wMargin, boxMM.getHeight() + m_hMargin);
  double margin = 10.; //precision

  QPointF ll = bRect.bottomLeft();
  QPointF lr = bRect.bottomRight();
  QPointF tl = bRect.topLeft();
  QPointF tr = bRect.topRight();

  if((x >= (ll.x() - margin) && x <= (ll.x() + margin))
    && (y >= (ll.y() - margin) && y <= (ll.y() + margin)))
  {
    QGraphicsItem::setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPLowerLeft;
  }
  else if((x >= (lr.x() - margin) && x <= (lr.x() + margin))
    && (y >= (lr.y() - margin) && y <= (lr.y() + margin)))
  {
    QGraphicsItem::setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPLowerRight;
  }
  else if((x >= (tl.x() - margin) && x <= (tl.x() + margin))
    && (y >= (tl.y() - margin) && y <= (tl.y() + margin)))
  {
    QGraphicsItem::setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPTopLeft;
  }
  else if((x >= (tr.x() - margin) && x <= (tr.x() + margin))
    && (y >= (tr.y() - margin) && y <= (tr.y() + margin)))
  {
    QGraphicsItem::setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPTopRight;
  }
  else
  {
    QGraphicsItem::setCursor(Qt::ArrowCursor);
    m_enumSides = TPNoneSide;
    result = false;
  }

  return result;
}

bool te::layout::MapItem::canBeChild( ItemObserver* item )
{
  GridMapItem* gItem = dynamic_cast<GridMapItem*>(item);
  if(gItem)
  {
    return true;
  }
  return false;
}

void te::layout::MapItem::contextUpdated()
{
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

  QSizeF currentSize = this->size();
  QSizeF newSize(box.getWidth(), box.getHeight());
  if(currentSize != newSize)
  {
    QPointF pt = scenePos();

    m_mapDisplay->setGeometry(pt.x(), pt.y(), newSize.width(), newSize.height());
    m_pixmapIsDirty = true;
  }
}
