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
  \file HorizontalRulerModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "HorizontalRuler.h"
#include "../../core/PaperConfig.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/Utils.h"

// STL
#include <sstream>

te::layout::HorizontalRuler::HorizontalRuler(PaperConfig* paperConfig) :
  AbstractRuler(paperConfig)
{
 
}

te::layout::HorizontalRuler::~HorizontalRuler()
{

}

void te::layout::HorizontalRuler::drawRuler( QGraphicsView* view, QPainter* painter )
{
  if(!painter)
    return;

  if(!view)
    return;

  if(!m_visible)
    return;

  if(!m_paperConfig)
    return;

  QBrush brush = painter->brush();

  QBrush bhWhite(QColor(255,255,255,255));
  QBrush bhGreyBack(QColor(145,145,145,255));
  QBrush bhGreyBox(QColor(180,180,180,255));

  QPen pen(QColor(0,0,0,255));
  
  double zoomFactor = Context::getInstance().getZoomFactor();
  double zoom = Context::getInstance().getZoomFactor();
  zoomFactor = 1. / zoomFactor; //Keeps the appearance of the ruler to 100%

  QPointF ll = view->mapToScene(0, 0);
  QPointF ur = view->mapToScene(view->width(), view->height());
  
  double h = 0;
  double w = 0;

  m_paperConfig->getPaperSize(w, h);

  //Horizontal Ruler
  QRectF rfH(QPointF(ll.x(), ll.y()), QPointF(ur.x(), ll.y() + m_height * zoomFactor));
  QRectF rfBackH(QPointF(ll.x() + (m_cornerSize * zoomFactor), ll.y()), QPointF(ur.x(), ll.y() + (m_height - 1.5) * zoomFactor));
  QRectF rfPaperH(QPointF(0, ll.y()), QPointF(w, ll.y() + (m_height - 1.5) * zoomFactor));
  QLineF rfLineH(QPointF(ll.x() + (m_cornerSize * zoomFactor), ll.y() + m_height * zoomFactor), QPointF(ur.x(), ll.y() + m_height * zoomFactor));
  
  //Rect corner
  QRectF rfRectCorner(QPointF(ll.x(), ll.y()), QPointF(ll.x() + m_cornerSize * zoomFactor, ll.y() + m_height * zoomFactor));

  painter->save();
  painter->setPen(Qt::NoPen);

  //Horizontal Ruler
  painter->setBrush(bhGreyBox);
  painter->drawRect(rfH);

  painter->setBrush(bhGreyBack);
  painter->drawRect(rfBackH);

  painter->setBrush(bhWhite);
  painter->drawRect(rfPaperH);

  painter->setBrush(brush);
  painter->setPen(pen);

  painter->drawLine(rfLineH);
  
  Utils* utils = Context::getInstance().getUtils();

  double wtxt = 0;
  double htxt = 0;

  QFont ft("Arial");
  ft.setPointSizeF(6);
  painter->setFont(ft);

  std::stringstream ss;

  double llx = rfBackH.bottomLeft().x();
  double lly = rfBackH.bottomLeft().y();
  double urx = rfBackH.topRight().x();

  double y = lly;

  //Horizontal Ruler Marks
  QLineF box;

  for(int i = (int)llx ; i < (int) urx ; ++i)
  {
    if((i % (int)(m_blockSize)) == 0)
    {
      box = QLineF(QPointF(i, y), QPointF(i, y - m_longLine * zoomFactor));  

      std::stringstream ss;//create a stringstream
      ss << i;//add number to the stream

      utils->textBoundingBox(wtxt, htxt, ss.str());

      QPoint p = view->mapFromScene(QPointF((double)i - (wtxt/2.), y - m_spacingLineText * zoomFactor));

      //Keeps the size of the text.(Aspect)
      painter->setMatrixEnabled(false);
      painter->drawText(p, ss.str().c_str());
      painter->setMatrixEnabled(true);
    }
    else if((i % (int)(m_middleBlockSize)) == 0)
    {
      box = QLineF(QPointF(i, y), QPointF(i, y - m_mediumLine * zoomFactor)); 
    }
    else if((i % (int)(m_smallBlockSize)) == 0)
    {
      box = QLineF(QPointF(i, y), QPointF(i, y - m_smallLine * zoomFactor));  
    }

    painter->drawLine(box);
  }

  painter->setBrush(bhGreyBox);
  painter->setPen(Qt::NoPen);
  painter->drawRect(rfRectCorner);

  painter->restore();
}
