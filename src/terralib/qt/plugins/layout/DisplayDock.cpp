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
  \file DisplayDock.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "DisplayDock.h"

// Qt
#include <QMainWindow>

te::qt::plugins::layout::DisplayDock::DisplayDock( QWidget * parent, Qt::WindowFlags flags ) :
	QDockWidget(parent, flags),
  m_previousCentralWidget(0),
  m_previousCentralWidgetVisibilite(true)
{  
	setVisible(false);
	setWindowTitle("Layout Display");
}

te::qt::plugins::layout::DisplayDock::~DisplayDock()
{

}

void te::qt::plugins::layout::DisplayDock::closeEvent( QCloseEvent * event )
{
  QDockWidget::closeEvent(event);

  if(widget())
  {
    widget()->close();
  }

  removeDock();
}

void te::qt::plugins::layout::DisplayDock::setPreviousCentralWidget( QWidget* previous )
{
  m_previousCentralWidget = previous;
}

void te::qt::plugins::layout::DisplayDock::removeDock()
{
  QMainWindow* mw = (QMainWindow*)parentWidget();

  if(mw)
  {
    mw->removeDockWidget(this);
	this->setParent(0);
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

void te::qt::plugins::layout::DisplayDock::onExit()
{
  close();
}
