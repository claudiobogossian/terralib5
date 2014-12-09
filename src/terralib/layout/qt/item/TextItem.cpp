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
  \file TextItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TextItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../core/Scene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../item/TextModel.h"

// STL
#include <string>

// Qt
#include <QTextDocument>
#include <QStyleOptionGraphicsItem>
#include <QTextCursor>
#include <QAbstractTextDocumentLayout>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QList>

te::layout::TextItem::TextItem( ItemController* controller, Observable* o ) :
  QGraphicsTextItem(0),
  ItemObserver(controller, o),
  m_document(0),
  m_editable(false),
  m_move(false)
{  
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges
    | QGraphicsItem::ItemIsFocusable);

  m_invertedMatrix = true;

  QGraphicsItem* item = this;
  m_nameClass = std::string(this->metaObject()->className());
  Context::getInstance().getScene()->insertItem((ItemObserver*)item);

  m_document = new QTextDocument;
  setDocument(m_document);

  m_backgroundColor.setAlpha(0);

  init();
  
  //If enabled is true, this item will accept hover events
  setTextInteractionFlags(Qt::TextEditable);
}

te::layout::TextItem::~TextItem()
{
  if(m_document)
  {
    delete m_document;
    m_document = 0;
  }
}

void te::layout::TextItem::init()
{
  QFont ft("Arial", 12);
  m_document->setDefaultFont(ft);

  std::string name = m_model->getName();
  TextModel* model = dynamic_cast<TextModel*>(m_model);
  if(model)
  {
    if(model->getText().compare("") == 0)
    {
      model->setText(name);
    }

    QTextCursor cursor(getDocument());
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText(name.c_str());

    std::string txt = model->getText();
    m_document->setPlainText(txt.c_str());
  }
}

void te::layout::TextItem::updateObserver( ContextItem context )
{
  if(!m_model)
    return;

  if(!m_document)
    return;

  TextModel* model = dynamic_cast<TextModel*>(m_model);

  if(!model)
    return;

  te::color::RGBAColor clrBack = model->getBackgroundColor();
  m_backgroundColor.setRed(clrBack.getRed());
  m_backgroundColor.setGreen(clrBack.getGreen());
  m_backgroundColor.setBlue(clrBack.getBlue());
  m_backgroundColor.setAlpha(clrBack.getAlpha());

  Font ft = model->getFont();

  QFont qft;
  qft.setFamily(ft.getFamily().c_str());
  qft.setBold(ft.isBold());
  qft.setItalic(ft.isItalic());
  qft.setPointSizeF(ft.getPointSize());
  qft.setKerning(ft.isKerning());
  qft.setStrikeOut(ft.isStrikeout());
  qft.setUnderline(ft.isUnderline());

  m_document->setDefaultFont(qft);
  
  refreshDocument();

  update();
}

void te::layout::TextItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground( painter );
  
  QGraphicsTextItem::paint(painter, option, widget);
       
  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::TextItem::drawBackground( QPainter* painter )
{
  if(!painter)
  {
    return;
  }

  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setRenderHint( QPainter::Antialiasing, true );
  painter->drawRect(QRectF( 0, 0, boundingRect().width(), boundingRect().height()));
  painter->restore();
}

void te::layout::TextItem::drawSelection( QPainter* painter )
{
  if(!painter)
  {
    return;
  }

  painter->save();

  qreal penWidth = painter->pen().widthF();

  const qreal adj = penWidth / 2;
  const QColor fgcolor(0,255,0);
  const QColor backgroundColor(0,0,0);

  QRectF rtAdjusted = boundingRect().adjusted(adj, adj, -adj, -adj);

  QPen penBackground(backgroundColor, 0, Qt::SolidLine);
  painter->setPen(penBackground);
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(rtAdjusted);

  QPen penForeground(fgcolor, 0, Qt::DashLine);
  painter->setPen(penForeground);
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(rtAdjusted);
  painter->restore();
}

