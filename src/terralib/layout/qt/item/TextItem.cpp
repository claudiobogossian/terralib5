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

#include <QGraphicsView>
#include <QList>

te::layout::TextItem::TextItem( ItemController* controller, Observable* o ) :
  ObjectItem(controller, o),
  m_document(0)
{  
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges
    | QGraphicsItem::ItemIsFocusable);

  QGraphicsItem* item = this;
  m_nameClass = std::string(this->metaObject()->className());
  Context::getInstance().getScene()->insertItem((ItemObserver*)item);

  m_document = new QTextDocument;

  QFont ft("Arial", 12);
  m_document->setDefaultFont(ft);

  std::string name = m_model->getName();
  TextModel* model = dynamic_cast<TextModel*>(m_model);
  if(model)
  {
    model->setText(name);
  }

  //If enabled is true, this item will accept hover events
  setAcceptHoverEvents(true);
  
  QTextCursor cursor(getDocument());
  cursor.movePosition(QTextCursor::Start);
  cursor.insertText(name.c_str());

  m_document->setPlainText("TESTANDO!");
}

te::layout::TextItem::~TextItem()
{
  if(m_document)
  {
    delete m_document;
    m_document = 0;
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
  
  double sizeW = 0;
  double sizeH = 0;

  if(m_document)
  {
    sizeW = m_document->size().width();
    sizeH = m_document->size().height();
  }

  if(sizeW != m_pixmap.width() || sizeH != m_pixmap.height())
  {
    refreshDocument();
  }

  QRectF boundRect;
  boundRect = boundingRect();

  painter->save();
  painter->translate( -boundRect.bottomLeft().x(), -boundRect.topRight().y() );
  painter->drawPixmap(boundRect, m_pixmap, QRectF( 0, 0, m_pixmap.width(), m_pixmap.height() ));
  painter->restore();
  
  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

QImage te::layout::TextItem::createImage()
{
  QImage img(m_document->size().width(), m_document->size().height(), QImage::Format_ARGB32);
  img.fill(m_backgroundColor);

  QPainter ptr(&img);
  ptr.setFont(m_document->defaultFont());
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

  QImage img = createImage();
  QPointF pp = scenePos();
  te::gm::Envelope box(pp.x(), pp.y(), pp.x() + img.widthMM(), pp.y() + img.heightMM());

  model->setBox(box);
  model->setText(m_document->toPlainText().toStdString());

  QPixmap pixmp = QPixmap::fromImage(img);
  setPixmap(pixmp);
}

QTextDocument* te::layout::TextItem::getDocument()
{
  return m_document;
}
