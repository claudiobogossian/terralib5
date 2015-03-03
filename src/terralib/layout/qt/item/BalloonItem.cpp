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
  \file RectangleItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BalloonItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"

// STL
#include <algorithm>    // std::max and std::min

// Qt
#include <QTextDocument>
#include <QStyleOptionGraphicsItem>
#include <QTextCursor>
#include <QAbstractTextDocumentLayout>
#include <QGraphicsSceneMouseEvent>
//
#include <QMessageBox>

te::layout::BalloonItem::BalloonItem( ItemController* controller, Observable* o ) :
  ObjectItem(controller, o),
    m_pointOne(0,0),
    m_pointTwo(0,0),
    m_pointThree(0,0),
    m_triangleRect(0),
    m_initialPosition(0,0),
    m_finalPosition(0,0),
    m_initialPoint(0,0),
    m_clikedArea(0,0),
    m_zero(0,0),
    m_flagArrow(0),
    m_flagReDraw(0),
    m_flagArea(0),
    m_flagChangeArrow(0),
    m_deltaX(0),
    m_deltaY(0),
    m_initialRect(0),
    m_boundDeltaX(0),
    m_boundDeltaY(0)
{
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges
    | QGraphicsItem::ItemIsFocusable);
      
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::BalloonItem::~BalloonItem()
{

}

void te::layout::BalloonItem::updateObserver( ContextItem context )
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

void te::layout::BalloonItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /* = 0 */ )
{
  ObjectItem::paint(painter, option, widget);

  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground(painter);
  
  QRectF boundRect(boundingRect().topLeft().x(), boundingRect().topLeft().y(), boundingRect().width()/2, boundingRect().height()/2);
          
  painter->save();
  
  QPainterPath balloonPath;
  
  balloonPath.setFillRule(Qt::WindingFill);
  
  balloonPath.addRect(boundRect.bottomRight().x()/2, boundRect.bottomRight().y()/2, (boundRect.width() + (boundRect.width()*0.5)), (boundRect.height() + (boundRect.height()*0.5)));
  m_initialPoint = QPointF((boundRect.width() - (boundRect.width()*0.5)), (boundRect.height() - (boundRect.height()*0.5))); //Ponto a partir de onde o triangulo será desenhado
  balloonPath.moveTo(m_pointOne.x(), m_pointOne.y());
  //if (m_initialPoint.isNull() || (!m_flagArrow && !m_flagArea))
  //{
  //  m_initialPoint = QPointF((boundRect.width() - (boundRect.width()*0.5)), (boundRect.height() - (boundRect.height()*0.5))); //Ponto a partir de onde o triangulo será desenhado
  //}
  QPointF pontoAtual = balloonPath.currentPosition();

  if (m_flagArrow && m_flagArea) //Se for clicado sobre a área do triangulo, mantendo a tecla ALT pressionado, significa que a alça será movida e entrará nesse If
  {
    m_pointOne.setX(m_initialPoint.x() + m_deltaX);
    m_pointOne.setY((m_initialPoint.y()*2) + m_deltaY);
    m_pointTwo.setX(m_zero.x() + m_deltaX);
    m_pointTwo.setY(m_zero.y() + m_deltaY);
    m_pointThree.setX((m_initialPoint.x()*2) + m_deltaX);
    m_pointThree.setY(m_initialPoint.y() + m_deltaY);

    QPolygonF trianglePolygon;

    trianglePolygon.push_back(m_pointOne);
    trianglePolygon.push_back(m_pointTwo);
    trianglePolygon.push_back(m_pointThree);
    trianglePolygon.push_back(m_pointOne);

    m_triangleRect = trianglePolygon; //Guardando os pontos do triangulo em um poligono

    balloonPath.addPolygon(trianglePolygon); //Adicionando o triangulo no Path para ser desenhado junto ao retangulo e formar o balão

    m_flagArrow = false;
    m_flagArea = false;
  } 
  else //Caso contrário, o balão manterá sua forma padrão. Após algum redimensionamento ou mudança de posição da alça, ele ainda manterá as alterações
  {
    m_pointOne.setX(m_initialPoint.x());
    m_pointOne.setY(m_initialPoint.y()*2);        
    m_pointTwo.setX(m_zero.x());
    m_pointTwo.setY(m_zero.y());
    m_pointThree.setX((m_initialPoint.x()*2));
    m_pointThree.setY(m_initialPoint.y());
                
    QPolygonF trianglePolygon;

    trianglePolygon.push_back(m_pointOne);
    trianglePolygon.push_back(m_pointTwo);
    trianglePolygon.push_back(m_pointThree);
    trianglePolygon.push_back(m_pointOne);
        
    m_triangleRect = trianglePolygon;

    balloonPath.addPolygon(trianglePolygon);
  }
  
  painter->setRenderHint(QPainter::Antialiasing);
  painter->strokePath(balloonPath.simplified(), QPen(Qt::black, .2));
  painter->restore(); 
    
  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::BalloonItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
  QPointF scenePos = mapToScene(event->pos());
  m_initialPosition.setX(scenePos.x());
  m_initialPosition.setY(scenePos.y()); //Pegando coordenadas iniciais do click
}

