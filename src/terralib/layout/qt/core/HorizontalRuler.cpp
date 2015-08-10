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
  \file HorizontalRulerModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "HorizontalRuler.h"
#include "../../core/PaperConfig.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/Utils.h"
#include "Scene.h"

// STL
#include <sstream>

te::layout::HorizontalRuler::HorizontalRuler() 
{
 
}

te::layout::HorizontalRuler::~HorizontalRuler()
{

}

void te::layout::HorizontalRuler::drawRuler( QGraphicsView* view, QPainter* painter, double scale )
{
  if(!painter)
    return;

  if(!view)
    return;

  if(!m_visible)
    return;

  Scene* sc = searchScene(view);
  if(!sc)
    return;

  PaperConfig* paperConfig = sc->getPaperConfig();
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

  //Horizontal Ruler
  QRectF rfH(QPointF(ll.x(), ur.y()), QPointF(ur.x(), ur.y() - m_height * zoomFactor));
  QRectF rfBackH(QPointF(ll.x() + (m_cornerSize * zoomFactor), ur.y()), QPointF(ur.x(), ur.y() - (m_height - 1.5) * zoomFactor));
  QRectF rfPaperH(QPointF(0, ur.y()), QPointF(w, ur.y() - (m_height - 1.5) * zoomFactor));
  QLineF rfLineH(QPointF(ll.x() + (m_cornerSize * zoomFactor), ur.y() - m_height * zoomFactor), QPointF(ur.x(), ur.y() - m_height * zoomFactor));
  
  //Rect corner
  QRectF rfRectCorner(QPointF(ll.x(), ur.y()), QPointF(ll.x() + m_cornerSize * zoomFactor, ur.y() - m_height * zoomFactor));

  painter->save();
  painter->setPen(Qt::NoPen);

  //Horizontal Ruler
  painter->setBrush(m_backgroundRulerColor);
  painter->drawRect(rfH);

  painter->setBrush(m_middleRulerColor);
  painter->drawRect(rfBackH);

  painter->setBrush(m_frontColor);
  painter->drawRect(rfPaperH);

  painter->setBrush(brush);

  pen.setWidth(m_penWidth);
  painter->setPen(pen);

  painter->drawLine(rfLineH);
  
  drawMarks(view, painter, rfBackH, zoomFactor);

  painter->setBrush(m_backgroundRulerColor);
  painter->setPen(Qt::NoPen);
  painter->drawRect(rfRectCorner);

  painter->restore();
}

void te::layout::HorizontalRuler::drawMarks( QGraphicsView* view, QPainter* painter, QRectF rect, double zoomFactor )
{
  Utils* utils = Context::getInstance().getUtils();

  if(!utils)
  {
    return;
  }

  painter->save();

  double wtxt = 0;
  double htxt = 0;

  painter->setFont(m_font);

  std::stringstream ss;

  double llx = rect.bottomLeft().x();
  double lly = rect.bottomLeft().y();
  double urx = rect.topRight().x();

  double y = lly;

  //Horizontal Ruler Marks
  QLineF box;

  for(int i = (int)llx ; i < (int) urx ; ++i)
  {
    if((i % (int)(m_blockSize)) == 0)
    {
      box = QLineF(QPointF(i, y), QPointF(i, y + m_longLine * zoomFactor));  

      std::stringstream ss;//create a stringstream
      ss << i;//add number to the stream

      utils->textBoundingBox(wtxt, htxt, ss.str());

      QPointF pTranslate((double)i, y + m_spacingLineText * zoomFactor);

      QPoint p = view->mapFromScene(pTranslate);
      p.setX(p.x() - 1 - (wtxt/4.));

      //Keeps the size of the text.(Aspect)
      painter->setMatrixEnabled(false);
      painter->drawText(p, ss.str().c_str());
      painter->setMatrixEnabled(true);
    }
    else if((i % (int)(m_middleBlockSize)) == 0)
    {
      box = QLineF(QPointF(i, y), QPointF(i, y + m_mediumLine * zoomFactor)); 
    }
    else if((i % (int)(m_smallBlockSize)) == 0)
    {
      box = QLineF(QPointF(i, y), QPointF(i, y + m_smallLine * zoomFactor));  
    }

    painter->drawLine(box);
  }

  painter->restore();
}

te::layout::Scene* te::layout::HorizontalRuler::searchScene(QGraphicsView* view)
{
  Scene* sc = 0;
  QGraphicsScene* qScene = view->scene();
  if(!qScene)
  {
    return sc;
  }

  sc = dynamic_cast<Scene*>(qScene);
  return sc;
}
