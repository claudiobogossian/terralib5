#include <QLayoutScene.h>

#include <QGraphicsScene.h>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QtGui>
#include <QPixmap>

#include "LayoutItemObserver.h"
#include <QWidget>
#include "LayoutContext.h"
#include "LayoutMode.h"
#include "QLayoutItemGroup.h"
#include "ItemGroupLayoutModel.h"
#include "ItemGroupLayoutController.h"
#include "LayoutAbstractType.h"

te::layout::QLayoutScene::QLayoutScene( QWidget* widget): 
  QGraphicsScene(widget),
  _paperType(TPA4),
  _paperSizeWMM(210.),
  _paperSizeHMM(297.)
{

}

te::layout::QLayoutScene::~QLayoutScene()
{

}

void te::layout::QLayoutScene::init( double widthMM, double heightMM, double dpix, double dpiy )
{
  _screenWidthMM = widthMM;
  _screenHeightMM = heightMM;

  _logicalDpiX = dpix;
  _logicalDpiY = dpiy;

  _boxW = calculateWindow();
  _boxPaperW = calculateBoxPaper();

  calculateMatrixViewScene();
  calculateMatrixSceneItem();

  createMasterParentItem();
}

void te::layout::QLayoutScene::insertItem( LayoutItemObserver* item )
{
  QGraphicsItem* qitem = ((QGraphicsItem*)item);
  qitem->setParentItem(_masterParent);
  this->addItem(qitem);
}

void te::layout::QLayoutScene::insertOutside( LayoutOutsideObserver* widget )
{
  QWidget* qWidget = ((QWidget*)widget);
  this->addWidget(qWidget);
}

QGraphicsProxyWidget* te::layout::QLayoutScene::insertOutsideProxy( LayoutOutsideObserver* widget )
{
  QWidget* qWidget = ((QWidget*)widget);
  return this->addWidget(qWidget);
}

void te::layout::QLayoutScene::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
  QGraphicsScene::mousePressEvent(mouseEvent);
}

te::gm::Envelope te::layout::QLayoutScene::getSceneBox()
{
  QRectF srect =	sceneRect();

  QPointF ll = srect.bottomLeft();
  QPointF ur = srect.topRight();

  te::gm::Envelope box(ll.x(), ll.y(), ur.x(), ur.y());
  return box;
}

QGraphicsItemGroup* te::layout::QLayoutScene::createItemGroup( const QList<QGraphicsItem *> & items )
{
  //The scene create a new group with important restriction
  QGraphicsItemGroup* p = QGraphicsScene::createItemGroup(items);
  //Create a new group
  ItemGroupLayoutModel* model = new ItemGroupLayoutModel();		  
  ItemGroupLayoutController* controller = new ItemGroupLayoutController(model);
  LayoutItemObserver* item = (LayoutItemObserver*)controller->getView();
  QLayoutItemGroup* group = dynamic_cast<QLayoutItemGroup*>(item);

  if(p)
  {
    group->setParentItem(p->parentItem());    
    foreach (QGraphicsItem *item, p->childItems())
      group->addToGroup(item);
    
    delete p;
  }  

  return group;
}

void te::layout::QLayoutScene::destroyItemGroup( QGraphicsItemGroup *group )
{
  group->setHandlesChildEvents(false);
  QGraphicsScene::destroyItemGroup(group);
}

te::gm::Envelope te::layout::QLayoutScene::calculateBoxPaper()
{
  double x1 = 0.;
  double y1 = 0.;
  double x2 = 0.;
  double y2 = 0.;

  te::gm::Envelope boxW = calculateWindow();

  int widthW = boxW.getWidth();
  int heightW = boxW.getHeight();

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

  te::gm::Envelope box(x1, y1, x2, y2);
  return box;
}

te::gm::Envelope te::layout::QLayoutScene::calculateWindow()
{
  double x1 = 0;
  double y1 = 0;
  double x2 = 0;
  double y2 = 0;

  int w = _screenWidthMM;
  int h = _screenHeightMM;

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

  te::gm::Envelope box(x1, y1, x2, y2);
  return box;
}

void te::layout::QLayoutScene::calculateMatrixViewScene()
{
  double llx = _boxW.getLowerLeftX();
  double lly = _boxW.getLowerLeftY();
  double urx = _boxW.getUpperRightX();
  double ury = _boxW.getUpperRightY();
  
  //Window - Mundo
  double dpiX = _logicalDpiX;
  double dpiY = _logicalDpiY;

  //mm (invers�o do y)
  _matrix = QTransform().scale(dpiX / 25.4, -dpiY / 25.4).translate(-llx, -ury);

  //Coordenadas de mundo - mm
  setSceneRect(QRectF(QPointF(llx, lly), QPointF(urx, ury)));
}

