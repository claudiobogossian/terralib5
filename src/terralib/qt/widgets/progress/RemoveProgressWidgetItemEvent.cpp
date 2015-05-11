/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/progress/RemoveProgressWidgetItemEvent.cpp

  \brief The RemoveProgressWidgetItemEvent is a custom event used to remove a progress widget item.
*/

// TerraLib
#include "RemoveProgressWidgetItemEvent.h"

QEvent::Type te::qt::widgets::RemoveProgressWidgetItemEvent::sm_customEventType(QEvent::None);

te::qt::widgets::RemoveProgressWidgetItemEvent::RemoveProgressWidgetItemEvent(const int& taskId)
  : QEvent(RemoveProgressWidgetItemEvent::type()),
    m_taskId(taskId)
{
}

te::qt::widgets::RemoveProgressWidgetItemEvent::~RemoveProgressWidgetItemEvent()
{
}

QEvent::Type te::qt::widgets::RemoveProgressWidgetItemEvent::type()
{
  if(sm_customEventType == QEvent::None)
  {
    int generatedType = QEvent::registerEventType();
    sm_customEventType = static_cast<QEvent::Type>(generatedType);
  }

  return sm_customEventType;
}
