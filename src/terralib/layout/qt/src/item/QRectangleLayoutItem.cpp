#include "QRectangleLayoutItem.h"
#include "LayoutItemController.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include <QGraphicsPixmapItem>
#include "LayoutItemModelObservable.h"
#include <QCursor>
#include <QPixmap>
#include <QPainter>

#include "../../../color/RGBAColor.h"
#include "../../../../qt/widgets/Utils.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../common/STLUtils.h"

te::layout::QRectangleLayoutItem::QRectangleLayoutItem( LayoutItemController* controller, LayoutItemModelObservable* o ) :
  QGraphicsPixmapItem(0, 0),
  LayoutItemObserver(controller, o),
  _mousePressedCtrl(false),
  _hoverAboveItem(false),
  _toResizeItem(false),
  _enumSides(TPNoneSide)
{
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges);

  QGraphicsItem* item = this;
  LayoutContext::getInstance()->getScene()->insertItem((LayoutItemObserver*)item);
  
  //If enabled is true, this item will accept hover events
  setAcceptHoverEvents(true);
}

te::layout::QRectangleLayoutItem::~QRectangleLayoutItem()
{

}

void te::layout::QRectangleLayoutItem::updateObserver( ContextLayoutItem context )
{
  te::color::RGBAColor** rgba = context.getPixmap();  
  LayoutItemModelObservable* model = (LayoutItemModelObservable*)_controller->getModel();
  te::gm::Envelope box = model->getBox();
  
  QPixmap pixmp;
  QImage* img = 0;
  
  if(rgba)
  {
    img = te::qt::widgets::GetImage(rgba, box.getWidth(), box.getHeight());
    pixmp = QPixmap::fromImage(*img);
  }

  te::common::Free(rgba, box.getHeight());
  if(img)
    delete img;
  
  setPixmap(pixmp);
  update();
}

void te::layout::QRectangleLayoutItem::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
  _controller->redraw(1.);
}

QVariant te::layout::QRectangleLayoutItem::itemChange( GraphicsItemChange change, const QVariant &value )
{

  if (change == ItemTransformHasChanged || change == ItemTransformChange || change == ItemScaleHasChanged)
  {
    int a = 0;
    a = a + 1;
  }

  return QGraphicsItem::itemChange(change, value);
}

void te::layout::QRectangleLayoutItem::setPosition( const double& x, const double& y )
{
  setPos(x,y);
}

te::gm::Coord2D te::layout::QRectangleLayoutItem::getPosition()
{
  QPointF posF = scenePos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::QRectangleLayoutItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{  
  QGraphicsItem::mousePressEvent(event);

  if(event->modifiers() == Qt::AltModifier && _toResizeItem)
  {
    _clonePixmap = pixmap();
    _mousePressedCtrl = true;
  }
}

void te::layout::QRectangleLayoutItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{  
  QGraphicsItem::mouseReleaseEvent(event);

  //Setar o novo box do model    
  LayoutItemModelObservable* model = (LayoutItemModelObservable*)_controller->getModel();
  te::gm::Envelope boxScene = createNewBoxInCoordScene(event->scenePos().x(), event->scenePos().y());
  if(boxScene.isValid() && boxScene.getWidth() > 0 && boxScene.getHeight() > 0)
    model->setBox(boxScene);

  _mousePressedCtrl = false;

  if(_toResizeItem && boxScene.isValid())
  {
    _toResizeItem = false;
    //Antes é necessário saber se o pixmap continua o mesmo, ou foi modificado.
    //Só chamará o redraw se foi modificado.

    double zoomfactor = LayoutContext::getInstance()->getZoomFactor();
    redraw(zoomfactor);
    setOpacity(1.);
  }
}

void te::layout::QRectangleLayoutItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
  if(event->modifiers() == Qt::AltModifier && event->buttons() == Qt::LeftButton && _toResizeItem)
  {
    _mousePressedCtrl = true;
    setOpacity(0.5);

    QPixmap pix = calculateNewPixmap(event->scenePos().x(), event->scenePos().y());
    setPixmap(pix);
    update();
  }
  else
  {
    if(!_toResizeItem)
      setOpacity(1.);
    _mousePressedCtrl = false;
    QGraphicsItem::mouseMoveEvent(event);
  }
}

void te::layout::QRectangleLayoutItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
  QGraphicsItem::hoverEnterEvent(event);
}

void te::layout::QRectangleLayoutItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
  _hoverAboveItem = false;
  setCursor(Qt::ArrowCursor);
  QGraphicsItem::hoverLeaveEvent(event);
}

void te::layout::QRectangleLayoutItem::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
  _hoverAboveItem = true;
  _toResizeItem = checkTouchesCorner(event->pos().x(), event->pos().y());
  QGraphicsItem::hoverMoveEvent(event);
}

