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
#include "VisualizationArea.h"
#include "OutsideArea.h"
#include "../../../../color/RGBAColor.h"
#include "PropertiesOutside.h"
#include "ObjectInspectorOutside.h"
#include "ToolbarOutside.h"
#include "ItemUtils.h"
#include "PaperConfig.h"
#include "ViewPan.h"
#include "Utils.h"

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
#include <QLineF>

#define _psPointInMM 0.352777778 //<! 1 PostScript point in millimeters
#define _inchInPSPoints 72 //<! 1 Inch in PostScript point
#define _inchInMillimeters 25.4 //<! 1 Inch in millimeters

te::layout::View::View( QWidget* widget) : 
  QGraphicsView(new QGraphicsScene, widget),
  m_outsideArea(0),
  m_visualizationArea(0),
  m_lineIntersectHrz(0),
  m_lineIntersectVrt(0),
  m_currentTool(0)
{
  //Use ScrollHand Drag Mode to enable Panning
  //You do need the enable scroll bars for that to work.
  setDragMode(RubberBandDrag);

  m_lineIntersectHrz = new QLineF(0,0,0,0);
  m_lineIntersectVrt = new QLineF(0,0,0,0);
}

te::layout::View::~View()
{
  if(m_visualizationArea)
  {
    delete m_visualizationArea;
    m_visualizationArea = 0;
  }

  if(m_lineIntersectHrz)
  {
    delete m_lineIntersectHrz;
    m_lineIntersectHrz = 0;
  }

  if(m_lineIntersectVrt)
  {
    delete m_lineIntersectVrt;
    m_lineIntersectVrt = 0;
  }
}

void te::layout::View::mousePressEvent( QMouseEvent * event )
{
  QGraphicsView::mousePressEvent(event);
  
  QPointF scenePos = mapToScene(event->pos());
  te::gm::Coord2D coord(scenePos.x(), scenePos.y());

  Scene* sc = dynamic_cast<Scene*>(scene());
  if(!sc)
    return;

  sc->createItem(coord); 
}

void te::layout::View::mouseMoveEvent( QMouseEvent * event )
{
  QGraphicsView::mouseMoveEvent(event);

  Scene* sc = dynamic_cast<Scene*>(scene());

  if(!sc)
    return;

   QPointF scenePos = mapToScene(event->pos());

   emit changeSceneCoordMouse(scenePos);
   
   m_lineIntersectHrz->setP1(QPointF(sc->sceneRect().topLeft().x(), scenePos.y()));
   m_lineIntersectHrz->setP2(scenePos);

   m_lineIntersectVrt->setP1(QPointF(scenePos.x(), sc->sceneRect().topLeft().y()));
   m_lineIntersectVrt->setP2(scenePos);
}

