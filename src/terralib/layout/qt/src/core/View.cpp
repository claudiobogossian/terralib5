/*  Copyright (C) 2013-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file View.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "View.h"
#include "Context.h"
#include "EnumMode.h"
#include "ItemGroup.h"
#include "Scene.h"
#include "VisualizationArea.h"
#include "OutsideArea.h"
#include "../../../../color/RGBAColor.h"
#include "PropertiesOutside.h"
#include "ObjectInspectorOutside.h"
#include "ToolbarOutside.h"
#include "BuildGraphicsItem.h"

// STL
#include <math.h>

// Qt
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QString>
#include <QGraphicsView>
#include <QtGui>
#include <QGraphicsItem>
#include <time.h>
#include <QMatrix> 
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>

#define _psPointInMM 0.352777778 //<! 1 PostScript point in millimeters
#define _inchInPSPoints 72 //<! 1 Inch in PostScript point
#define _inchInMillimeters 25.4 //<! 1 Inch in millimeters

te::layout::View::View( QWidget* widget) : 
  QGraphicsView(new QGraphicsScene, widget),
  m_outsideArea(0),
  m_visualizationArea(0),
  m_buildItems(0)
{
  //Use ScrollHand Drag Mode to enable Panning
  //You do need the enable scroll bars for that to work.
  setDragMode(RubberBandDrag);

  m_buildItems = new BuildGraphicsItem;
}

te::layout::View::~View()
{
  if(m_visualizationArea)
  {
    delete m_visualizationArea;
    m_visualizationArea = 0;
  }

  if(m_buildItems)
  {
    delete m_buildItems;
    m_buildItems = 0;
  }
}

void te::layout::View::mousePressEvent( QMouseEvent * event )
{
  QGraphicsView::mousePressEvent(event);
  
  QPointF scenePos = mapToScene(event->pos());
  te::gm::Coord2D coord(scenePos.x(), scenePos.y());

  createItem(coord); 
}

void te::layout::View::wheelEvent( QWheelEvent *event )
{
  scaleView(pow((double)2, -event->delta() / 240.0));
}

void te::layout::View::scaleView( qreal scaleFactor )
{
  //Window - Mundo
  double dpiX = this->logicalDpiX();
  double dpiY = this->logicalDpiY();
  double scaleX = (dpiX / 25.4) * scaleFactor;
  double scaleY = (-dpiY / 25.4) * scaleFactor;

  qreal factor = QTransform().scale(scaleX, scaleY).mapRect(QRectF(0, 0, 1, 1)).width();

  if (factor < 0.5 || factor > 10)
    return;

  scale(scaleFactor, scaleFactor);
  			
  Scene* sc = dynamic_cast<Scene*>(scene());
  if(sc)
  {
    sc->redrawItems();
  }
}

void te::layout::View::keyPressEvent( QKeyEvent* keyEvent )
{
  Scene* sc = dynamic_cast<Scene*>(scene());
  
  if(keyEvent->key() == Qt::Key_P)
  {
    //Apenas redesenhar itens que estão dentro do box do papel.
    sc->printPreview();
  }
  else if(keyEvent->key() == Qt::Key_E)
  {
    sc->savePaperAsPDF();
  }
  else if(keyEvent->key() == Qt::Key_I)
  {
    sc->savePaperAsImage();
  }
  else if(keyEvent->key() == Qt::Key_G)
  {
    createItemGroup();
  }
  else if(keyEvent->key() == Qt::Key_D)
  {
    destroyItemGroup();
  }
  else if(keyEvent->key() == Qt::Key_Delete)
  {
    deleteItems();
  }

  QGraphicsView::keyPressEvent(keyEvent);
}

void te::layout::View::config()
{	
  connect(scene(), SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
  connect(scene(), SIGNAL(addItemFinalized()), this, SLOT(onAddItemFinalized()));

  Scene* lScene = dynamic_cast<Scene*>(scene());

  if(!lScene)
    return;

  lScene->init(widthMM(), heightMM());

  te::gm::Envelope* boxW = lScene->getWorldBox();

  double llx = boxW->getLowerLeftX();
  double ury = boxW->getUpperRightY();
  		
  //Transform calcula automaticamente a matriz inversa
  setTransform(lScene->getMatrixViewScene());
  			
  setTransformationAnchor(QGraphicsView::NoAnchor);	
  centerOn(QPointF(llx, ury));
  
  /* Mirror the y coordinate, because the scene is in Cartesian coordinates. */
  scale(1, -1);    
  //----------------------------------------------------------------------------------------------
  
  connect(m_outsideArea->getToolbarOutside(), SIGNAL(changeContext(bool)), this, SLOT(onToolbarChangeContext(bool)));

  m_visualizationArea = new VisualizationArea(boxW);
  m_visualizationArea->build();
        
  scene()->setBackgroundBrush(QBrush(QColor(105,105,030)));
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void te::layout::View::resizeEvent(QResizeEvent * event)
{
  QGraphicsView::resizeEvent(event);
}

int te::layout::View::metric( PaintDeviceMetric metric ) const 
{
  if(metric == PdmHeightMM)
  {
    return 297;
  }
	
  return QGraphicsView::metric(metric);
}

void te::layout::View::paintEvent( QPaintEvent * event )
{
  QGraphicsView::paintEvent(event);
}