void te::layout::BalloonItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
  if(event->modifiers() == Qt::ShiftModifier && event->buttons() == Qt::LeftButton && m_toResizeItem) //Redimensionar balão se SHIFT e Botão Esquerdo do mouse estiverem pressionados
  {
    //m_mousePressedCtrl = true;
    m_flagReDraw = true;
    setOpacity(0.5);

    m_finalCoord = event->scenePos();

    QPixmap pix = calculateNewPixmap(event->scenePos().x(), event->scenePos().y());
    setPixmap(pix);
    update();
  }
  else
  {
    if(event->modifiers() == Qt::AltModifier && event->buttons() == Qt::LeftButton /*&& m_toResizeItem*/) //Mover alça do balão se ALT e Botão Esquerdo do mouse estiverem pressionados
    {
      m_flagArrow = true;
      m_flagChangeArrow = true;
      m_clikedArea.setX(event->pos().x());
      m_clikedArea.setY(event->pos().y()); //coordenadas do click a serem verificadas se estão contidas no triangulo da alça do balão
    }
    else
    {
      if(!m_toResizeItem)
        setOpacity(1.);

      //m_mousePressedCtrl = false;
      QGraphicsItem::mouseMoveEvent(event);
    }    
  }
}

void te::layout::BalloonItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{ 
  QPointF scenePos = mapToScene(event->pos());
  m_finalPosition.setX(scenePos.x());
  m_finalPosition.setY(scenePos.y()); //Pegando coordenadas finais do click

  //Adotando sistema de coordenadas do Rect (Inicio (0, 0) em TopRigth e término em BottomLeft (x, y))
  QPointF topLeftBound = mapToParent(boundingRect().bottomLeft());
  QPointF topRightBound = mapToParent(boundingRect().bottomRight());
  QPointF bottomLeftBound = mapToParent(boundingRect().topLeft());
  QPointF bottomRightBound = mapToParent(boundingRect().topRight());    
  
  m_boundDeltaX = m_finalPosition.x() - m_initialPosition.x();
  m_boundDeltaY = m_finalPosition.y() - m_initialPosition.y(); //Calculando a variação de X e Y para poder redimensionar o BoundRect

  if ((m_triangleRect.containsPoint(m_clikedArea, Qt::OddEvenFill))) //Se for clicado na área do triangulo, entrará nesse If
  {   
    m_flagArea = true;
    
    if (m_flagArrow && m_flagArea) //Se for clicado na área do triangulo para mover a alça, calculará a diferença entre o ponto onde houve o Release com o ponto do click inicial
    {
      //Scene p/ Item
      QPointF finalPosition = mapFromScene(m_finalPosition);
      QPointF initialPosition = mapFromScene(m_initialPosition);

      m_deltaX = finalPosition.x() - initialPosition.x();
      m_deltaY = finalPosition.y() - initialPosition.y();
      
      //Atualiza o ponto agudo da alça quando ela for movida
      m_zero.setX(finalPosition.x());
      m_zero.setY(finalPosition.y());
    }    

    m_clikedArea.setX(-1);
    m_clikedArea.setY(-1);
  }
  //Redimensionando
  if (m_size == UpLeft && m_flagReDraw)
  {
    te::gm::Envelope newBound((bottomLeftBound.x()+ m_boundDeltaX), bottomLeftBound.y(), topRightBound.x(), (topRightBound.y()+ m_boundDeltaY));
    m_controller->setBox(newBound);
    QGraphicsItem::setPos((bottomLeftBound.x()+ m_boundDeltaX), bottomLeftBound.y());
    redraw();
  } 
  else
  { 
    if (m_size == UpRight && m_flagReDraw)
    {
      te::gm::Envelope newBound(bottomLeftBound.x(), bottomLeftBound.y(), (topRightBound.x()+ m_boundDeltaX), (topRightBound.y()+ m_boundDeltaY));
      m_controller->setBox(newBound);
      QGraphicsItem::setPos(bottomLeftBound.x(), bottomLeftBound.y());
      redraw();
    }
    else
    {
      if (m_size == DownRight && m_flagReDraw)
      {
        te::gm::Envelope newBound(bottomLeftBound.x(), (bottomLeftBound.y()+ m_boundDeltaY), (topRightBound.x()+ m_boundDeltaX), topRightBound.y());
        m_controller->setBox(newBound);
        QGraphicsItem::setPos(topLeftBound.x(), bottomLeftBound.y()+ m_boundDeltaY);
        redraw();
      }
      else
      {
        if (m_size == DownLeft && m_flagReDraw)
        {
          te::gm::Envelope newBound((bottomLeftBound.x()+ m_boundDeltaX), (bottomLeftBound.y()+ m_boundDeltaY), topRightBound.x(), topRightBound.y());
          m_controller->setBox(newBound);
          QGraphicsItem::setPos((bottomLeftBound.x()+ m_boundDeltaX), (bottomLeftBound.y()+ m_boundDeltaY));
          redraw();
        }
      }
    }
  }
  
  m_flagReDraw = false;
}

