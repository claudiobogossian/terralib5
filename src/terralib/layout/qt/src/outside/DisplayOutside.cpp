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
  \file DisplayOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "DisplayOutside.h"
#include "Context.h"
#include "AbstractScene.h"
#include "Scene.h"
#include "OutsideModelObservable.h"
#include "ItemObserver.h"
#include "OutsideObserver.h"
#include "OutsideController.h"
#include "../../../../geometry/Envelope.h"

// Qt
#include <QGraphicsWidget>
#include <QMainWindow>

te::layout::DisplayOutside::DisplayOutside( OutsideController* controller, Observable* o ) :
	QDockWidget("", 0, 0),
	OutsideObserver(controller, o),
  m_previousCentralWidget(0)
{  
	setVisible(false);
	setWindowTitle("Layout Display");
}

te::layout::DisplayOutside::~DisplayOutside()
{

}

void te::layout::DisplayOutside::updateObserver( ContextItem context )
{
	setVisible(context.getShow());
	if(context.getShow() == true)
		show();
	else
		hide();
}

void te::layout::DisplayOutside::setPosition( const double& x, const double& y )
{
	move(x,y);
	refresh();
}

te::gm::Coord2D te::layout::DisplayOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::DisplayOutside::closeEvent( QCloseEvent * event )
{
  QDockWidget::closeEvent(event);

  if(widget())
  {
    widget()->close();
  }

  removeDock();
}

void te::layout::DisplayOutside::setPreviousCentralWidget( QWidget* previous )
{
  m_previousCentralWidget = previous;
}

void te::layout::DisplayOutside::removeDock()
{
  QMainWindow* mw = (QMainWindow*)parentWidget();

  if(mw)
  {
    mw->removeDockWidget(this);
    if(m_previousCentralWidget)
    {
      QDockWidget* dock = dynamic_cast<QDockWidget*>(m_previousCentralWidget);
      if(dock)
      {
        mw->setCentralWidget(dock);
        dock->setVisible(m_previousCentralWidgetVisibilite);
      }
    }
  }
}
