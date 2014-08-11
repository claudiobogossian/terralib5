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
#include <QStyleOptionGraphicsItem>

te::layout::DefaultTextItem::DefaultTextItem( ItemController* controller, Observable* o ) :
  QGraphicsTextItem(0),
  ItemObserver(controller, o)
{  
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges
    | QGraphicsItem::ItemIsFocusable);

  QGraphicsItem* item = this;
  Context::getInstance().getScene()->insertItem((ItemObserver*)item);
  
  m_nameClass = std::string(this->metaObject()->className());

  std::string name = m_model->getName();
  DefaultTextModel* model = dynamic_cast<DefaultTextModel*>(m_model);
  if(model)
  {
    model->setText(name);
  }
  setPlainText(name.c_str());
}

te::layout::DefaultTextItem::~DefaultTextItem()
{

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

    setPlainText(model->getText().c_str());
  }

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
  return m_controller->contains(te::gm::Coord2D(point.x(), point.y()));
}

void te::layout::DefaultTextItem::setPos( const QPointF &pos )
{
  /* The matrix transformation of MapItem object is the inverse of the scene, 
  so you need to do translate when you change the position, since the coordinate 
  must be in the world coordinate. */
  QPointF p1(pos.x() - transform().dx(), pos.y() - transform().dy());

  QGraphicsTextItem::setPos(p1);

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
