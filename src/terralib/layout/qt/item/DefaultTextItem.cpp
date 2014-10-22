/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file DefaultTextItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "DefaultTextItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../core/Scene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../item/DefaultTextModel.h"

// STL
#include <string>

// Qt
#include <QTextDocument>
#include <QStyleOptionGraphicsItem>
#include <QTextCursor>
#include <QAbstractTextDocumentLayout>
#include <QGraphicsSceneMouseEvent>

te::layout::DefaultTextItem::DefaultTextItem( ItemController* controller, Observable* o ) :
  QGraphicsTextItem(0),
  ItemObserver(controller, o),
  m_table(0),
  m_oldAdjustSizeW(-1.),
  m_oldAdjustSizeH(-1.)
{  
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges
    | QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIgnoresTransformations);

  m_invertedMatrix = true;

  setTextInteractionFlags(Qt::NoTextInteraction);

  QGraphicsItem* item = this;
  m_nameClass = std::string(this->metaObject()->className());
  Context::getInstance().getScene()->insertItem((ItemObserver*)item);

  std::string name = m_model->getName();
  DefaultTextModel* model = dynamic_cast<DefaultTextModel*>(m_model);
  if(model)
  {
    model->setText(name);
  }

  //If enabled is true, this item will accept hover events
  setAcceptHoverEvents(true);

  m_boxCopy = m_model->getBox();

  QTextCursor cursor(document());
  cursor.movePosition(QTextCursor::Start);
  cursor.insertText(name.c_str());
  adjustSize();
}

te::layout::DefaultTextItem::~DefaultTextItem()
{

}

void te::layout::DefaultTextItem::init()
{
  if(!document())
    return;

  connect(document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(onContentsChange(int,int,int)));
}

void te::layout::DefaultTextItem::updateObserver( ContextItem context )
{
  if(!m_model)
    return;

  te::color::RGBAColor** rgba = context.getPixmap();

  if(!rgba)
    return;

  Utils* utils = context.getUtils();

  if(!utils)
    return;

  te::gm::Envelope box = utils->viewportBox(m_model->getBox());

  if(!box.isValid())
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

  /* This item ignores the transformations of the scene, so comes with no zoom. 
  His transformation matrix is the inverse scene, understanding the pixel 
  coordinates, and its position can only be given in the scene coordinates(mm). 
  For these reasons, it is necessary to scale and so accompany the zoom scene. */
  double zoomFactor = Context::getInstance().getZoomFactor();
  double oldZoomFactor = Context::getInstance().getOldZoomFactor();
  setScale(zoomFactor);

  Scene* sc = dynamic_cast<Scene*>(scene());

  if(sc->getPreviewState() != Scene::NoPrinter)
  {
    QTransform trf = sc->getMatrixViewScene();
    setScale(trf.m11());
  }
  
  refreshTable();

  setPixmap(pixmap);
  update();
}

