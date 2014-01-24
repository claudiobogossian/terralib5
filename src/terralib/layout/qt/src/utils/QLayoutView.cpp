#include <QLayoutView.h>

#include <QGraphicsView>
#include <QtGui>
#include <QGraphicsItem>
#include "LayoutItem.h"
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
#include "GroupRulerLayoutModel.h"

#define _psPointInMM 0.352777778 //<! 1 PostScript point in millimeters
#define _inchInPSPoints 72 //<! 1 Inch in PostScript point
#define _inchInMillimeters 25.4 //<! 1 Inch in millimeters

te::layout::QLayoutView::QLayoutView( QWidget* widget) : 
  QGraphicsView(new QGraphicsScene, widget),
  _paperType(TPA4),
  _paperSizeWMM(210.),
  _paperSizeHMM(297.),
  _diagonalScreenInchSize(0)
{
  _layoutController = 0;

  _boxW = calculateWindow();
  _boxPaperW = calculateBoxPaper();
  /*setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);*/
}

te::layout::QLayoutView::~QLayoutView()
{
  if(scene())
  scene()->clear();
}

void te::layout::QLayoutView::mousePressEvent( QMouseEvent * event )
{
  QGraphicsView::mousePressEvent(event);

  if(LayoutContext::getInstance()->getMode() == TypeCreate)
  {
    _layoutController->createNewItem(event->x(), event->y());
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

  //_matrix = QTransform().scale(scaleX * scaleFactor, scaleY * scaleFactor).translate(-_boxW.getX1(), -_boxW.getY2());
  qreal factor = QTransform().scale(scaleX, scaleY).mapRect(QRectF(0, 0, 1, 1)).width();
  //qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();

  if (factor < 0.5 || factor > 10)
  return;

  scale(scaleFactor, scaleFactor);

  LayoutContext::getInstance()->setZoomFactor(factor);
			
  QList<QGraphicsItem*> graphicsItems = this->scene()->items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      //item->setTransform(this->transform().inverted());
			
      QGraphicsWidget* outside = dynamic_cast<QGraphicsWidget*>(item);

      if(outside)
        continue;

      LayoutItem* lItem = dynamic_cast<LayoutItem*>(item);
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
  if(keyEvent->key() == Qt::Key_P)
  {
    scaleView(1.);

    QPrinter* printer=new QPrinter(QPrinter::HighResolution);
    printer->setPageSize(QPrinter::A1);
    printer->setOrientation( QPrinter::Landscape );
    printer->pageRect(QPrinter::Millimeter);
    if (QPrintDialog(printer).exec() == QDialog::Accepted) {
      QPainter painter(printer);
      painter.setRenderHint(QPainter::Antialiasing);
      painter.scale(1., 1.);
      render(&painter);
    }
  }
  else if (keyEvent->key() == Qt::Key_I)
  {
    QString fileName = QFileDialog::getOpenFileName(QApplication::desktop(),"Open Image File",QDir::currentPath());
    if(!fileName.isEmpty())
    {
      QImage image(fileName);
      if(image.isNull())
      {
        QMessageBox::information(QApplication::desktop(),"Image Viewer","Error Displaying image");
      }
      else
      {
        _fileName = fileName;
      }
    }
}

  QGraphicsView::keyPressEvent(keyEvent);
}

void te::layout::QLayoutView::config()
{	
  _boxW = calculateWindow();
  _boxPaperW = calculateBoxPaper();
  
  double llx = _boxW->getLowerLeftX();
  double lly = _boxW->getLowerLeftY();
  double urx = _boxW->getUpperRightX();
  double ury = _boxW->getUpperRightY();

  double w = _boxW->getWidth();
  double h = _boxW->getHeight();
		
  //Window - Mundo
  double dpiX = this->logicalDpiX();
  double dpiY = this->logicalDpiY();
			
  //mm (inversão do y)
  _matrix = QTransform().scale(dpiX / 25.4, -dpiY / 25.4).translate(-llx, -ury);

  //Coordenadas de mundo - mm
  scene()->setSceneRect(QRectF(QPointF(llx, lly), QPointF(urx, ury)));
		
  //Transform calcula automaticamente a matriz inversa
  setTransform(_matrix);
  			
  setTransformationAnchor(QGraphicsView::NoAnchor);	
  QPointF center0 = mapToScene(0, 0);
  centerOn(center0);
			
  //----------------------------------------------------------------------------------------------

  //mm entre Cena e Itens
  _matrixItem = QTransform().scale(1., -1).translate(Qt::XAxis, -ury);

  //Background
  QRectF sceneRectBack = scene()->sceneRect();	
  QGraphicsRectItem* rectItemBack1 = scene()->addRect(sceneRectBack);
  rectItemBack1->setTransform(_matrixItem); 
  rectItemBack1->setBrush((QBrush(QColor(109,109,109))));	
  rectItemBack1->setPos(0, 0);
	
  //Paper
  QGraphicsRectItem* rectItemScene3 = scene()->addRect(0,0,_boxPaperW->getWidth(),_boxPaperW->getHeight());
  rectItemScene3->setBrush((QBrush(QColor(255,255,255))));
  rectItemScene3->setParentItem(rectItemBack1);
  rectItemScene3->moveBy(0,0); //Coordenada de cena em mm
	
  ////Régua: utilizando o canvas da Terralib 5
  //HorizontalRulerLayoutModel* modelRuler = new HorizontalRulerLayoutModel();		
  //HorizontalRulerLayoutController* controllerRuler = new HorizontalRulerLayoutController(modelRuler);
  //modelRuler->setBox(new te::gm::Envelope(llx, lly, urx, lly + 10));
  //modelRuler->invertedLines(true);
  //LayoutObserver* oRuler = (LayoutObserver*)controllerRuler->getView();
  //LayoutItem* itemRuler = dynamic_cast<LayoutItem*>(oRuler);
  //QHorizontalRulerLayoutItem* rectRuler = dynamic_cast<QHorizontalRulerLayoutItem*>(itemRuler);
  //rectRuler->setPPI(logicalDpiX());		
  //rectRuler->setParentItem(rectItemBack1);
  //controllerRuler->redraw(1.);
  //rectRuler->moveBy(llx, lly);

  //VerticalRulerLayoutModel* modelRulerV = new VerticalRulerLayoutModel();		
  //VerticalRulerLayoutController* controllerRulerV = new VerticalRulerLayoutController(modelRulerV);
  //modelRulerV->setBox(new te::gm::Envelope(llx, lly, llx + 10, ury));
  //modelRulerV->invertedLines(true);
  //LayoutObserver* oRulerV = (LayoutObserver*)controllerRulerV->getView();
  //LayoutItem* itemRulerV = dynamic_cast<LayoutItem*>(oRulerV);
  //QVerticalRulerLayoutItem* rectRulerV = dynamic_cast<QVerticalRulerLayoutItem*>(itemRulerV);
  //rectRulerV->setPPI(logicalDpiX());		
  //rectRulerV->setParentItem(rectItemBack1);
  //controllerRulerV->redraw(1.);
  //rectRulerV->moveBy(llx, lly);

  //Régua: utilizando o canvas da Terralib 5
  HorizontalRulerLayoutModel* modelRulerH = new HorizontalRulerLayoutModel();		
  modelRulerH->setBox(new te::gm::Envelope(llx, lly, urx, lly + 10));
  modelRulerH->invertedLines(true);

  VerticalRulerLayoutModel* modelRulerV = new VerticalRulerLayoutModel();	
  modelRulerV->setBox(new te::gm::Envelope(llx, lly, llx + 10, ury));
  modelRulerV->invertedLines(true);
 
  GroupRulerLayoutModel* modelGroupRuler = new GroupRulerLayoutModel(modelRulerV, modelRulerH);		
  VerticalRulerLayoutController* controllerGroupRuler = new VerticalRulerLayoutController(modelGroupRuler);
  LayoutObserver* oRulerGroupRuler = (LayoutObserver*)controllerGroupRuler->getView();
  LayoutItem* itemGroupRuler = dynamic_cast<LayoutItem*>(oRulerGroupRuler);
  QVerticalRulerLayoutItem* rectGroupRuler = dynamic_cast<QVerticalRulerLayoutItem*>(itemGroupRuler);
  rectGroupRuler->setPPI(logicalDpiX());		
  rectGroupRuler->setParentItem(rectItemBack1);
  controllerGroupRuler->redraw(1.);
  rectGroupRuler->moveBy(llx, lly);

  //Retângulo: utilizando o canvas da Terralib 5
  RectangleLayoutModel* model = new RectangleLayoutModel();		  
  RectangleLayoutController* controller = new RectangleLayoutController(model);
  LayoutObserver* o = (LayoutObserver*)controller->getView();
  LayoutItem* item = dynamic_cast<LayoutItem*>(o);
  QRectangleLayoutItem* rect = dynamic_cast<QRectangleLayoutItem*>(item);
  item->redraw();
  rect->moveBy(llx, lly);
	
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
  rectText40->setTransform(_matrix.inverted());
  rectText40->moveBy(30, -20); //Coordenada de cena em mm		
  
  scene()->setBackgroundBrush(QBrush(QColor(105,105,030)));
}

te::gm::Envelope* te::layout::QLayoutView::calculateBoxPaper()
{
  double x1 = 0.;
  double y1 = 0.;
  double x2 = 0.;
  double y2 = 0.;

  te::gm::Envelope* boxW = calculateWindow();

  int widthW = boxW->getWidth();
  int heightW = boxW->getHeight();

  double middleWW = widthW/2.;
  double middleHW = heightW/2.;	

  double ppW = _paperSizeWMM/2.;
  double ppH = _paperSizeHMM/2.;

  if(widthW > _paperSizeWMM)
  {
    x1 = 0;
    x2 = _paperSizeWMM;
  }
  else
  {
    x1 = middleWW - ppW;
    x2 = x1 + _paperSizeWMM;
  }

  if(heightW > _paperSizeHMM)
  {
    y1 = 0;
    y2 = _paperSizeHMM;
  }
  else
  {
    y1 = middleHW - ppH;
    y2 = y1 + _paperSizeHMM;
  }

  te::gm::Envelope* box = new te::gm::Envelope(x1, y1, x2, y2);
  return box;
}

te::gm::Envelope* te::layout::QLayoutView::calculateWindow()
{
  double x1 = 0;
  double y1 = 0;
  double x2 = 0;
  double y2 = 0;
  
  int w = widthMM();
  int h = heightMM();
			
  double paddingW = w - _paperSizeWMM;
  double margin1 = paddingW / 2.;

  if(w > _paperSizeWMM)
  {
    x1 = - margin1;
    x2 = (paddingW + _paperSizeWMM) - margin1;
  }
  else
  {
    x1 = 0;
    x2 = w;
  }
	
  double paddingH = h - _paperSizeHMM;
  double margin2 = paddingH / 2.;
	
  if(h > _paperSizeHMM)
  {
    y1 = - margin2;
    y2 = (paddingW + _paperSizeHMM) - margin2;

  }
  else
  {
    y1 = 0;
    y2 = h;
  }
	
  te::gm::Envelope* box = new te::gm::Envelope(x1, y1, x2, y2);
  return box;
}

void te::layout::QLayoutView::resizeEvent(QResizeEvent * event)
{
  if(event->size().height() != 1080 || event->size().width() != 1920)
  {
    /*scene()->clear();
    config();*/
  }

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

QPaintEngine* te::layout::QLayoutView::paintEngine() const
{
  QPaintEngine* engine = QGraphicsView::paintEngine();
  return engine;
}

QString te::layout::QLayoutView::fileName() const 
{ 
  return _fileName; 
}

void te::layout::QLayoutView::fileName(QString val) 
{ 
  _fileName = val; 
}

te::gm::Envelope* te::layout::QLayoutView::getWorldBox()
{
  return _boxW;
}
