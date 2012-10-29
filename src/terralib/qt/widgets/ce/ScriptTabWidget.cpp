/*  Copyright (C) 2011-20011 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraLib Code Editor - A widget component for scripting with TerraLib Platform.

    TerraLib Code Editor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib Code Editor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    Gilberto Ribeiro de Queiroz at <gribeiro@dpi.inpe.br>.
 */

/*!
  \file ScriptTabWidget.cpp

  \brief The main tab widget may be used to display several scripts.
*/

// TerraLib Code Editor
#include "Application.h"
#include "ScriptTabWidget.h"
#include "ScriptWidget.h"

// Qt
#include <QtGui/QMenu>
#include <QtGui/QMouseEvent>

tce::ScriptTabWidget::ScriptTabWidget(Application* parent)
  : QTabWidget(parent),
    m_parent(parent)
{
}

tce::ScriptTabWidget::~ScriptTabWidget()
{
}

tce::Application* tce::ScriptTabWidget::getApplication() const
{
  return m_parent;
}

void tce::ScriptTabWidget::mousePressEvent(QMouseEvent* evt)
{
  if(evt->button() == Qt::RightButton)
  {
    QMenu m(this);

    if(m_parent->isFullScreen())
    {
      QAction* viewNormal = new QAction(tr("&View Normal"), this);
      
      m.addAction(viewNormal);

      connect(viewNormal, SIGNAL(triggered()), m_parent, SLOT(viewNormal()));
    }

    //m.popup(evt->pos());
    m.exec(evt->globalPos());
  }

  QTabWidget::mousePressEvent(evt);
}