bool te::layout::QRectangleLayoutItem::checkTouchesCorner( const double& x, const double& y )
{
  bool result = true;
  QRectF bRect = boundingRect();
  double margin = 2.; //precision

  QPointF ll = bRect.bottomLeft();
  QPointF lr = bRect.bottomRight();
  QPointF tl = bRect.topLeft();
  QPointF tr = bRect.topRight();

  if((x >= (ll.x() - margin) && x <= (ll.x() + margin))
    && (y >= (ll.y() - margin) && y <= (ll.y() + margin)))
  {
    setCursor(Qt::SizeBDiagCursor);
    _enumSides = TPLowerLeft;
  }
  else if((x >= (lr.x() - margin) && x <= (lr.x() + margin))
    && (y >= (lr.y() - margin) && y <= (lr.y() + margin)))
  {
    setCursor(Qt::SizeFDiagCursor);
    _enumSides = TPLowerRight;
  }
  else if((x >= (tl.x() - margin) && x <= (tl.x() + margin))
    && (y >= (tl.y() - margin) && y <= (tl.y() + margin)))
  {
    setCursor(Qt::SizeFDiagCursor);
    _enumSides = TPTopLeft;
  }
  else if((x >= (tr.x() - margin) && x <= (tr.x() + margin))
    && (y >= (tr.y() - margin) && y <= (tr.y() + margin)))
  {
    setCursor(Qt::SizeBDiagCursor);
    _enumSides = TPTopRight;
  }
  else
  {
    setCursor(Qt::ArrowCursor);
    _enumSides = TPNoneSide;
    result = false;
  }

  return result;
}

QPixmap te::layout::QRectangleLayoutItem::calculateNewPixmap( const double& x, const double& y )
{    
  te::gm::Envelope boxScene = createNewBoxInCoordScene(x, y);
  QPixmap pix = pixmap().scaled(boxScene.getWidth(), boxScene.getHeight());

  if(pix.isNull())
    pix = pixmap();

  return pix;
}

te::gm::Envelope te::layout::QRectangleLayoutItem::createNewBoxInCoordScene( const double& x, const double& y )
{
  LayoutItemModelObservable* model = (LayoutItemModelObservable*)_controller->getModel();
  te::gm::Envelope boxScene = model->getBox();;
  
  QPointF posItem = this->scenePos();
  QPointF posAtual(x, y);
    
  double xTranslation = 0;
  double yTranslation = 0;

  double x1 = posItem.x() < posAtual.x() ? posItem.x() : posAtual.x();
  double y1 = posItem.y() < posAtual.y() ? posItem.y() : posAtual.y();
  double x2 = posItem.x() > posAtual.x() ? posItem.x() : posAtual.x();
  double y2 = posItem.y() > posAtual.y() ? posItem.y() : posAtual.y();

  double dx = x2 - x1;
  double dy = y2 - y1;

  if(_mousePressedCtrl && _toResizeItem)
  {
    switch(_enumSides)
    {
      case TPLowerRight :
        {
          boxScene = te::gm::Envelope(posItem.x(), posItem.y() - dy, posItem.x() + dx, posItem.y());
          break;
        }
      case TPLowerLeft:
        {
          dx = posItem.x() - posAtual.x();
          dy = posItem.y() - posAtual.y();
                    
          QPointF pbxy2 = mapToScene(boundingRect().topRight());
          if(posAtual.x() < pbxy2.x() && posAtual.y() < posItem.y())
          {
            boxScene = te::gm::Envelope(posItem.x() - dx, posItem.y() - dy, pbxy2.x(), posItem.y());

            QPointF p_f = mapFromScene(QPointF(posItem.x() - dx, posItem.y()));
            QPointF p_ff = mapToParent(p_f);                                  
            xTranslation = p_ff.x();
            yTranslation = p_ff.y();

            //In Parent Coordinates
            setPos( xTranslation, yTranslation );
          }
          break;
        }
      case TPTopRight:
        {
          QPointF pbxy2 = mapToScene(boundingRect().topRight());

          dx = posAtual.x() - pbxy2.x();
          dy = posAtual.y() - pbxy2.y();

          QPointF pbxy1 = mapToScene(boundingRect().bottomLeft());
          if(posAtual.x() > posItem.x() && posAtual.y() > pbxy1.y())
          {
            boxScene = te::gm::Envelope(posItem.x(), pbxy1.y(), pbxy2.x() + dx, posItem.y() + dy);

            QPointF p_f = mapFromScene(QPointF(posItem.x(), posItem.y() + dy));
            QPointF p_ff = mapToParent(p_f);
            xTranslation = p_ff.x();
            yTranslation = p_ff.y();

            //In Parent Coordinates
            setPos( xTranslation, yTranslation );
          }
          break;
        }
      case TPTopLeft :
        {  
          QPointF pbxy2 = mapToScene(boundingRect().topLeft());

          dx = posAtual.x() - pbxy2.x();
          dy = posAtual.y() - pbxy2.y();

          QPointF pbxy1 = mapToScene(boundingRect().bottomRight());
          if(posAtual.x() < pbxy1.x() && posAtual.y() > pbxy1.y())
          {
            boxScene = te::gm::Envelope(posItem.x() + dx, pbxy1.y(), pbxy1.x(), posItem.y() + dy);

            QPointF p_f = mapFromScene(QPointF(posItem.x() + dx, posItem.y() + dy));
            QPointF p_ff = mapToParent(p_f);
            xTranslation = p_ff.x();
            yTranslation = p_ff.y();

            //In Parent Coordinates
            setPos( xTranslation, yTranslation );
          }
          break;
        }
      default :
        {
          boxScene = te::gm::Envelope(posItem.x(), posItem.y() - boundingRect().height(), 
            posItem.x() + boundingRect().width(), posItem.y());
          break;
        }
    }
  }
  
  return boxScene;
}

bool te::layout::QRectangleLayoutItem::contains( const QPointF &point ) const
{
  LayoutItemModelObservable* model = (LayoutItemModelObservable*)_controller->getModel();
  return model->contains(te::gm::Coord2D(point.x(), point.y()));
}
