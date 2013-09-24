/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/canvas/TextFrame.h

  \brief The frame data....
*/

// TerraLib
#include "DraftLayoutEditor.h"

#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <QtGui/QMouseEvent>
#include <QtCore/QRect>

te::qt::widgets::DraftLayoutEditor::DraftLayoutEditor(QWidget* parent, Qt::WindowFlags f) :
  QWidget(parent, f)
{
  setMouseTracking(true);
  show();
  installEventFilter(this);
}

te::qt::widgets::DraftLayoutEditor::~DraftLayoutEditor()
{
}

QPixmap* te::qt::widgets::DraftLayoutEditor::getDraftPixmap()
{
  return &m_draftPixmap;
}

void te::qt::widgets::DraftLayoutEditor::resizeEvent(QResizeEvent*)
{
  m_draftPixmap = QPixmap(size().width(), size().height());
  m_draftPixmap.fill(Qt::transparent);
}

void te::qt::widgets::DraftLayoutEditor::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.drawPixmap(0, 0, m_draftPixmap);
}

bool te::qt::widgets::DraftLayoutEditor::eventFilter(QObject* obj, QEvent* e)
{
  // return true to stop the event; otherwise return false.

  if(obj == this) 
  {
    if(e->type() == QEvent::MouseMove)
    {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
      if(mouseEvent->buttons() == Qt::NoButton)
        lower();
    }
    //else if(e->type() == QEvent::MouseButtonPress)
    //  raise(); // nao sei porque nao passa por aqui quando clico sobre um objeto editavel.
               // Por isso precisei criar um metodo no layoutEditor para dar raise no draftLayoutEditor.
  }
  return false;
}