void te::layout::QLayoutScene::calculateMatrixSceneItem()
{
  //Matrix for items ( between scene and items )
  //take translate
  double m_31 = _matrix.inverted().m31();
  double m_32 = _matrix.inverted().m32(); 
  //m11 and m22 : scale
  //Create matrix for items
  // shearing( m21 and m12, cisalhamento ) : distor��o 
  // m31 and m32 : translate  
  _matrixItem = QTransform(1, 0, 0, 0, -1, 0, m_31, m_32, 1);
}

te::gm::Envelope te::layout::QLayoutScene::getWorldBox()
{
 return _boxW; 
}

te::gm::Envelope te::layout::QLayoutScene::getPaperBox()
{
  return _boxPaperW;
}

QTransform te::layout::QLayoutScene::getMatrixViewScene()
{
  return _matrix;
}

QTransform te::layout::QLayoutScene::getMatrixSceneItem()
{
  return _matrixItem;
}

void te::layout::QLayoutScene::createMasterParentItem()
{
  double llx = _boxW.getLowerLeftX();
  double lly = _boxW.getLowerLeftY();

  //Background
  QRectF sceneRectBack = sceneRect();	
  _masterParent = addRect(sceneRectBack);
  _masterParent->setTransform(_matrixItem.inverted()); 
  ((QGraphicsRectItem*)_masterParent)->setBrush((QBrush(QColor(109,109,109))));	
  _masterParent->setPos(llx, lly);
}

QGraphicsItem* te::layout::QLayoutScene::getMasterParentItem()
{
  return _masterParent;
}

void te::layout::QLayoutScene::printPaper()
{
  //Impress�o de parte da Cena
    //N�o � necess�rio mudar a escala do View

    QPrinter* printer=new QPrinter(QPrinter::HighResolution);
    printer->setPageSize(QPrinter::A4);
    printer->setOrientation( QPrinter::Portrait );
    printer->pageRect(QPrinter::Millimeter);

    if (QPrintDialog(printer).exec() == QDialog::Accepted) 
    {
      QPainter painter(printer);
      painter.setRenderHint(QPainter::Antialiasing);
      painter.scale(1., 1.);
      
      double dpiX = LayoutContext::getInstance()->getDpiX();
      double dpiY = LayoutContext::getInstance()->getDpiY();

      LayoutContext::getInstance()->setDpiX(printer->logicalDpiX());
      LayoutContext::getInstance()->setDpiY(printer->logicalDpiY());
      
      te::gm::Envelope paperBox =  getPaperBox();
      
      //Box da folha na Cena (Fonte)
      QRectF mmSourceRect(paperBox.getLowerLeftX(), paperBox.getLowerLeftY(), 
        paperBox.getWidth(), paperBox.getHeight());

      //Tamanho da folha utilizando o dpi da impressora (Alvo)
      QRect pxTargetRect(0, 0, painter.device()->width(), painter.device()->height());

      //Imprimir folha (Cena para Impressora)
      //draw items with printer painter
      /*
        Idea: Inside the paint object, whether in print mode, 
        the pixmap before drawing, redrawing the object (model redraw).
      */
      render(&painter, pxTargetRect, mmSourceRect); // upside down
      
      LayoutContext::getInstance()->setDpiX(dpiX);
      LayoutContext::getInstance()->setDpiY(dpiY);

    }
}

void te::layout::QLayoutScene::savePaperAsImage( std::string pathWithFileName)
{
  QPixmap pixmap;
  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.scale(1., 1.);

  te::gm::Envelope paperBox =  getPaperBox();
  //Box da folha na Cena (Fonte)
  QRectF mmSourceRect(paperBox.getLowerLeftX(), paperBox.getLowerLeftY(), 
    paperBox.getWidth(), paperBox.getHeight());

  //Tamanho da folha utilizando o dpi da impressora (Alvo)
  //1� Converter tamanho do papel para pixel?????????
  //Utilizar canvas como painter?
  QRect pxTargetRect(0, 0, painter.device()->width(), painter.device()->height());

  //Imprimir folha (Cena para Impressora)
  //draw items with printer painter
  /*
    Idea: Inside the paint object, whether in print mode, 
    the pixmap before drawing, redrawing the object (model redraw).
  */
  render(&painter, pxTargetRect, mmSourceRect); // upside down
  render(&painter);
  painter.end();

  pixmap.save(QString(pathWithFileName.c_str()));
}
