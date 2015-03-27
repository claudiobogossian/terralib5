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
  \file ItemGroup.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PaperItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../core/Scene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/PaperModel.h"

te::layout::PaperItem::PaperItem( ItemController* controller, Observable* o ) :
  ObjectItem(controller, o)
{  

  this->setFlags(QGraphicsItem::ItemSendsGeometryChanges);

  m_canChangeGraphicOrder = false;

  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::PaperItem::~PaperItem()
{

}

void te::layout::PaperItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground( painter );

  drawPaper(painter);

  drawBorder(painter);

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::PaperItem::drawPaper( QPainter * painter )
{
  PaperModel* model = dynamic_cast<PaperModel*>(m_model);
  if(!model)
  {
    return;
  }

  QRectF boundRect;
  boundRect = boundingRect();

  painter->save();

  te::color::RGBAColor clrPaper = model->getPaperColor();
  QColor cback;
  cback.setRed(clrPaper.getRed());
  cback.setGreen(clrPaper.getGreen());
  cback.setBlue(clrPaper.getBlue());
  cback.setAlpha(clrPaper.getAlpha());

  te::color::RGBAColor clrShadow = model->getShadowColor();
  QColor cShadow;
  cShadow.setRed(clrShadow.getRed());
  cShadow.setGreen(clrShadow.getGreen());
  cShadow.setBlue(clrShadow.getBlue());
  cShadow.setAlpha(clrShadow.getAlpha());

  te::color::RGBAColor clrBorder = model->getBorderColor();
  QColor cBorder;
  cBorder.setRed(clrBorder.getRed());
  cBorder.setGreen(clrBorder.getGreen());
  cBorder.setBlue(clrBorder.getBlue());
  cBorder.setAlpha(clrBorder.getAlpha());

  QBrush bsh(cShadow);
  painter->setBrush(bsh);
  QPen pen(cBorder);
  painter->setPen(pen);
  
  if(!model->getPaperConfig())
    return;

  double pw = 0.;
  double ph = 0.;

  model->getPaperConfig()->getPaperSize(pw, ph);

  double shadowPadding = model->getShadowPadding();

  QRectF boxShadow(boundRect.x() + shadowPadding, boundRect.y(), boundRect.width() - shadowPadding, boundRect.height() - shadowPadding);
  painter->drawRect(boxShadow);
  
  bsh.setColor(cback);
  painter->setBrush(bsh);

  QRectF boxPaper = QRectF(boundRect.x(), boundRect.y() + shadowPadding, boundRect.width() - shadowPadding, boundRect.height() - shadowPadding);
  painter->drawRect(boxPaper);

  painter->restore();  
}