void te::layout::BalloonItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
  QGraphicsItem::hoverEnterEvent(event);
}

void te::layout::BalloonItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
  m_hoverAboveItem = false;
  setCursor(Qt::ArrowCursor);
  QGraphicsItem::hoverLeaveEvent(event);
}

void te::layout::BalloonItem::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
  m_hoverAboveItem = true;
  double toleranceX = 2.;
  double toleranceY = 2.;

  //Adotando sistema de coordenadas do Rect (Inicio (0, 0) em TopRigth e término em BottomLeft (x, y))
  QPointF topLeftRect = boundingRect().bottomLeft();
  QPointF topRightRect = boundingRect().bottomRight();
  QPointF bottomLeftRect = boundingRect().topLeft();
  QPointF bottomRightRect = boundingRect().topRight();

  QPointF scenePos = event->pos();
  //Altera seta do Cursor de acordo com a diagonal a qual o Mouse está e altera o Enum para a opsição equivalente, para posterior identificação ao redimensionar o BoundRect
  if (((topLeftRect.x() >= (scenePos.x()-toleranceX)) && (topLeftRect.x() <= (scenePos.x()+toleranceX))) && ((topLeftRect.y() >= (scenePos.y()-toleranceY)) && (topLeftRect.y() <= (scenePos.y()+toleranceY))))
  {
    setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPTopLeft;
    m_size = UpLeft; 
    m_toResizeItem = true;
  }

  if (((topRightRect.x() >= (scenePos.x()-toleranceX)) && (topRightRect.x() <= (scenePos.x()+toleranceX))) && ((topRightRect.y() >= (scenePos.y()-toleranceY)) && (topRightRect.y() <= (scenePos.y()+toleranceY))))
  {
    setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPTopRight;
    m_size = UpRight;
    m_toResizeItem = true;
  }

  if (((bottomRightRect.x() >= (scenePos.x()-toleranceX)) && (bottomRightRect.x() <= (scenePos.x()+toleranceX))) && ((bottomRightRect.y() >= (scenePos.y()-toleranceY)) && (bottomRightRect.y() <= (scenePos.y()+toleranceY))))
  {
    setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPLowerRight;
    m_size = DownRight;
    m_toResizeItem = true;
  }

  if (((bottomLeftRect.x() >= (scenePos.x()-toleranceX)) && (bottomLeftRect.x() <= (scenePos.x()+toleranceX))) && ((bottomLeftRect.y() >= (scenePos.y()-toleranceY)) && (bottomLeftRect.y() <= (scenePos.y()+toleranceY))))
  {
    setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPLowerLeft;
    m_size = DownLeft;
    m_toResizeItem = true;
  }

  QGraphicsItem::hoverMoveEvent(event);  
}