QImage te::layout::TextItem::createImage()
{
  TextModel* model = dynamic_cast<TextModel*>(m_model);

  if(!model)
  {
    QImage ig;
    return ig;
  }

  double zoomFactor = Context::getInstance().getZoomFactor();
  QFont ft = m_document->defaultFont();

  if(zoomFactor > 1.)
  {
    Font fot = model->getFont();

    int pts = (int)fot.getPointSize() * zoomFactor;
    ft.setPointSize(pts);
    m_document->setDefaultFont(ft);
  }

  double w = m_document->size().width();
  double h = m_document->size().height();
  
  QImage img(w, h, QImage::Format_ARGB32_Premultiplied);
  img.fill(m_backgroundColor);

  QPainter ptr(&img);
  ptr.setFont(ft);
  ptr.setRenderHint(QPainter::Antialiasing, true);

  m_document->drawContents(&ptr);

  return img;
}

void te::layout::TextItem::refreshDocument()
{
  if(!m_model)
    return;

  TextModel* model = dynamic_cast<TextModel*>(m_model);

  if(!model)
    return;

  double w = 0;
  double h = 0;

  getDocumentSizeMM(w, h);

  QPointF pp = scenePos();
  
  double x1 = pp.x();
  double y1 = pp.y() - h;
  double x2 = x1 + w;
  double y2 = y1 + h;
  
  te::gm::Envelope box(x1, y1, x2, y2);

  model->setBox(box);
  model->setText(m_document->toPlainText().toStdString());
}

QTextDocument* te::layout::TextItem::getDocument()
{
  return m_document;
}

te::gm::Coord2D te::layout::TextItem::getPosition()
{
  QPointF posF = scenePos();

  double w = 0;
  double h = 0;

  getDocumentSizeMM(w, h);  
  qreal valuex = posF.x();
  qreal valuey = posF.y() - h;

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

te::color::RGBAColor** te::layout::TextItem::getImage()
{
  QImage img = createImage();
  te::color::RGBAColor** teImg = te::qt::widgets::GetImage(&img);
  return teImg;
}

int te::layout::TextItem::getZValueItem()
{
  return QGraphicsItem::zValue();
}

void te::layout::TextItem::applyRotation()
{

}

QVariant te::layout::TextItem::itemChange( GraphicsItemChange change, const QVariant & value )
{
  if(change == QGraphicsItem::ItemPositionChange && !m_move)
  {
    // value is the new position.
    QPointF newPos = value.toPointF();
    double w = 0;
    double h = 0;
    getDocumentSizeMM(w, h);

    newPos.setX(newPos.x() - transform().dx());
    newPos.setY(newPos.y() - transform().dy() + h);
    return newPos;
  }
  if(change == QGraphicsItem::ItemPositionHasChanged)
  {
    refresh();
    m_move = false;
  }

  return QGraphicsTextItem::itemChange(change, value);
}

void te::layout::TextItem::keyPressEvent( QKeyEvent * event )
{
  if((event->modifiers() == Qt::AltModifier) & (event->key() == Qt::Key_E))
  {
    if(m_editable == false)
    {
      m_editable = true;
    }
    else
    {
      m_editable = false;
      refreshDocument();
      /*Necessary clear the selection and focus of the edit 
      after being completely closed and like this not cause bad behavior.*/
      QTextCursor cursor(textCursor());
      cursor.clearSelection();
      setTextCursor(cursor);
      clearFocus();     
      setSelected(true);
    }
  }
  else
  {
    if(m_editable == true)
      QGraphicsTextItem::keyPressEvent(event);
  }
}

void te::layout::TextItem::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
  if(m_editable == false)
  {
    QGraphicsItem::mouseDoubleClickEvent(event);
    return;
  }
  QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void te::layout::TextItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
  m_move = true;
  if(m_editable == false)
  {
    QGraphicsItem::mouseMoveEvent(event);
    return;
  }  
  QGraphicsTextItem::mouseMoveEvent(event);
}

void te::layout::TextItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
  if(m_editable == false)
  {
    QGraphicsItem::mousePressEvent(event);
    return;
  }
  QGraphicsTextItem::mousePressEvent(event);
}

void te::layout::TextItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
  if(m_editable == false)
  {
    QGraphicsItem::mouseReleaseEvent(event);
    return;
  }
  QGraphicsTextItem::mouseReleaseEvent(event);
}

bool te::layout::TextItem::isEditable()
{
  return m_editable;
}

void te::layout::TextItem::getDocumentSizeMM( double &w, double &h )
{
  QImage img = createImage();
  w = img.widthMM();
  h = img.heightMM();
}