te::gm::Coord2D te::layout::DefaultTextItem::getPosition()
{
  QPointF posF = scenePos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

int te::layout::DefaultTextItem::getZValueItem()
{
  return QGraphicsTextItem::zValue();
}

void te::layout::DefaultTextItem::setPixmap( const QPixmap& pixmap )
{
  m_pixmap = pixmap;

  if(m_pixmap.isNull())
    return;

  QPointF point = pos();

  te::gm::Envelope box = m_model->getBox();

  QGraphicsTextItem::prepareGeometryChange();

  setRect(QRectF(0, 0, box.getWidth(), box.getHeight()));
  update();
}

QPixmap te::layout::DefaultTextItem::getPixmap()
{
  return m_pixmap;
}

void te::layout::DefaultTextItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground( painter );

  QRectF boundRect;
  boundRect = boundingRect();

  painter->save();
  painter->translate( -boundRect.bottomLeft().x(), -boundRect.topRight().y() );
  painter->drawPixmap(boundRect, m_pixmap, QRectF( 0, 0, m_pixmap.width(), m_pixmap.height() ));
  painter->restore();  

  QGraphicsTextItem::paint(painter, option, widget);

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

QRectF te::layout::DefaultTextItem::boundingRect() const
{
  return m_rect;
}

void te::layout::DefaultTextItem::setRect( QRectF rect )
{
  if (rect.isEmpty() && !rect.isNull())
    return;

  m_rect = rect;
  update(rect);
}

bool te::layout::DefaultTextItem::contains( const QPointF &point ) const
{
  te::gm::Coord2D coord(point.x(), point.y());
  return m_controller->contains(coord);
}

void te::layout::DefaultTextItem::setPos( const QPointF &pos )
{
  /* The matrix transformation of DefaultTextItem object is the inverse of the scene, 
  so you need to do translate when you change the position, since the coordinate 
  must be in the world coordinate. */
  QPointF pt(pos.x() - transform().dx(), pos.y() - transform().dy());

  QGraphicsTextItem::setPos(pt);

  refresh();
}

void te::layout::DefaultTextItem::setZValue( qreal z )
{
  QGraphicsTextItem::setZValue(z);
  setZValueItem(z);
}

void te::layout::DefaultTextItem::drawBackground( QPainter* painter )
{
  if (painter)
  {
    //painter->setBrush(brush());
    painter->setPen(Qt::NoPen);
    painter->setRenderHint( QPainter::Antialiasing, true );
    painter->drawRect(QRectF( 0, 0, boundingRect().width(), boundingRect().height()));
  }
}

void te::layout::DefaultTextItem::drawSelection( QPainter* painter )
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

void te::layout::DefaultTextItem::onContentsChange( int position, int charsRemoved, int charsAdded )
{
  if(!document())
    return;
  
  double vw = document()->documentLayout()->documentSize().width();
  double vh = document()->documentLayout()->documentSize().height();

  if(vw != m_oldAdjustSizeW || vh != m_oldAdjustSizeH)
  {
    adjustSizeMM();
  }
}

void te::layout::DefaultTextItem::adjustSizeMM()
{
  if(!m_model)
    return;
  
  if(!document())
    return;

  if(document()->isEmpty())
    return;
  
  Utils* utils = Context::getInstance().getUtils();

  if(!utils)
    return;

  te::gm::Envelope world = m_model->getBox();
  te::gm::Envelope viewport = utils->viewportBox(world);

  adjustSize();
    
  m_oldAdjustSizeW = document()->documentLayout()->documentSize().width();
  m_oldAdjustSizeH = document()->documentLayout()->documentSize().height();
  
  DefaultTextModel* model = dynamic_cast<DefaultTextModel*>(m_model);

  if(!model)
    return;

  te::gm::Envelope newBox(model->getBox().m_llx, model->getBox().m_lly, 
              model->getBox().m_llx + m_oldAdjustSizeW, model->getBox().m_lly + m_oldAdjustSizeH);
  
  model->setBox(newBox);

  setRect(QRectF(0, 0, newBox.getWidth(), newBox.getHeight()));
}

void te::layout::DefaultTextItem::refreshTable()
{
  refreshText();
}

void te::layout::DefaultTextItem::refreshText()
{
  DefaultTextModel* model = dynamic_cast<DefaultTextModel*>(m_model);
  if(!model)
    return;

  document()->clear();

  Font font = model->getFont();
  QFont qfont;
  qfont.setFamily(font.getFamily().c_str());
  qfont.setPointSize(font.getPointSize());
  qfont.setBold(font.isBold());
  qfont.setItalic(font.isItalic());
  qfont.setUnderline(font.isUnderline());
  qfont.setStrikeOut(font.isStrikeout());
  qfont.setKerning(font.isKerning());
  setFont(qfont);

  QTextCursor cursor(document());
  cursor.movePosition(QTextCursor::Start);
  cursor.insertText(model->getText().c_str());
  adjustSize();
}

void te::layout::DefaultTextItem::setTextInteraction( bool on, bool selectAll /*= false*/ )
{
  if(on && textInteractionFlags() == Qt::NoTextInteraction)
  {
    // switch on editor mode:
    setTextInteractionFlags(Qt::TextEditorInteraction);
    // manually do what a mouse click would do else:
    setFocus(Qt::MouseFocusReason); // this gives the item keyboard focus
    setSelected(true); // this ensures that itemChange() gets called when we click out of the item
    if(selectAll) // option to select the whole text (e.g. after creation of the TextItem)
    {
      QTextCursor c = textCursor();
      c.select(QTextCursor::Document);
      setTextCursor(c);
    }
  }
  else if(!on && textInteractionFlags() == Qt::TextEditorInteraction)
  {
    // turn off editor mode:
    setTextInteractionFlags(Qt::NoTextInteraction);
    // deselect text (else it keeps gray shade):
    QTextCursor c = this->textCursor();
    c.clearSelection();
    this->setTextCursor(c);
    clearFocus();
  }
}

void te::layout::DefaultTextItem::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *evt )
{
  if(textInteractionFlags() == Qt::TextEditorInteraction)
  {
    // if editor mode is already on: pass double click events on to the editor:
    QGraphicsTextItem::mouseDoubleClickEvent(evt);
    return;
  }

  // if editor mode is off:
  // 1. turn editor mode on and set selected and focused:
  setTextInteraction(true);

  // 2. send a single click to this QGraphicsTextItem (this will set the cursor to the mouse position):
  // create a new mouse event with the same parameters as evt
  QGraphicsSceneMouseEvent *click = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
  click->setButton(evt->button());
  click->setPos(evt->pos());
  QGraphicsTextItem::mousePressEvent(click);
  delete click; // don't forget to delete the event
}

