#include <QLayoutView.h>

#include <QGraphicsView>
#include <QtGui>
#include <QGraphicsItem>
#include "LayoutContext.h"
#include "QRectangleLayoutItem.h"
#include "LayoutMode.h"
#include "QLayoutScene.h"
#include "LayoutView.h"
#include "LayoutController.h"
#include <time.h>
#include <QMatrix>
#include <math.h> 
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include "LayoutViewController.h"
#include "HorizontalRulerLayoutModel.h"
#include "HorizontalRulerLayoutController.h"
#include "QHorizontalRulerLayoutItem.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QString>
#include "RectangleLayoutModel.h"
#include "RectangleLayoutController.h"

#include "../../../../geometry/Envelope.h"
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

#define _psPointInMM 0.352777778 //<! 1 PostScript point in millimeters
#define _inchInPSPoints 72 //<! 1 Inch in PostScript point
#define _inchInMillimeters 25.4 //<! 1 Inch in millimeters

te::layout::QLayoutView::QLayoutView( QWidget* widget) : 
  QGraphicsView(new QGraphicsScene, widget),
  _diagonalScreenInchSize(0),
  _dockProperties(0),
  _dockInspector(0),
  _dockParent(0)
{
  _layoutController = 0;

  //Use ScrollHand Drag Mode to enable Panning
  //You do need the enable scroll bars for that to work.
  setDragMode(RubberBandDrag);

  /*setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);*/
}

