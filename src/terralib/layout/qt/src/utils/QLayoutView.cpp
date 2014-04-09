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
  \file QLayoutView.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "QLayoutView.h"
#include "LayoutContext.h"
#include "QRectangleLayoutItem.h"
#include "LayoutMode.h"
#include "HorizontalRulerLayoutModel.h"
#include "HorizontalRulerLayoutController.h"
#include "QHorizontalRulerLayoutItem.h"
#include "RectangleLayoutModel.h"
#include "RectangleLayoutController.h"
#include "VerticalRulerLayoutModel.h"
#include "VerticalRulerLayoutController.h"
#include "QVerticalRulerLayoutItem.h"
#include "QLayoutItemGroup.h"
#include "QPropertiesWindowOutside.h"
#include "PropertiesWindowLayoutModel.h"
#include "PropertiesWindowLayoutController.h"
#include "LayoutItemObserver.h"
#include "LayoutOutsideObserver.h"
#include "QLayoutItemFactory.h"
#include "QLayoutOutsideFactory.h"
#include "ObjectInspectorWindowLayoutModel.h"
#include "ObjectInspectorWindowLayoutController.h"
#include "QObjectInspectorWindowOutside.h"
#include "ToolbarWindowLayoutModel.h"
#include "ToolbarWindowLayoutController.h"
#include "QToolbarWindowOutside.h"
#include "PaperLayoutModel.h"
#include "PaperLayoutController.h"
#include "QPaperLayoutItem.h"
#include "MapLayoutModel.h"
#include "MapLayoutController.h"
#include "QMapLayoutItem.h"
#include "LayoutOutsideController.h"
#include "../../../../color/RGBAColor.h"
#include "../../../../geometry/Envelope.h"

// Qt
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QString>
#include <QGraphicsView>
#include <QtGui>
#include <QGraphicsItem>
#include "QLayoutScene.h"
#include "LayoutView.h"
#include <time.h>
#include <QMatrix>
#include <math.h> 
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>

#define _psPointInMM 0.352777778 //<! 1 PostScript point in millimeters
#define _inchInPSPoints 72 //<! 1 Inch in PostScript point
#define _inchInMillimeters 25.4 //<! 1 Inch in millimeters

te::layout::QLayoutView::QLayoutView( QWidget* widget) : 
  QGraphicsView(new QGraphicsScene, widget),
  m_diagonalScreenInchSize(0),
  m_dockProperties(0),
  m_dockInspector(0),
  m_dockParent(0),
  m_dockToolbar(0),
  m_dpiX(96),
  m_dpiY(96)
{
  //Use ScrollHand Drag Mode to enable Panning
  //You do need the enable scroll bars for that to work.
  setDragMode(RubberBandDrag);
}

te::layout::QLayoutView::~QLayoutView()
{
  if(scene())
    scene()->clear();

   QMainWindow* win = (QMainWindow*)m_dockParent;
  
  if(m_dockProperties)
  {
    if(win)
    {
      win->removeDockWidget(m_dockProperties);
    }

    m_dockProperties->close();
    m_dockProperties->setParent(0);
    delete m_dockProperties;
  }
  
  if(m_dockInspector)
  {
    if(win)
    {
      win->removeDockWidget(m_dockInspector);
    }

    m_dockInspector->close();
    m_dockInspector->setParent(0);
    delete m_dockInspector;
  }

  if(m_dockToolbar)
  {
    if(win)
    {
      win->removeDockWidget(m_dockToolbar);
    }

    m_dockToolbar->close();
    m_dockToolbar->setParent(0);
    delete m_dockToolbar;
  }

  if(LayoutContext::getInstance())
  {
    LayoutContext::getInstance()->setScene(0);
    if(LayoutContext::getInstance()->getCanvas())
    {
      delete LayoutContext::getInstance()->getCanvas();
      LayoutContext::getInstance()->setCanvas(0);
    }

    if(LayoutContext::getInstance()->getItemFactory())
    {
      delete (QLayoutItemFactory*)LayoutContext::getInstance()->getItemFactory();
      LayoutContext::getInstance()->setItemFactory(0);
    }

    if(LayoutContext::getInstance()->getOutsideFactory())
    {
      delete (QLayoutOutsideFactory*)LayoutContext::getInstance()->getOutsideFactory();
      LayoutContext::getInstance()->setOutsideFactory(0);
    }

    if(LayoutContext::getInstance()->getUtils())
    {
      delete LayoutContext::getInstance()->getUtils();
      LayoutContext::getInstance()->setUtils(0);
    }
  }
}