QVariant te::layout::DefaultTextItem::itemChange( GraphicsItemChange change, const QVariant & value )
{
  if(change == QGraphicsItem::ItemSelectedChange 
    && textInteractionFlags() != Qt::NoTextInteraction 
    && !value.toBool())
  {
    // item received SelectedChange event AND is in editor mode AND is about to be deselected:
    setTextInteraction(false); // leave editor mode
  }
  return QGraphicsTextItem::itemChange(change, value);
}

void te::layout::DefaultTextItem::applyRotation()
{
  if(!m_model)
    return;

  ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
  if(!model)
    return;

  double angle = model->getAngle();

  QPointF center = boundingRect().center();

  double centerX = center.x();
  double centerY = center.y();

  setTransform(QTransform().translate(centerX, centerY).rotate(angle).translate(-centerX, -centerY));
}

bool te::layout::DefaultTextItem::checkTouchesCorner( const double& x, const double& y )
{
  bool result = true;
  QRectF bRect = boundingRect();
  double margin = 4.; //precision

  QPointF ll = bRect.bottomLeft();
  QPointF lr = bRect.bottomRight();
  QPointF tl = bRect.topLeft();
  QPointF tr = bRect.topRight();

  if((x >= (ll.x() - margin) && x <= (ll.x() + margin))
    && (y >= (ll.y() - margin) && y <= (ll.y() + margin)))
  {
    setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPLowerLeft;
  }
  else if((x >= (lr.x() - margin) && x <= (lr.x() + margin))
    && (y >= (lr.y() - margin) && y <= (lr.y() + margin)))
  {
    setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPLowerRight;
  }
  else if((x >= (tl.x() - margin) && x <= (tl.x() + margin))
    && (y >= (tl.y() - margin) && y <= (tl.y() + margin)))
  {
    setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPTopLeft;
  }
  else if((x >= (tr.x() - margin) && x <= (tr.x() + margin))
    && (y >= (tr.y() - margin) && y <= (tr.y() + margin)))
  {
    setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPTopRight;
  }
  else
  {
    setCursor(Qt::ArrowCursor);
    m_enumSides = TPNoneSide;
    result = false;
  }

  return result;
}

QPixmap te::layout::DefaultTextItem::calculateNewPixmap( const double& x, const double& y )
{
  te::gm::Envelope boxScene = createNewBoxInCoordScene(x, y);
  QPixmap pix = getPixmap().scaled(boxScene.getWidth(), boxScene.getHeight());

  if(pix.isNull())
    pix = getPixmap();

  return pix;
}

