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
  \file terralib/qt/widgets/progress/CreateProgressWidgetItemEvent.cpp

  \brief The CreateProgressWidgetItemEvent is a custom event used to create a progress widget item.
*/

// TerraLib
#include "CreateProgressWidgetItemEvent.h"

QEvent::Type te::qt::widgets::CreateProgressWidgetItemEvent::sm_customEventType(QEvent::None);

te::qt::widgets::CreateProgressWidgetItemEvent::CreateProgressWidgetItemEvent(const QString& label, int taskId, int totalSteps)
  : QEvent(CreateProgressWidgetItemEvent::type()),
    m_label(label),
    m_taskId(taskId),
    m_totalSteps(totalSteps)
{
}

te::qt::widgets::CreateProgressWidgetItemEvent::~CreateProgressWidgetItemEvent()
{
}

QEvent::Type te::qt::widgets::CreateProgressWidgetItemEvent::type()
{
  if(sm_customEventType == QEvent::None)
  {
    int generatedType = QEvent::registerEventType();
    sm_customEventType = static_cast<QEvent::Type>(generatedType);
  }

  return sm_customEventType;
}
