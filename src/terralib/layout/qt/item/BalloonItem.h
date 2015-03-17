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
  \file BalloonItem.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_BALLOON_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_BALLOON_ITEM_H

// TerraLib
#include "ObjectItem.h"
#include <vector>

// Qt
#include <QGraphicsSceneMouseEvent>
#include <QPointF>

namespace te
{
  namespace layout
  {
    class Observable;

    class BalloonItem : public ObjectItem
    {
      public:

        BalloonItem( ItemController* controller, Observable* o );

        virtual ~BalloonItem();

        virtual void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
        
      protected:

        virtual void	mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
        
        virtual void  mousePressEvent ( QGraphicsSceneMouseEvent * event );

        virtual void	mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

        virtual void	hoverEnterEvent ( QGraphicsSceneHoverEvent * event );

        virtual void	hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );

        virtual void	hoverMoveEvent ( QGraphicsSceneHoverEvent * event );

        QPolygonF m_triangleRect; //Poligono que armazena os pontos do triangulo (al�a do Balloon)
        QPointF m_pointOne; //Ponto que armazena o primeiro ponto do triangulo
        QPointF m_pointTwo; //Ponto que armazena o segundo ponto do triangulo
        QPointF m_pointThree; //Ponto que armazena o terceiro ponto do triangulo
        QPointF m_initialPosition; //Ponto que armazena a posi��o inicial ao clicar com o Mouse
        QPointF m_finalPosition; //Ponto que armazena a posi��o do evento de click do Mouse
        QPointF m_initialPoint; //Ponto que armazena a posi��o inicial onde o triangulo come�ar� a ser desenhado
        QPointF m_clikedArea; //Ponto que armazena a posi��o de um click qualquer para verificar se esse ponto est� contido na �rea do triangulo
        QPointF m_zero; //Ponto que armazena o ponto agudo do bal�o (ponta da al�a do balloon)
        bool m_flagReDraw; //Propriedade booleana que ser� verdadeira quando a tecla SHIFT estiver acionada, permitindo o redimensionamento do BoundRect
        bool m_flagArrow; //Propriedade booleana que ser� verdadeira quando a tecla ALT estiver acionada, permitindo arrastar a al�a do bal�o
        bool m_flagArea; //Propriedade booleana que ser� verdadeira quando a for realizado um click sobre a �rea do triangulo
        bool m_flagChangeArrow; //Propriedade booleana que ser� verdadeira quando for movido a al�a do bal�o, permitindo que ela seja redimensionada proportionalmente ap�s movida para outra posi��o
        double m_deltaX; //Propriedade double que guarda a varia��o em X quando a al�a do bal�o � arrastada
        double m_deltaY; //Propriedade double que guarda a varia��o em Y quando a al�a do bal�o � arrastada
        double m_initialRect; //Propriedade double que guarda o tamanho do width do Rect adicionado ao QPainterPath
        double m_boundDeltaX; //Propriedade double que guarda a varia��o em X para redimensionar o BoundRect
        double m_boundDeltaY; //Propriedade double que guarda a varia��o em Y para redimensionar o BoundRect
               
        enum rectDiagonal //Enum que cont�m as diagonais do ret�ngudo do BoundRect, usado para identificar em qual dire��o o redimensionamento acontecer�
        {
          UpLeft, 
          UpRight, 
          DownLeft, 
          DownRight
        };
        rectDiagonal m_size; //Propriedade do tipo rectDiagonal utilizada para acessar as op��es de rectDiagonal
    };
  }
}

#endif
