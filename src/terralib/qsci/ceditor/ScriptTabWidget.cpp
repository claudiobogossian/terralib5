/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file ScriptTabWidget.cpp

  \brief The main tab widget may be used to display several scripts.
*/

// TerraLib Code Editor
#include "ScriptTabWidget.h"
#include "ScriptWidget.h"

// Qt
#include <QMenu>
#include <QMouseEvent>

te::ce::ScriptTabWidget::ScriptTabWidget(QWidget* parent)
  : QTabWidget(parent)
{
}

te::ce::ScriptTabWidget::~ScriptTabWidget()
{
}

//te::ce::Application* te::ce::ScriptTabWidget::getApplication() const
//{
//  return m_parent;
//}

void te::ce::ScriptTabWidget::mousePressEvent(QMouseEvent* evt)
{
  if(evt->button() == Qt::RightButton)
  {
    QMenu m(this);

//    if(m_parent->isFullScreen())
    {
      QAction* viewNormal = new QAction(tr("&View Normal"), this);
      
      m.addAction(viewNormal);

//      connect(viewNormal, SIGNAL(triggered()), m_parent, SLOT(viewNormal()));
    }

    //m.popup(evt->pos());
    m.exec(evt->globalPos());
  }

  QTabWidget::mousePressEvent(evt);
}