void te::layout::QLayoutView::mousePressEvent( QMouseEvent * event )
{
  QGraphicsView::mousePressEvent(event);
  
  if(LayoutContext::getInstance()->getMode() == TypeCreateMap)
  {
    QPointF scenePos = mapToScene(event->pos());
    createMap(scenePos);
  } 
  else if(LayoutContext::getInstance()->getMode() == TypeCreateMap)
  {
    //createText();
  } else  if(LayoutContext::getInstance()->getMode() == TypeCreateRectangle)
  {
    QPointF scenePos = mapToScene(event->pos());
    createRectangle(scenePos);
  } 
}

void te::layout::QLayoutView::wheelEvent( QWheelEvent *event )
{
  scaleView(pow((double)2, -event->delta() / 240.0));
}

void te::layout::QLayoutView::scaleView( qreal scaleFactor )
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

  LayoutContext::getInstance()->setZoomFactor(factor);
			
  QList<QGraphicsItem*> graphicsItems = this->scene()->items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {			
      QGraphicsWidget* outside = dynamic_cast<QGraphicsWidget*>(item);

      if(outside)
        continue;

      LayoutItemObserver* lItem = dynamic_cast<LayoutItemObserver*>(item);
      if(lItem)
        lItem->redraw(factor);
    }
  }
}