void te::layout::View::wheelEvent( QWheelEvent *event )
{
  QGraphicsView::wheelEvent(event);
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
    sc->deleteItems();
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

  PaperConfig* pConfig =  Context::getInstance()->getPaperConfig();

  double w = 0;
  double h = 0;

  pConfig->getPaperSize(w, h);

  lScene->init(widthMM(), heightMM(), w, h);

  configTransform(lScene);

  //----------------------------------------------------------------------------------------------

  te::gm::Envelope* boxW = lScene->getWorldBox();
  te::gm::Envelope* box = lScene->getPaperBox();
  pConfig->setPaperBoxW(box);

  connect(m_outsideArea->getToolbarOutside(), SIGNAL(changeContext(bool)), this, SLOT(onToolbarChangeContext(bool)));

  connect(m_outsideArea, SIGNAL(changeMenuContext(bool)), this, SLOT(onMainMenuChangeContext(bool)));

  m_visualizationArea = new VisualizationArea(boxW);
  m_visualizationArea->build();

  lScene->setLineIntersectionHzr(m_lineIntersectHrz);
  lScene->setLineIntersectionVrt(m_lineIntersectVrt);
        
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void te::layout::View::resizeEvent(QResizeEvent * event)
{
  QGraphicsView::resizeEvent(event);
}

int te::layout::View::metric( PaintDeviceMetric metric ) const 
{
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
  {
    m_outsideArea->closeAllDocks();
    m_outsideArea->closeMainMenu();
  }
}

void te::layout::View::closeEvent( QCloseEvent * event )
{
  QGraphicsView::closeEvent(event);

  if(m_outsideArea)
  {
    m_outsideArea->closeAllDocks();
    m_outsideArea->closeMainMenu();
  }
}

void te::layout::View::showEvent( QShowEvent * event )
{
  QGraphicsView::showEvent(event);

  if(m_outsideArea)
  {
    m_outsideArea->openAllDocks();
    m_outsideArea->openMainMenu();
  }
}

void te::layout::View::onToolbarChangeContext( bool change )
{
  outsideAreaChangeContext(change);
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

void te::layout::View::createItemGroup()
{
  Scene* sc = dynamic_cast<Scene*>(scene());
  QList<QGraphicsItem*> graphicsItems = this->scene()->selectedItems();

  if(sc)
  {
    QGraphicsItemGroup* group = sc->createItemGroup(graphicsItems);

    if(!group)
      return;
          
    ItemGroup* layoutGroup = dynamic_cast<ItemGroup*>(group);
      
    if(!layoutGroup)
      return;

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

void te::layout::View::resetDefaultConfig()
{
  //Use ScrollHand Drag Mode to enable Panning
  //You do need the enable scroll bars for that to work.
  setDragMode(RubberBandDrag);
  //Whole view not interactive while in ScrollHandDrag Mode
  setInteractive(true);
  setCursor(Qt::ArrowCursor);
  if(m_currentTool)
  {
    viewport()->removeEventFilter(m_currentTool);
    delete m_currentTool;
    m_currentTool = 0;
  }
}

void te::layout::View::onMainMenuChangeContext( bool change )
{
  outsideAreaChangeContext(change);
}

void te::layout::View::outsideAreaChangeContext( bool change )
{
  Scene* sc = dynamic_cast<Scene*>(scene());

  if(!sc)
    return;

  LayoutMode mode = Context::getInstance()->getMode();
  QList<QGraphicsItem*> graphicsItems;
  double zoomFactor = Context::getInstance()->getZoomFactor();

  switch(mode)
  {
  case TypeUnitsMetricsChange:
    {
      break;
    }
  case TypeNewTemplate:
    {
      sc->reset();
      m_visualizationArea->build();
      resetDefaultConfig();
      break;
    }
  case TypeExportPropsJSON:
    {
      sc->exportPropsAsJSON();
      resetDefaultConfig();
      break;
    }
  case TypeImportJSONProps:
    {
      sc->buildTemplate(m_visualizationArea);
      resetDefaultConfig();
      break;
    }
  case TypeMapPan:
    {
      graphicsItems = sc->selectedItems();
      if(te::layout::getMapItemList(graphicsItems).empty())
      {
        resetDefaultConfig();
      }
      else
      {
        sc->setCurrentToolInSelectedMapItems(TypeMapPan);
      }
      break;
    }
  case TypeMapZoomIn:
    {
      graphicsItems = sc->selectedItems();
      if(te::layout::getMapItemList(graphicsItems).empty())
      {
        resetDefaultConfig();
      }
      else
      {
        sc->setCurrentToolInSelectedMapItems(TypeMapZoomIn);
      }
      break;
    }
  case TypeMapZoomOut:
    {
      graphicsItems = sc->selectedItems();
      if(te::layout::getMapItemList(graphicsItems).empty())
      {
        resetDefaultConfig();
      }
      else
      {
        sc->setCurrentToolInSelectedMapItems(TypeMapZoomOut);
      }
      break;
    }
  case TypePan:
    {
      /*
        The QGraphicsView inherits QAbstractScrollArea. 
        The QAbstractScrollArea on your EventFilter event invokes the viewportEvent instead eventFilter of the son, 
        so it is necessary to add QAbstractScrollArea for the filter installed can listen to events.       
      */
      setInteractive(false);
      m_currentTool = new ViewPan(this, Qt::OpenHandCursor, Qt::ClosedHandCursor);
      viewport()->installEventFilter(m_currentTool);
      break;
    }
  case TypeGroup:
    {
      createItemGroup();
      resetDefaultConfig();
    }
    break;
  case TypeUngroup:
    {
      destroyItemGroup();
      resetDefaultConfig();
    }
    break;
  case TypePrinter:
    {
      sc->printPreview();
    }
    break;
  case TypeExit:
    {
      //hide();
    }
    break;
  case TypeSceneZoom:
    {
      sc->refresh(this);
      sc->redrawItems(true);
      resetDefaultConfig();
    }
    break;
  case TypeBringToFront:
    {
      sc->bringToFront();
      resetDefaultConfig();
    }
    break;
  case TypeSendToBack:
    {
      sc->sendToBack();
      resetDefaultConfig();
    }
    break;
  default:
    {
      resetDefaultConfig();
    }
  }
}

void te::layout::View::configTransform( Scene* sc )
{
  te::gm::Envelope* boxW = sc->getWorldBox();

  double llx = boxW->getLowerLeftX();
  double ury = boxW->getUpperRightY();

  //Transform calcula automaticamente a matriz inversa
  setTransform(sc->getMatrixViewScene());

  setTransformationAnchor(QGraphicsView::NoAnchor);	
  centerOn(QPointF(llx, ury));

  /* Mirror the y coordinate, because the scene is in Cartesian coordinates. */
  scale(1, -1);   
}
