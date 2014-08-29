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
    | QGraphicsItem::ItemIsFocusable);

  setTextInteractionFlags(Qt::NoTextInteraction);

  QGraphicsItem* item = this;
  Context::getInstance().getScene()->insertItem((ItemObserver*)item);
  
  m_nameClass = std::string(this->metaObject()->className());

  std::string name = m_model->getName();
  DefaultTextModel* model = dynamic_cast<DefaultTextModel*>(m_model);
  if(model)
  {
    model->setText(name);
  }

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
  const QColor fgcolor(255,255,255);
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

  WorldTransformer transf = utils->getTransformGeo(world, viewport);
  transf.setMirroring(true);
  
  m_oldAdjustSizeW = document()->documentLayout()->documentSize().width();
  m_oldAdjustSizeH = document()->documentLayout()->documentSize().height();

  double wx = 0;
  double wy = 0;

  transf.system2Tosystem1(m_oldAdjustSizeW, m_oldAdjustSizeH, wx, wy);

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
  if(model)
  {
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

    document()->clear();
    QTextCursor cursor(document());
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText(model->getText().c_str());
    adjustSize();
  }
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