te::gm::Envelope te::layout::DefaultTextItem::createNewBoxInCoordScene( const double& x, const double& y )
{
  QPointF pointScene = this->scenePos();

  QPointF pbxy1 = mapToScene(boundingRect().bottomLeft());
  QPointF pbxy2 = mapToScene(boundingRect().topRight());

  double dx = 0;
  double dy = 0;

  if(m_mousePressedCtrl && m_toResizeItem)
  {
    double dx = m_finalCoord.x() - m_initialCoord.x();
    double dy = m_finalCoord.y() - m_initialCoord.y();

    switch(m_enumSides)
    {
    case TPLowerRight :
      {
        if(m_finalCoord.x() > pbxy1.x() && m_finalCoord.y() > pbxy2.y())
        {
          m_boxCopy = te::gm::Envelope(m_model->getBox().m_llx, 
            m_model->getBox().m_lly, m_model->getBox().m_urx + dx, m_model->getBox().m_ury + dy);

          //In Parent Coordinates
          setPos(QPointF(m_model->getBox().m_llx, m_model->getBox().m_lly));
        }
        break;
      }
    case TPLowerLeft:
      {
        if(m_finalCoord.x() < pbxy2.x() && m_finalCoord.y() > pbxy2.y())
        {
          m_boxCopy = te::gm::Envelope(m_model->getBox().m_llx + dx, 
            m_model->getBox().m_lly - dy, m_model->getBox().m_urx, m_model->getBox().m_ury);

          //In Parent Coordinates
          setPos(QPointF(m_finalCoord.x(), m_model->getBox().m_lly));
        }
        break;
      }
    case TPTopRight:
      {
        if(m_finalCoord.x() > pbxy1.x() && m_finalCoord.y() < pbxy1.y())
        {
          m_boxCopy = te::gm::Envelope(m_model->getBox().m_llx, 
            m_model->getBox().m_lly, m_model->getBox().m_urx + dx, m_model->getBox().m_ury - dy);

          //In Parent Coordinates
          setPos(QPointF(m_model->getBox().m_llx, m_finalCoord.y()));
        }
        break;
      }
    case TPTopLeft :
      {  
        if(m_finalCoord.x() < pbxy2.x() && m_finalCoord.y() < pbxy1.y())
        {
          m_boxCopy = te::gm::Envelope(m_model->getBox().m_llx + dx, 
            m_model->getBox().m_lly + dy, m_model->getBox().m_urx, m_model->getBox().m_ury);

          //In Parent Coordinates
          setPos(QPointF(m_finalCoord.x(), m_finalCoord.y()));
        }
        break;
      }
    default :
      {
        m_boxCopy = m_model->getBox();
        break;
      }
    }
  }

  return m_boxCopy;
}

void te::layout::DefaultTextItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
  QGraphicsItem::mousePressEvent(event);

  if(event->modifiers() == Qt::AltModifier && m_toResizeItem)
  {
    m_clonePixmap = getPixmap();
    m_mousePressedCtrl = true;
    m_initialCoord = event->scenePos();
  }
}

void te::layout::DefaultTextItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
  QGraphicsItem::mouseReleaseEvent(event);

  m_finalCoord = event->scenePos();

  te::gm::Envelope boxScene = createNewBoxInCoordScene(event->scenePos().x(), event->scenePos().y());
  if(boxScene.isValid() && boxScene.getWidth() > 0 && boxScene.getHeight() > 0)
    m_controller->setBox(boxScene);

  m_mousePressedCtrl = false;

  if(m_toResizeItem && boxScene.isValid())
  {
    m_toResizeItem = false;
    //Antes é necessário saber se o pixmap continua o mesmo, ou foi modificado.
    //Só chamará o redraw se foi modificado.

    redraw();
    setOpacity(1.);
  }

  refresh();
}

void te::layout::DefaultTextItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
  if(event->modifiers() == Qt::AltModifier && event->buttons() == Qt::LeftButton && m_toResizeItem)
  {
    m_mousePressedCtrl = true;
    setOpacity(0.5);

    m_finalCoord = event->scenePos();

    QPixmap pix = calculateNewPixmap(event->scenePos().x(), event->scenePos().y());
    setPixmap(pix);
    update();
  }
  else
  {
    if(!m_toResizeItem)
      setOpacity(1.);
    m_mousePressedCtrl = false;
    QGraphicsItem::mouseMoveEvent(event);
  }
}

void te::layout::DefaultTextItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
  QGraphicsItem::hoverEnterEvent(event);
}

void te::layout::DefaultTextItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
  m_hoverAboveItem = false;
  setCursor(Qt::ArrowCursor);
  QGraphicsItem::hoverLeaveEvent(event);
}

void te::layout::DefaultTextItem::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
  m_hoverAboveItem = true;
  m_toResizeItem = checkTouchesCorner(event->pos().x(), event->pos().y());
  QGraphicsItem::hoverMoveEvent(event);
}

te::color::RGBAColor** te::layout::DefaultTextItem::getImage()
{
  QImage img = m_pixmap.toImage();
  te::color::RGBAColor** teImg = te::qt::widgets::GetImage(&img);
  return teImg;
}
