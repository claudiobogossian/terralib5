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
#include "../../core/pattern/singleton/Context.h"
#include "../../core/enum/EnumMode.h"
#include "../item/ItemGroup.h"
#include "VisualizationArea.h"
#include "../../../color/RGBAColor.h"
#include "../outside/PropertiesOutside.h"
#include "../outside/ObjectInspectorOutside.h"
#include "../outside/ToolbarOutside.h"
#include "ItemUtils.h"
#include "tools/ViewPan.h"
#include "../../core/Utils.h"
#include "tools/ViewZoomArea.h"
#include "../../outside/PageSetupModel.h"
#include "../../outside/PageSetupController.h"
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../outside/SystematicScaleModel.h"
#include "../../outside/SystematicScaleController.h"
#include "../../core/SystematicScaleConfig.h"

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
#include <QMessageBox>

#define _psPointInMM 0.352777778 //<! 1 PostScript point in millimeters
#define _inchInPSPoints 72 //<! 1 Inch in PostScript point
#define _inchInMillimeters 25.4 //<! 1 Inch in millimeters

te::layout::View::View( QWidget* widget) : 
  QGraphicsView(new QGraphicsScene, widget),
  m_visualizationArea(0),
  m_lineIntersectHrz(0),
  m_lineIntersectVrt(0),
  m_currentTool(0),
  m_pageSetupOutside(0),
  m_systematicOutside(0)
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

  if(m_pageSetupOutside)
  {
    delete m_pageSetupOutside;
    m_pageSetupOutside = 0;
  }

  if(m_systematicOutside)
  {
    delete m_systematicOutside;
    m_systematicOutside = 0;
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

  if(Context::getInstance().getMode() == TypeSystematicScale)
  {
    bool intersection = false;
    int number = sc->intersectionMap(coord, intersection);
    
    QMessageBox msgBox;

    if(number == 1)
    {
      if(intersection)
      {
        m_coordSystematic = coord;
        showSystematicScale();
      }
    }
    else if(number > 1)
    {
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setText("Select just one object.");
      msgBox.exec();
    }
    else 
    {
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setText("Select a Map Object.");
      msgBox.exec();
    }
    Context::getInstance().setMode(TypeNone);
  }
  else
  {
    sc->createItem(coord);
  } 
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
  Scene* lScene = dynamic_cast<Scene*>(scene());

  if(!lScene)
    return;

  PaperConfig* pConfig =  Context::getInstance().getPaperConfig();

  double w = 0;
  double h = 0;

  pConfig->getPaperSize(w, h);

  double sw = widthMM();
  double sh = heightMM();
    
  double zoomFactor = Context::getInstance().getZoomFactor();
  lScene->init(sw, sh, w, h, zoomFactor);

  configTransform(lScene);

  //----------------------------------------------------------------------------------------------

  te::gm::Envelope* boxW = lScene->getWorldBox();
  te::gm::Envelope* box = lScene->getPaperBox();
  pConfig->setPaperBoxW(box);
  
  if(!m_visualizationArea)
  {
    m_visualizationArea = new VisualizationArea(boxW);
    m_visualizationArea->build();
  }

  lScene->setLineIntersectionHzr(m_lineIntersectHrz);
  lScene->setLineIntersectionVrt(m_lineIntersectVrt);
        
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void te::layout::View::resizeEvent(QResizeEvent * event)
{
  QGraphicsView::resizeEvent(event);
  Scene* lScene = dynamic_cast<Scene*>(scene());

  if(lScene)
  {
    double zoomFactor = Context::getInstance().getZoomFactor();

    lScene->refresh(this, zoomFactor);
    lScene->redrawItems(true);
  }
}

void te::layout::View::onToolbarChangeContext( bool change )
{
  outsideAreaChangeContext(change);
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

  LayoutMode mode = Context::getInstance().getMode();
  QList<QGraphicsItem*> graphicsItems;
  double zoomFactor = Context::getInstance().getZoomFactor();
  double oldZoomFactor = Context::getInstance().getOldZoomFactor();

  te::gm::Envelope* env = sc->getWorldBox();

  double newZoomFactor = 1. / zoomFactor;
  if(zoomFactor < 1.)
    newZoomFactor = zoomFactor;

  double halfWidth = env->getWidth() * newZoomFactor  / 2.;
  double halfHeight = env->getHeight() * newZoomFactor / 2.;
  te::gm::Coord2D center = env->getCenter();

  te::gm::Envelope zoomBox;
  
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
      env->m_llx = center.x - halfWidth;
      env->m_lly = center.y - halfHeight;
      env->m_urx = center.x + halfWidth;
      env->m_ury = center.y + halfHeight;
                 
      sc->refresh(this, zoomFactor);            
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
  case TypeZoomIn:
    {
      resetDefaultConfig();
      /*
        The QGraphicsView inherits QAbstractScrollArea. 
        The QAbstractScrollArea on your EventFilter event invokes the viewportEvent instead eventFilter of the son, 
        so it is necessary to add QAbstractScrollArea for the filter installed can listen to events.       
      */
      setInteractive(false);
      m_currentTool = new ViewZoomArea(this, Qt::CrossCursor);
      viewport()->installEventFilter(m_currentTool);
      break;
    }
    break;
  case TypeZoomOut:
    {

    }
    break;
  case TypeRecompose:
    {
      double dZoom = Context::getInstance().getDefaultZoomFactor();
      double zoom = Context::getInstance().getZoomFactor();
      if(dZoom != zoom)
      {
        sc->refresh(this, dZoom);
        sc->redrawItems(true);
      }
      resetDefaultConfig();
    }
    break;
  case TypePageConfig:
    showPageSetup();
    resetDefaultConfig();
    break;
  case TypeMapCreateTextGrid:
    sc->createTextGridAsObject();
    resetDefaultConfig();
    break;
  case TypeMapCreateTextMap:
    sc->createTextMapAsObject();
    resetDefaultConfig();
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

void te::layout::View::hideEvent( QHideEvent * event )
{
  QGraphicsView::hideEvent(event);
  emit hideView();
}

void te::layout::View::closeEvent( QCloseEvent * event )
{
  QGraphicsView::closeEvent(event);
  emit closeView();
}

void te::layout::View::showEvent( QShowEvent * event )
{
  QGraphicsView::showEvent(event);
  emit showView();
}

void te::layout::View::showPageSetup()
{
  if(!m_pageSetupOutside)
  {
    PageSetupModel* model = new PageSetupModel;
    PageSetupController* controller = new PageSetupController(model);
    OutsideObserver* obsever = (OutsideObserver*)(controller->getView());
    m_pageSetupOutside = dynamic_cast<PageSetupOutside*>(obsever);
    connect(m_pageSetupOutside, SIGNAL(changeConfig()), this, SLOT(onChangeConfig()));
  }

  m_pageSetupOutside->load();
  m_pageSetupOutside->show();
}

void te::layout::View::onChangeConfig()
{
  double zoomFactor = Context::getInstance().getDefaultZoomFactor();
  Context::getInstance().setZoomFactor(zoomFactor);
  config();

  Scene* sc = dynamic_cast<Scene*>(scene());

  sc->refresh(this, zoomFactor);            
  sc->redrawItems(true);
}

void te::layout::View::showSystematicScale()
{
  if(!m_systematicOutside)
  {
    SystematicScaleModel* model = new SystematicScaleModel;
    SystematicScaleController* controller = new SystematicScaleController(model);
    OutsideObserver* obsever = (OutsideObserver*)(controller->getView());
    m_systematicOutside = dynamic_cast<SystematicScaleOutside*>(obsever);
    connect(m_systematicOutside, SIGNAL(systematicApply(double,SystematicScaleType)), this, SLOT(onSystematicApply(double,SystematicScaleType)));
  }

  m_systematicOutside->show();
}

void te::layout::View::closeOutsideWindows()
{
  if(m_pageSetupOutside)
  {
    m_pageSetupOutside->close();
  }
}

void te::layout::View::onSystematicApply(double scale, SystematicScaleType type)
{
  SystematicScaleConfig* config = Context::getInstance().getSystematicScaleConfig();
  Systematic* sys = config->getSystematic(type);
  
  Scene* sc = dynamic_cast<Scene*>(scene());
  if(sc)
  {
    sc->setCurrentMapSystematic(sys, m_coordSystematic);
  }
}
