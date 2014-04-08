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
  \file QDisplayWindowOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "QDisplayWindowOutside.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "QLayoutScene.h"
#include "LayoutOutsideModelObservable.h"
#include "LayoutItemObserver.h"
#include "LayoutOutsideObserver.h"
#include "LayoutOutsideController.h"
#include "../../../../geometry/Envelope.h"

// Qt
#include <QGraphicsWidget>
#include <QMainWindow>

te::layout::QDisplayWindowOutside::QDisplayWindowOutside( LayoutOutsideController* controller, LayoutObservable* o ) :
	QDockWidget("", 0, 0),
	LayoutOutsideObserver(controller, o),
  m_previousCentralWidget(0)
{  
	setVisible(false);
	setWindowTitle("Layout Display");
}

te::layout::QDisplayWindowOutside::~QDisplayWindowOutside()
{

}

void te::layout::QDisplayWindowOutside::updateObserver( ContextLayoutItem context )
{
	setVisible(context.getShow());
	if(context.getShow() == true)
		show();
	else
		hide();
}

void te::layout::QDisplayWindowOutside::setPosition( const double& x, const double& y )
{
	move(x,y);
	refresh();
}

te::gm::Coord2D te::layout::QDisplayWindowOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::QDisplayWindowOutside::closeEvent( QCloseEvent * event )
{
  QDockWidget::closeEvent(event);

  widget()->close();

  QMainWindow* mw = (QMainWindow*)parentWidget();

  if(mw)
  {
    mw->removeDockWidget(this);
    if(m_previousCentralWidget)
    {
      QDockWidget* dock = dynamic_cast<QDockWidget*>(m_previousCentralWidget);
      mw->setCentralWidget(dock);
      dock->setVisible(m_previousCentralWidgetVisibilite);
    }
  }
}

void te::layout::QDisplayWindowOutside::setPreviousCentralWidget( QWidget* previous )
{
  m_previousCentralWidget = previous;
}
