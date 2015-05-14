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
  \file terralib/qt/widgets/help/HelpPushButton.cpp

  \brief A specialized button that uses terralib help managers.
*/

// TerraLib
#include "HelpManager.h"
#include "HelpPushButton.h"

//Qt
#include <QMouseEvent>

te::qt::widgets::HelpPushButton::HelpPushButton(QWidget* parent)
  : QPushButton(tr("&Help"), parent)
{
}

void te::qt::widgets::HelpPushButton::setPageReference(const QString& ref)
{
  m_pgRef = ref;
}

void te::qt::widgets::HelpPushButton::setNameSpace(const QString& ns)
{
  m_ns = ns;
}

void te::qt::widgets::HelpPushButton::mousePressEvent(QMouseEvent* e)
{
  QPushButton::mousePressEvent(e);

  if(e->button() == Qt::LeftButton)
    te::qt::widgets::HelpManager::getInstance().showHelp(m_pgRef, m_ns);
}