void te::layout::QLayoutView::keyPressEvent( QKeyEvent* keyEvent )
{
  QLayoutScene* sc = dynamic_cast<QLayoutScene*>(scene());

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
    if(sc)
    {
      QGraphicsItemGroup* group = sc->createItemGroup(scene()->selectedItems());
      QLayoutScene* lScene = dynamic_cast<QLayoutScene*>(scene());
      group->setParentItem(lScene->getMasterParentItem());
      
      QLayoutItemGroup* layoutGroup = dynamic_cast<QLayoutItemGroup*>(group);
      
      if(layoutGroup)
        layoutGroup->redraw();

      /*If "enabled=true", QGraphicsItemGroup will handle all the events. For example, 
      the event of mouse click on the child item won't be handled by child item.
      If "enabled=false", QGraphicsItem Group will not block the child item's event 
      and let child item handle it own event.*/
      group->setHandlesChildEvents(true);
    }
  }
  else if(keyEvent->key() == Qt::Key_D)
  {
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
  else if(keyEvent->key() == Qt::Key_Delete)
  {
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

  QGraphicsView::keyPressEvent(keyEvent);
}

void te::layout::QLayoutView::config()
{	
  LayoutContext::getInstance()->setDpiX(95.78);
  LayoutContext::getInstance()->setDpiY(95.78);
  m_dpiX = 95.78;
  m_dpiY = 95.78;

  connect(scene(), SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
  connect(scene(), SIGNAL(addItemFinalized()), this, SLOT(onAddItemFinalized()));

  QLayoutScene* lScene = dynamic_cast<QLayoutScene*>(scene());

  if(!lScene)
    return;

  lScene->init(widthMM(), heightMM(), logicalDpiX(), logicalDpiY());

  te::gm::Envelope boxW = lScene->getWorldBox();

  double llx = boxW.getLowerLeftX();
  double lly = boxW.getLowerLeftY();
  double urx = boxW.getUpperRightX();
  double ury = boxW.getUpperRightY();
  		
  //Transform calcula automaticamente a matriz inversa
  setTransform(lScene->getMatrixViewScene());
  			
  setTransformationAnchor(QGraphicsView::NoAnchor);	
  centerOn(QPointF(llx, ury));

  if(m_dockParent)
  {
    //Use the Property Browser Framework for create Property Window
    PropertiesWindowLayoutModel* dockPropertyModel = new PropertiesWindowLayoutModel();		 
    PropertiesWindowLayoutController* dockPropertyController = new PropertiesWindowLayoutController(dockPropertyModel);
    LayoutOutsideObserver* itemDockProperty = (LayoutOutsideObserver*)dockPropertyController->getView();
    m_dockProperties = dynamic_cast<QPropertiesWindowOutside*>(itemDockProperty);

    m_dockProperties->setParent(m_dockParent); //The father need be the window of application, in this case, terraview main window!  
    m_dockProperties->setVisible(true);
    ((QMainWindow*)m_dockParent)->addDockWidget(Qt::LeftDockWidgetArea, m_dockProperties);

    //Use the Property Browser Framework for create Object Inspector Window
    ObjectInspectorWindowLayoutModel* dockInspectorModel = new ObjectInspectorWindowLayoutModel();		 
    ObjectInspectorWindowLayoutController* dockInspectorController = new ObjectInspectorWindowLayoutController(dockInspectorModel);
    LayoutOutsideObserver* itemDockInspector = (LayoutOutsideObserver*)dockInspectorController->getView();
    m_dockInspector = dynamic_cast<QObjectInspectorWindowOutside*>(itemDockInspector);
    m_dockInspector->setParent(m_dockParent); //The father need be the window of application, in this case, terraview main window!
    m_dockInspector->setVisible(true);
    ((QMainWindow*)m_dockParent)->addDockWidget(Qt::LeftDockWidgetArea, m_dockInspector);

    //Use the Property Browser Framework for create Object Inspector Window
    ToolbarWindowLayoutModel* dockToolbarModel = new ToolbarWindowLayoutModel();		 
    ToolbarWindowLayoutController* dockToolbarController = new ToolbarWindowLayoutController(dockToolbarModel);
    LayoutOutsideObserver* itemDockToolbar = (LayoutOutsideObserver*)dockToolbarController->getView();
    m_dockToolbar = dynamic_cast<QToolbarWindowOutside*>(itemDockToolbar); 
    m_dockToolbar->setParent(m_dockParent); //The father need be the window of application, in this case, terraview main window!
    m_dockToolbar->setVisible(true);
    ((QMainWindow*)m_dockParent)->addDockWidget(Qt::TopDockWidgetArea, m_dockToolbar);
    connect(m_dockToolbar, SIGNAL(changeContext(bool)), this, SLOT(onToolbarChangeContext(bool)));
  }

  //----------------------------------------------------------------------------------------------
  
  //Paper
  //Retângulo: utilizando o canvas da Terralib 5
  PaperLayoutModel* modelPaper = new PaperLayoutModel();	
  PaperLayoutController* controllerPaper = new PaperLayoutController(modelPaper);
  LayoutItemObserver* itemPaper = (LayoutItemObserver*)controllerPaper->getView();
  QPaperLayoutItem* qPaper = dynamic_cast<QPaperLayoutItem*>(itemPaper);
  itemPaper->setItemPosition(0, 0);
  itemPaper->redraw();
	
  //Régua: utilizando o canvas da Terralib 5
  HorizontalRulerLayoutModel* modelRuler = new HorizontalRulerLayoutModel();		
  modelRuler->setBox(te::gm::Envelope(llx, ury - 10, urx + 10, ury));
  HorizontalRulerLayoutController* controllerRuler = new HorizontalRulerLayoutController(modelRuler);
  LayoutItemObserver* itemRuler = (LayoutItemObserver*)controllerRuler->getView();
  QHorizontalRulerLayoutItem* rectRuler = dynamic_cast<QHorizontalRulerLayoutItem*>(itemRuler);
  rectRuler->setPPI(logicalDpiX());
  rectRuler->setItemPosition(llx, lly);
  rectRuler->redraw();

  VerticalRulerLayoutModel* modelRulerV = new VerticalRulerLayoutModel();		
  modelRulerV->setBox(te::gm::Envelope(llx, lly, llx + 10, ury + 10));
  VerticalRulerLayoutController* controllerRulerV = new VerticalRulerLayoutController(modelRulerV);
  LayoutItemObserver* itemRulerV = (LayoutItemObserver*)controllerRulerV->getView();
  QVerticalRulerLayoutItem* rectRulerV = dynamic_cast<QVerticalRulerLayoutItem*>(itemRulerV);
  rectRulerV->setPPI(logicalDpiX());		
  rectRulerV->setItemPosition(llx, lly + 10);
  rectRulerV->redraw();
    
  //Retângulo: utilizando o canvas da Terralib 5
  RectangleLayoutModel* model = new RectangleLayoutModel();	
  model->setName("RECT_01");
  RectangleLayoutController* controller = new RectangleLayoutController(model);
  LayoutItemObserver* item = (LayoutItemObserver*)controller->getView();
  QRectangleLayoutItem* rect = dynamic_cast<QRectangleLayoutItem*>(item);
  item->setItemPosition(llx, lly);
  item->redraw();
	
  //Retângulo: utilizando o canvas da Terralib 5
  RectangleLayoutModel* model2 = new RectangleLayoutModel();		  
  model2->setName("RECT_02");
  RectangleLayoutController* controller2 = new RectangleLayoutController(model2);
  LayoutItemObserver* item2 = (LayoutItemObserver*)controller2->getView();
  QRectangleLayoutItem* rect2 = dynamic_cast<QRectangleLayoutItem*>(item2);
  item2->setItemPosition(llx + 40, lly + 40);
  item2->redraw();

  //-------------------------TESTE MAP---------------------------------------------
  //Retângulo: utilizando o canvas da Terralib 5
  MapLayoutModel* modelMap = new MapLayoutModel();		  
  modelMap->setName("MAPA_01");
  MapLayoutController* controllerMap = new MapLayoutController(modelMap);
  LayoutItemObserver* itemMap = (LayoutItemObserver*)controllerMap->getView();
  QMapLayoutItem* qrectMap = dynamic_cast<QMapLayoutItem*>(itemMap);
  itemMap->setItemPosition(llx + 60, lly + 60);
  itemMap->redraw();

  ////-----------------------------------------------------------------------------------------------------
	
  QFont font;
  font.setFamily("Arial"); 
  font.setPointSize(12);

  QFontMetrics fm(font, this);
  int fontpx = fm.height();

  font.setPixelSize(fontpx);

  int mm = (12 * _psPointInMM) ;

  //Inverte a matriz do item, porém o posicionamento continua pela coordenada de cena,
  //então -----> o Y deve ser invertido. ****-dpiY / 25.4
  //Sempre começar o desenho pelo ponto 0,0, caso contrário o ponto central(center) fica estranho?!
  QGraphicsTextItem* rectText40 = scene()->addText("ÕÇ_Atexto50,0 ", font);
  //rectText40->setParentItem(rectItemBack1);
  rectText40->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges);
  rectText40->setTransform(lScene->getMatrixViewScene().inverted());
  rectText40->setPos(30, -20); //Coordenada de cena em mm		
        
  scene()->setBackgroundBrush(QBrush(QColor(105,105,030)));
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void te::layout::QLayoutView::resizeEvent(QResizeEvent * event)
{
  QGraphicsView::resizeEvent(event);
}

int te::layout::QLayoutView::metric( PaintDeviceMetric metric ) const 
{
  switch(metric)
  {
    /*case PdmWidthMM:
    {
    return 509;
    }*/
    case PdmHeightMM:
    {
      //return 286;
      return 297;
    }
    //case PdmPhysicalDpiX:
    //{
    //  return m_dpiX;
    //  //Caso esteja imprimindo, obter o dpi da impressora
    //  //.
    //  //.
    //  //.
    //}
    //case PdmPhysicalDpiY:
    //{
    //  return m_dpiY;
    //  //Caso esteja imprimindo, obter o dpi da impressora
    //  //.
    //  //.
    //  //.
    //}
    //case PdmDpiX:
    //{
    //  return m_dpiX;
    //  //Caso esteja imprimindo, obter o dpi da impressora
    //  //.
    //  //.
    //  //.
    //}
    //case PdmDpiY:
    //{
    //  return m_dpiY;
    //  //Caso esteja imprimindo, obter o dpi da impressora
    //  //.
    //  //.
    //  //.
    //}
  }
	
  return QGraphicsView::metric(metric);
}

void te::layout::QLayoutView::paintEvent( QPaintEvent * event )
{
  QGraphicsView::paintEvent(event);
}

void te::layout::QLayoutView::setDockPropertiesParent( QWidget* dockParent )
{
  m_dockParent = dockParent;
}

void te::layout::QLayoutView::hideEvent( QHideEvent * event )
{
  QGraphicsView::hideEvent(event);
}

void te::layout::QLayoutView::closeEvent( QCloseEvent * event )
{
  QGraphicsView::closeEvent(event);

  QMainWindow* win = (QMainWindow*)m_dockParent;

  if(win)
  {
    if(m_dockProperties)
    {
      win->removeDockWidget(m_dockProperties);
      m_dockProperties->close();
      m_dockProperties->setParent(0);
    }
    if(m_dockInspector)
    {
      win->removeDockWidget(m_dockInspector);
      m_dockInspector->close();
      m_dockInspector->setParent(0);
    }
    if(m_dockToolbar)
    {
      win->removeDockWidget(m_dockToolbar);
      m_dockToolbar->close();
      m_dockToolbar->setParent(0);
    }
  }
}

bool te::layout::QLayoutView::createMap( QPointF point )
{
  bool result = true;
  double x = point.x();
  double y = point.y();

  MapLayoutModel* modelMap = new MapLayoutModel();		  
  modelMap->setName("MAPA_01");
  MapLayoutController* controllerMap = new MapLayoutController(modelMap);
  LayoutItemObserver* itemMap = (LayoutItemObserver*)controllerMap->getView();
  QMapLayoutItem* qrectMap = dynamic_cast<QMapLayoutItem*>(itemMap);
  itemMap->setItemPosition(x, y);
  itemMap->redraw();

  if(itemMap)
    result = false;

  return result;
}

bool te::layout::QLayoutView::createText( QPointF point )
{
  bool result = true;

  double x = point.x();
  double y = point.y();

  QFont font;
  font.setFamily("Arial"); 
  font.setPointSize(12);

  QFontMetrics fm(font, this);
  int fontpx = fm.height();

  font.setPixelSize(fontpx);

  QLayoutScene* lScene = dynamic_cast<QLayoutScene*>(scene());
  QGraphicsTextItem* objText = scene()->addText("LAYOUT", font);
  objText->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges);
  objText->setTransform(lScene->getMatrixViewScene().inverted());
  objText->setPos(x, y); //Coordenada de cena em mm		

  if(!objText)
    result = false;

  return result;
}

bool te::layout::QLayoutView::createRectangle( QPointF point )
{
  bool result = true;

  double x = point.x();
  double y = point.y();

  //Retângulo: utilizando o canvas da Terralib 5
  RectangleLayoutModel* model = new RectangleLayoutModel();	
  model->setName("AZUL");
  RectangleLayoutController* controller = new RectangleLayoutController(model);
  LayoutItemObserver* item = (LayoutItemObserver*)controller->getView();
  QRectangleLayoutItem* rect = dynamic_cast<QRectangleLayoutItem*>(item);
  item->setItemPosition(x, y);
  item->redraw();

  if(!item)
    result = false;

  return result;
}

void te::layout::QLayoutView::onToolbarChangeContext( bool change )
{
  QLayoutScene* sc = dynamic_cast<QLayoutScene*>(scene());

  if(LayoutContext::getInstance()->getMode() == TypeUnitsMetricsChange)
  {

  }

  if(LayoutContext::getInstance()->getMode() == TypeSavePropsJSON)
  {
    sc->savePropsAsJSON();
  }

  if(LayoutContext::getInstance()->getMode() == TypePan)
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

void te::layout::QLayoutView::onSelectionChanged()
{
  //Refresh Property window 
  if(!scene()->selectedItems().isEmpty())
  {
    if(m_dockProperties)
      m_dockProperties->itemsSelected(scene()->selectedItems());
  }
}

void te::layout::QLayoutView::onAddItemFinalized()
{
  //Refresh Inspector Object window
  if(!scene()->items().isEmpty())
  {
    if(m_dockInspector)
      m_dockInspector->itemsInspector(scene()->items());
  }
}