void te::layout::View::hideEvent( QHideEvent * event )
{
  QGraphicsView::hideEvent(event);

  if(m_outsideArea)
    m_outsideArea->closeAllDocks();
}

void te::layout::View::closeEvent( QCloseEvent * event )
{
  QGraphicsView::closeEvent(event);

  if(m_outsideArea)
    m_outsideArea->closeAllDocks();
}

void te::layout::View::showEvent( QShowEvent * event )
{
  QGraphicsView::showEvent(event);

  if(m_outsideArea)
    m_outsideArea->openAllDocks();
}

void te::layout::View::onToolbarChangeContext( bool change )
{
  Scene* sc = dynamic_cast<Scene*>(scene());

  if(!sc)
    return;

  if(Context::getInstance()->getMode() == TypeUnitsMetricsChange)
  {

  }

  if( Context::getInstance()->getMode() == TypeNewTemplate)
  {
    sc->refresh();
    m_visualizationArea->build();
  }
  if(Context::getInstance()->getMode() == TypeExportPropsJSON)
  {
    sc->exportPropsAsJSON();
  }
  if(Context::getInstance()->getMode() == TypeImportJSONProps)
  {
    buildTemplate();
  }

  if(Context::getInstance()->getMode() == TypePan)
  {
    //Use ScrollHand Drag Mode to enable Panning
    //You do need the enable scroll bars for that to work.
    setDragMode(ScrollHandDrag);
    //Whole view not interactive while in ScrollHandDrag Mode
    setInteractive(false);
    //setCursor(Qt::OpenHandCursor);
  }
  else
  {
    //Use ScrollHand Drag Mode to enable Panning
    //You do need the enable scroll bars for that to work.
    setDragMode(RubberBandDrag);
    //Whole view not interactive while in ScrollHandDrag Mode
    setInteractive(true);
    setCursor(Qt::ArrowCursor);
  }
}

void te::layout::View::onSelectionChanged()
{
  QList<QGraphicsItem*> graphicsItems = scene()->selectedItems();
  QList<QGraphicsItem*> allItems = scene()->items();
  //Refresh Property window   
  if(m_outsideArea->getPropertiesOutside())
    m_outsideArea->getPropertiesOutside()->itemsSelected(graphicsItems, allItems);
}

void te::layout::View::onAddItemFinalized()
{
  QList<QGraphicsItem*> graphicsItems = scene()->selectedItems();
  //Refresh Inspector Object window
  if(m_outsideArea->getObjectInspectorOutside())
    m_outsideArea->getObjectInspectorOutside()->itemsInspector(graphicsItems);
}

void te::layout::View::setOutsideArea( OutsideArea* outsideArea )
{
  m_outsideArea = outsideArea;
}

void te::layout::View::createItem( const te::gm::Coord2D& coord )
{
  if(!m_buildItems)
    return;

  QGraphicsItem* item = 0;

  LayoutMode mode = Context::getInstance()->getMode();

  item = m_buildItems->createItem(mode, coord);

  //If Create item
  if(item)
  {
    Context::getInstance()->setMode(TypeNone);
  }
}

void te::layout::View::createItemGroup()
{
  Scene* sc = dynamic_cast<Scene*>(scene());
  QList<QGraphicsItem*> graphicsItems = this->scene()->selectedItems();

  if(sc)
  {
    QGraphicsItemGroup* group = sc->createItemGroup(graphicsItems);
    group->setParentItem(sc->getMasterParentItem());
      
    ItemGroup* layoutGroup = dynamic_cast<ItemGroup*>(group);
      
    if(layoutGroup) 
      layoutGroup->redraw();

    /*If "enabled=true", QGraphicsItemGroup will handle all the events. For example, 
    the event of mouse click on the child item won't be handled by child item.
    If "enabled=false", QGraphicsItem Group will not block the child item's event 
    and let child item handle it own event.*/
    group->setHandlesChildEvents(true);
  }
}

void te::layout::View::destroyItemGroup()
{
  Scene* sc = dynamic_cast<Scene*>(scene());
  QList<QGraphicsItem*> graphicsItems = this->scene()->selectedItems();

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      QGraphicsItemGroup* group = dynamic_cast<QGraphicsItemGroup*>(item);
      if(group)
      {
        if(sc)
        {
          sc->destroyItemGroup(group);
        }
      }
    }
  }
}

void te::layout::View::deleteItems()
{
  Scene* sc = dynamic_cast<Scene*>(scene());
  QList<QGraphicsItem*> graphicsItems = this->scene()->selectedItems();

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      sc->removeItem(item);
      if(item)
      {
        delete item;
      }
    }
  }
}

void te::layout::View::buildTemplate()
{
  Scene* sc = dynamic_cast<Scene*>(scene());

  if(!sc)
    return;
  
  std::vector<te::layout::Properties*> props = sc->importJsonAsProps();

  if(props.empty())
    return;

  sc->refresh();

  std::vector<te::layout::Properties*>::iterator it;

  te::gm::Envelope* boxW = sc->getWorldBox();
  m_visualizationArea->changeBoxArea(boxW);
  m_visualizationArea->rebuildWithoutPaper();

  for(it = props.begin() ; it != props.end() ; ++it)
  {
    te::layout::Properties* proper = (*it);

    if(!proper)
      continue;

    m_buildItems->rebuildItem(proper);
  }
}