te::layout::QLayoutView::~QLayoutView()
{
  if(scene())
    scene()->clear();
  
  if(_dockProperties)
  {
    QMainWindow* win = (QMainWindow*)_dockParent;

    if(win)
    {
      win->removeDockWidget(_dockProperties);
    }

    _dockProperties->close();
    _dockProperties->setParent(0);
    delete _dockProperties;
  }
  
  if(_dockInspector)
  {
    QMainWindow* win = (QMainWindow*)_dockParent;

    if(win)
    {
      win->removeDockWidget(_dockInspector);
    }

    _dockInspector->close();
    _dockInspector->setParent(0);
    delete _dockInspector;
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

  //Refresh Property window
  if(!scene()->selectedItems().isEmpty())
  {
    if(_dockProperties)
      _dockProperties->itemsSelected(scene()->selectedItems());

    if(_dockInspector)
      _dockInspector->itemsInspector(scene()->selectedItems());
  }

 /* if(LayoutContext::getInstance()->getMode() == TypeCreate)
  {
    _layoutController->createNewItem(event->x(), event->y());
  }*/
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

void te::layout::QLayoutView::setLayoutController( LayoutController* controller )
{
  _layoutController = controller;
}

void te::layout::QLayoutView::keyPressEvent( QKeyEvent* keyEvent )
{
  QLayoutScene* sc = dynamic_cast<QLayoutScene*>(scene());

  if(keyEvent->key() == Qt::Key_P)
  {
    scaleView(1.);

    QPrinter* printer=new QPrinter(QPrinter::HighResolution);
    printer->setPageSize(QPrinter::A4);
    printer->setOrientation( QPrinter::Portrait );
    printer->pageRect(QPrinter::Millimeter);
    if (QPrintDialog(printer).exec() == QDialog::Accepted) {
      QPainter painter(printer);
      painter.setRenderHint(QPainter::Antialiasing);
      painter.scale(1., 1.);
      //render(&painter);

      QLayoutScene* sc = dynamic_cast<QLayoutScene*>(scene());
      te::gm::Envelope paperBox =  sc->getPaperBox();

      QPoint lefttop = mapFromScene(paperBox.getLowerLeftX(), paperBox.getUpperRightY());
      QPoint rightbottom = mapFromScene(paperBox.getUpperRightX(), paperBox.getLowerLeftY());

      QRectF sourceTargetRect(paperBox.getLowerLeftX(), paperBox.getLowerLeftY(), 
        paperBox.getWidth(), paperBox.getHeight());
      QRect sourceRect(paperBox.getLowerLeftX(), paperBox.getUpperRightY(), 
        paperBox.getWidth(), paperBox.getHeight());
      scene()->render(&painter, sourceTargetRect, sourceTargetRect); // upside down
    }
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

  QGraphicsView::keyPressEvent(keyEvent);
}

void te::layout::QLayoutView::config()
{	

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
  QPointF center0 = mapToScene(0, 0);
  centerOn(center0);
			
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
  modelRuler->setBox(te::gm::Envelope(llx, lly, urx + 10, lly + 10));
  modelRuler->invertedLines(true);
  HorizontalRulerLayoutController* controllerRuler = new HorizontalRulerLayoutController(modelRuler);
  LayoutItemObserver* itemRuler = (LayoutItemObserver*)controllerRuler->getView();
  QHorizontalRulerLayoutItem* rectRuler = dynamic_cast<QHorizontalRulerLayoutItem*>(itemRuler);
  rectRuler->setPPI(logicalDpiX());
  rectRuler->setItemPosition(llx + 10, lly);
  rectRuler->redraw();

  VerticalRulerLayoutModel* modelRulerV = new VerticalRulerLayoutModel();		
  modelRulerV->setBox(te::gm::Envelope(llx, lly, llx + 10, ury + 10));
  modelRulerV->invertedLines(true);
  VerticalRulerLayoutController* controllerRulerV = new VerticalRulerLayoutController(modelRulerV);
  LayoutItemObserver* itemRulerV = (LayoutItemObserver*)controllerRulerV->getView();
  QVerticalRulerLayoutItem* rectRulerV = dynamic_cast<QVerticalRulerLayoutItem*>(itemRulerV);
  rectRulerV->setPPI(logicalDpiX());		
  rectRulerV->setItemPosition(llx, lly + 10);
  rectRulerV->redraw();
    
  //Retângulo: utilizando o canvas da Terralib 5
  RectangleLayoutModel* model = new RectangleLayoutModel();	
  model->setName("AZUL");
  RectangleLayoutController* controller = new RectangleLayoutController(model);
  LayoutItemObserver* item = (LayoutItemObserver*)controller->getView();
  QRectangleLayoutItem* rect = dynamic_cast<QRectangleLayoutItem*>(item);
  item->setItemPosition(llx, lly);
  item->redraw();
	
  //Retângulo: utilizando o canvas da Terralib 5
  RectangleLayoutModel* model2 = new RectangleLayoutModel();		  
  model2->setName("VERDE");
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
  rectText40->setTransform(lScene->getMatrixViewScene().inverted());
  rectText40->setPos(30, -20); //Coordenada de cena em mm		

  //-------------------------------------------------------------------------------------------------------------------

  if(_dockParent)
  {
    //Use the Property Browser Framework for create Property Window
    PropertiesWindowLayoutModel* dockPropertyModel = new PropertiesWindowLayoutModel();		 
    PropertiesWindowLayoutController* dockPropertyController = new PropertiesWindowLayoutController(dockPropertyModel);
    LayoutOutsideObserver* itemDockProperty = (LayoutOutsideObserver*)dockPropertyController->getView();
    _dockProperties = dynamic_cast<QPropertiesWindowOutside*>(itemDockProperty);
    
    _dockProperties->setParent(_dockParent); //The father need be the window of application, in this case, terraview main window!  
    _dockProperties->setVisible(true);

    //Use the Property Browser Framework for create Object Inspector Window
    ObjectInspectorWindowLayoutModel* dockInspectorModel = new ObjectInspectorWindowLayoutModel();		 
    ObjectInspectorWindowLayoutController* dockInspectorController = new ObjectInspectorWindowLayoutController(dockInspectorModel);
    LayoutOutsideObserver* itemDockInspector = (LayoutOutsideObserver*)dockInspectorController->getView();
    _dockInspector = dynamic_cast<QObjectInspectorWindowOutside*>(itemDockInspector);
    _dockInspector->setParent(_dockParent); //The father need be the window of application, in this case, terraview main window!
    _dockInspector->setVisible(true);

    //Use the Property Browser Framework for create Object Inspector Window
    ToolbarWindowLayoutModel* dockToolbarModel = new ToolbarWindowLayoutModel();		 
    ToolbarWindowLayoutController* dockToolbarController = new ToolbarWindowLayoutController(dockToolbarModel);
    LayoutOutsideObserver* itemDockToolbar = (LayoutOutsideObserver*)dockToolbarController->getView();
    _dockToolbar = dynamic_cast<QToolbarWindowOutside*>(itemDockToolbar);
    _dockToolbar->init(this);    
    _dockToolbar->setParent(_dockParent); //The father need be the window of application, in this case, terraview main window!
    _dockToolbar->setVisible(true);
  }
      
  scene()->setBackgroundBrush(QBrush(QColor(105,105,030)));
}

void te::layout::QLayoutView::resizeEvent(QResizeEvent * event)
{
  QGraphicsView::resizeEvent(event);
}

int te::layout::QLayoutView::metric( PaintDeviceMetric metric ) const 
{
  switch(metric)
  {
    case PdmWidthMM:
    {
      return 509;
    }
    case PdmHeightMM:
    {
      return 286;
    }
    case PdmPhysicalDpiX:
    {
      return 95.78;
      //Caso esteja imprimindo, obter o dpi da impressora
      //.
      //.
      //.
    }
    case PdmPhysicalDpiY:
    {
      return 95.78;
      //Caso esteja imprimindo, obter o dpi da impressora
      //.
      //.
      //.
    }
    case PdmDpiX:
    {
      return 95.78;
      //Caso esteja imprimindo, obter o dpi da impressora
      //.
      //.
      //.
    }
    case PdmDpiY:
    {
      return 95.78;
      //Caso esteja imprimindo, obter o dpi da impressora
      //.
      //.
      //.
    }
  }
	
  return QGraphicsView::metric(metric);
}

void te::layout::QLayoutView::paintEvent( QPaintEvent * event )
{
  QGraphicsView::paintEvent(event);
}

void te::layout::QLayoutView::setDockPropertiesParent( QWidget* dockParent )
{
  _dockParent = dockParent;
}

void te::layout::QLayoutView::hideEvent( QHideEvent * event )
{
  QGraphicsView::hideEvent(event);
}

void te::layout::QLayoutView::closeEvent( QCloseEvent * event )
{
  QGraphicsView::closeEvent(event);

  QMainWindow* win = (QMainWindow*)_dockParent;

  if(win)
  {
    win->removeDockWidget(_dockProperties);
    win->removeDockWidget(_dockInspector);

    _dockProperties->close();
    _dockProperties->setParent(0);

    _dockInspector->close();
    _dockInspector->setParent(0);
  }
}

void te::layout::QLayoutView::changeMode()
{
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
