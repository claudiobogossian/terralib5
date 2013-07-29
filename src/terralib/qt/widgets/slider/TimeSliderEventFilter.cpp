/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/slider/TimeSliderEventFilter.cpp

  \brief An eventFilter used to define the behavior and appearance depending on the events it receives
*/

//Terralib
#include "TimeSliderEventFilter.h"

//QT
#include <qevent.h>
#include <qwidget.h>
#include <qgraphicseffect.h>

te::qt::widgets::TimeSliderEventFilter::TimeSliderEventFilter(QObject* parent)
: QObject(parent)
{
}

bool te::qt::widgets::TimeSliderEventFilter::eventFilter(QObject* obj, QEvent* e)
{
  if(QWidget* widget = qobject_cast<QWidget*>(obj))
  {
    QGraphicsEffect* effect = widget->graphicsEffect();
    if(e->type() == QEvent::Enter)
    {
      effect->setEnabled(false);
    }
    else if(e->type() == QEvent::Leave)
    {
      effect->setEnabled(true);
    }
  }
   return QObject::eventFilter(obj, e);
}