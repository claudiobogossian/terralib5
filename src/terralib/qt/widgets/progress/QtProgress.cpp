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
  \file terralib/qt/widgets/progress/QtProgress.cpp

  \brief The QtProgress is a qt implementation of a progress class used to define the main progress functions.
*/

// TerraLib
#include "QtProgress.h"
#include "QtProgressBarSetMessageEvent.h"
#include "QtProgressBarSetValueEvent.h"

// Qt
#include <QtCore/QCoreApplication>

te::qt::widgets::QtProgress::QtProgress(QWidget* parent) : QProgressDialog(parent), te::common::AbstractProgress()
{
  this->setWindowModality(Qt::WindowModal);

  //set default range
  QProgressDialog::setRange(0, 100);

  //enable this object to handle events
  installEventFilter(this);

  //connect signal cancel
  connect(this, SIGNAL(canceled()), this, SLOT(cancel()));
}

te::qt::widgets::QtProgress::~QtProgress()
{
}

void te::qt::widgets::QtProgress::setCurrentStep(const int& step)
{
  te::common::AbstractProgress::setCurrentStep(step);

  if(this->hasToUpdate())
  {
    int value = this->getCurrentProportionalStep();
    if(m_isMultiThread)
    {
      QCoreApplication::postEvent(this, new te::qt::progress::QtProgressBarSetValueEvent(value));

      QCoreApplication::processEvents();
    }
    else
    {
      QProgressDialog::setValue(value);
    }
  }
}

void te::qt::widgets::QtProgress::setMessage(const std::string& message)
{
  if(m_isMultiThread)
  {
    QCoreApplication::postEvent(this, new te::qt::progress::QtProgressBarSetMessageEvent(message));
  }
  else
  {
    QProgressDialog::setLabelText(message.c_str());
  }
  
  te::common::AbstractProgress::setMessage(message);
}

void te::qt::widgets::QtProgress::setTitle(const std::string& title)
{
  QProgressDialog::setWindowTitle(title.c_str());
}

void te::qt::widgets::QtProgress::reset()
{
  QProgressDialog::reset();

  te::common::AbstractProgress::reset();
}

void te::qt::widgets::QtProgress::cancel()
{
  this->setActive(false);
}

bool te::qt::widgets::QtProgress::eventFilter(QObject* obj, QEvent* event)
{
  if(obj == this && event->type() == te::qt::progress::QtProgressBarSetValueEvent::type())
  {
    te::qt::progress::QtProgressBarSetValueEvent* e = 
      static_cast<te::qt::progress::QtProgressBarSetValueEvent*>(event);

    QProgressDialog::setValue(e->m_value);

    te::common::AbstractProgress::pulse();

    return true;
  }
  else if(obj == this && event->type() == te::qt::progress::QtProgressBarSetMessageEvent::type())
  {
    te::qt::progress::QtProgressBarSetMessageEvent* e = 
      static_cast<te::qt::progress::QtProgressBarSetMessageEvent*>(event);

    QProgressDialog::setLabelText(e->m_value.c_str());

    te::common::AbstractProgress::pulse();

    return true;
  }
  else
  {
    return QProgressDialog::eventFilter(obj, event);
  }
}
