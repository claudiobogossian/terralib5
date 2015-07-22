/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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

// Qt
#include <QTransform>
#include <QPainter>

te::layout::VerticalRuler::VerticalRuler()
{
  
}

te::layout::VerticalRuler::~VerticalRuler()
{

}

void te::layout::VerticalRuler::drawRuler( QGraphicsView* view, QPainter* painter, double scale )
{
  if(!painter)
    return;

  if(!view)
    return;

  if(!m_visible)
    return;

  PaperConfig* paperConfig = Context::getInstance().getPaperConfig();
  if(!paperConfig)
    return;

  QBrush brush = painter->brush();
  
  QPen pen(m_penColor);
  
  double zoomFactor = 1. / scale; //Keeps the appearance of the ruler to 100%
  
  QPointF ll = view->mapToScene(0, view->height());
  QPointF ur = view->mapToScene(view->width(), 0);

  double h = 0;
  double w = 0;

  paperConfig->getPaperSize(w, h);
  
  //Vertical Ruler
  QRectF rfV(QPointF(ll.x(), ll.y()), QPointF(ll.x() + m_height * zoomFactor, ur.y()));
  QRectF rfBackV(QPointF(ll.x(), ll.y()), QPointF(ll.x() + (m_cornerSize - 1.5) * zoomFactor, ur.y() - ((m_height) * zoomFactor)));
  QRectF rfPaperV(QPointF(ll.x(), 0), QPointF(ll.x() + (m_cornerSize - 1.5) * zoomFactor, h));
  QLineF rfLineV(QPointF(ll.x() + m_cornerSize * zoomFactor, ll.y()), QPointF(ll.x() + m_height * zoomFactor, ur.y() - (m_height * zoomFactor)));

  //Rect corner
  QRectF rfRectCorner(QPointF(ll.x(), ur.y()), QPointF(ll.x() + m_cornerSize * zoomFactor, ur.y() - m_height * zoomFactor));

  painter->save();
  painter->setPen(Qt::NoPen);
  
  //Vertical Ruler
  painter->setBrush(m_backgroundRulerColor);
  painter->drawRect(rfV);

  painter->setBrush(m_middleRulerColor);
  painter->drawRect(rfBackV);

  painter->setBrush(m_frontColor);
  painter->drawRect(rfPaperV);

  painter->setBrush(brush);

  pen.setWidth(m_penWidth);
  painter->setPen(pen);

  painter->drawLine(rfLineV);

  drawMarks(view, painter, rfBackV, zoomFactor);

  painter->setBrush(m_backgroundRulerColor);
  painter->setPen(Qt::NoPen);
  painter->drawRect(rfRectCorner);

  painter->restore();
}

void te::layout::VerticalRuler::drawMarks( QGraphicsView* view, QPainter* painter, QRectF rect, double zoomFactor )
{
  Utils* utils = Context::getInstance().getUtils();

  if(!utils)
  {
    return;
  }

  painter->save();

  painter->setFont(m_font);

  double wtxt = 0;
  double htxt = 0;

  double urx = rect.topRight().x();
  double ury = rect.bottomLeft().y();
  double lly = rect.topRight().y();

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

      utils->textBoundingBox(wtxt, htxt, ss.str()); // size in mm

      QPointF pTranslate(x - m_spacingLineText * zoomFactor, i);

      QPointF p1 = view->mapFromScene(pTranslate);
      p1.setY(p1.y() + 1 + (wtxt/4.));

      painter->save();

      QTransform m;
      m.translate( p1.x(), p1.y() );
      m.rotate(-90);
      m.translate( -p1.x(), -p1.y() );

      painter->setMatrixEnabled(false);
      painter->setTransform(m);
      painter->setFont(m_font);
      painter->drawText(p1, ss.str().c_str());

      painter->restore();
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

  painter->restore();
}
