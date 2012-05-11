/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file QtProgressEvent.h
 
  \brief The QtProgressBarSetValueEvent is a custom event used to set a new value into 
         a progress bar. Used in thread codes.
*/


//TerraLib
#include "QtProgressBarSetMessageEvent.h"

QEvent::Type te::qt::widgets::QtProgressBarSetMessageEvent::m_customEventType = QEvent::None;

te::qt::widgets::QtProgressBarSetMessageEvent::QtProgressBarSetMessageEvent(std::string value) : 
  QEvent(QtProgressBarSetMessageEvent::type()), m_value(value)
{
}

te::qt::widgets::QtProgressBarSetMessageEvent::~QtProgressBarSetMessageEvent()
{
}

QEvent::Type te::qt::widgets::QtProgressBarSetMessageEvent::type()
{
  if (m_customEventType == QEvent::None)
  {
    int generatedType = QEvent::registerEventType();
    m_customEventType = static_cast<QEvent::Type>(generatedType);
  }
            
  return m_customEventType;
}
      
      