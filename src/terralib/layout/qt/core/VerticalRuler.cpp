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
  \file VerticalRuler.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "VerticalRuler.h" 
#include "../../core/PaperConfig.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/Utils.h"

// STL
#include <sstream>

te::layout::VerticalRuler::VerticalRuler(PaperConfig* paperConfig):
  AbstractRuler(paperConfig)
{
  
}

te::layout::VerticalRuler::~VerticalRuler()
{

}

void te::layout::VerticalRuler::drawRuler( QGraphicsView* view, QPainter* painter )
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
  
  //Vertical Ruler
  QRectF rfV(QPointF(ll.x(), ll.y()), QPointF(ll.x() + m_height * zoomFactor, ur.y()));
  QRectF rfBackV(QPointF(ll.x(), ll.y() + ((m_height) * zoomFactor)), QPointF(ll.x() + (m_cornerSize - 1.5) * zoomFactor, ur.y()));
  QRectF rfPaperV(QPointF(ll.x(), 0), QPointF(ll.x() + (m_cornerSize - 1.5) * zoomFactor, h));
  QLineF rfLineV(QPointF(ll.x() + m_cornerSize * zoomFactor, ll.y() + (m_height * zoomFactor)), QPointF(ll.x() + m_height * zoomFactor, ur.y()));

  //Rect corner
  QRectF rfRectCorner(ll.x(), ll.y(), m_cornerSize * zoomFactor, m_height * zoomFactor);

  painter->save();
  painter->setPen(Qt::NoPen);
  
  //Vertical Ruler
  painter->setBrush(bhGreyBox);
  painter->drawRect(rfV);

  painter->setBrush(bhGreyBack);
  painter->drawRect(rfBackV);

  painter->setBrush(bhWhite);
  painter->drawRect(rfPaperV);

  painter->setBrush(brush);
  painter->setPen(pen);

  painter->drawLine(rfLineV);

  Utils* utils = Context::getInstance().getUtils();

  double wtxt = 0;
  double htxt = 0;

  QFont ft("Arial");
  ft.setPointSizeF(3);
  painter->setFont(ft);

  double urx = rfBackV.topRight().x();
  double ury = rfBackV.bottomLeft().y();
  double lly = rfBackV.topRight().y();
  
  double x = urx;

  //Horizontal Ruler Marks
  QLineF box;

  for(int i = (int)lly ; i < (int) ury ; ++i)
  {
    if((i % (int)m_blockSize) == 0)
    {
      box = QLineF(QPointF(x, i), QPointF(x - m_longLine * zoomFactor, i));

      std::stringstream ss;//create a stringstream
      ss << i;//add number to the stream

      utils->textBoundingBox(wtxt, htxt, ss.str());
      QPointF p1(x - m_spacingLineText, (double)i - (wtxt/2.));
      painter->save();
      painter->translate(p1);
      painter->rotate(-90);
      painter->drawText(QPointF(- wtxt, 0), ss.str().c_str());
      painter->restore();
      painter->setBrush(brush);
      painter->setPen(pen);
      painter->setFont(ft);
    }
    else if((i % (int)m_middleBlockSize) == 0)
    {
      box = QLineF(QPointF(x, i), QPointF(x - m_mediumLine * zoomFactor, i));
    }
    else if((i % (int)m_smallBlockSize) == 0)
    {
      box = QLineF(QPointF(x, i), QPointF(x - m_smallLine * zoomFactor, i));
    }

    painter->drawLine(box);
  }

  painter->setBrush(bhGreyBox);
  painter->setPen(Qt::NoPen);
  painter->drawRect(rfRectCorner);

  painter->restore();
}
