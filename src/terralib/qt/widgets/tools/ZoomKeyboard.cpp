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
  \file terralib/qt/widgets/tools/ZoomKeyboard.cpp

  \brief This class implements a concrete tool to geographic zoom operation using the keyboard.
*/

// TerraLib
#include "../canvas/MapDisplay.h"
#include "ZoomKeyboard.h"

// Qt
#include <QKeyEvent>

te::qt::widgets::ZoomKeyboard::ZoomKeyboard(te::qt::widgets::MapDisplay* display, const double& zoomFactor, QObject* parent) 
  : Zoom(display, zoomFactor, In, parent)
{
  m_display->setFocusPolicy(Qt::ClickFocus);
}

te::qt::widgets::ZoomKeyboard::~ZoomKeyboard()
{
}

bool te::qt::widgets::ZoomKeyboard::eventFilter(QObject* watched, QEvent* e)
{
  if(watched != m_display)
    return false;

  switch(e->type())
  {
    case QEvent::KeyPress:
    {
      QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);

      int key = keyEvent->key();

      switch(key)
      {
        case Qt::Key_Up:
        case Qt::Key_Plus:
          setZoomType(In);
        break;

        case Qt::Key_Down:
        case Qt::Key_Minus:
          setZoomType(Out);
        break;

        default:
          return false;
      }
      
      applyZoom();

      return true;
    }

    default:
       return Zoom::eventFilter(watched, e);
  }
}
