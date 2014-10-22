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

  te::color::RGBAColor** rgba = context.getPixmap();  

  if(!rgba)
    return;

  Utils* utils = context.getUtils();

  if(!utils)
    return;

  te::gm::Envelope box = utils->viewportBox(m_model->getBox());

  if(!box.isValid())
    return;

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

QTextDocument* te::layout::TextItem::getDocument()
{
  return m_document;
}

void te::layout::TextItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  if(!m_document)
    return;

  drawBackground( painter );

  QRectF boundRect;
  boundRect = boundingRect();

  QSizeF sz = m_document->size();

  /*QPixmap img(sz.width(), sz.height());
  img.fill(Qt::transparent);

  QPainter ptr(&img);
  m_document->drawContents(&ptr);*/
  
  painter->save();
  painter->translate( -boundRect.bottomLeft().x(), -boundRect.topRight().y() );
  painter->drawPixmap(boundRect, m_pixmap, QRectF( 0, 0, m_pixmap.width(), m_pixmap.height() ));
  painter->restore();  
  
  QTransform trm = painter->transform();
  QPointF pV = trm.map(scenePos());
  QFont ft = m_document->defaultFont();
    
  std::string txt = "TEXT_12345";
  
  painter->save();

  QBrush br(Qt::green);
  QPainter ptr1(painter->device());
  ptr1.setBrush(br);
  ptr1.setFont(ft);
  ptr1.drawText(pV.x(), pV.y(), "TESTE_0980980");
  m_document->documentLayout()->setPaintDevice(painter->device());
  QRectF rtf1(pV.x(), pV.y(), sz.width(), sz.height());
  //m_document->drawContents(&ptr1, rtf1);
  m_document->drawContents(&ptr1);

  painter->restore();

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}
