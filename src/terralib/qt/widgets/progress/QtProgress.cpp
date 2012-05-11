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
#include <QtGui/QApplication>

te::qt::widgets::QtProgress::QtProgress(QWidget* parent) : 
  QProgressDialog(parent), te::common::AbstractProgress(), m_numberOfCursors(0)
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

void te::qt::widgets::QtProgress::setTotalSteps(const int& value)
{
  if(QApplication::overrideCursor())
  {
    if(QApplication::overrideCursor()->shape() != Qt::WaitCursor)
    {
      QApplication::setOverrideCursor( Qt::WaitCursor );
      m_numberOfCursors++;
    }
  }
  else
  {
    QApplication::setOverrideCursor( Qt::WaitCursor );
    m_numberOfCursors++;
  }

  te::common::AbstractProgress::setTotalSteps(value);
}

void te::qt::widgets::QtProgress::setCurrentStep(const int& step)
{
  te::common::AbstractProgress::setCurrentStep(step);

  if(this->hasToUpdate())
  {
    int value = this->getCurrentProportionalStep();
    if(m_isMultiThread)
    {
      QCoreApplication::postEvent(this, new QtProgressBarSetValueEvent(value));
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
    QCoreApplication::postEvent(this, new QtProgressBarSetMessageEvent(message));
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

  setModal(true);

  resetCursor();

  te::common::AbstractProgress::reset();
}

void te::qt::widgets::QtProgress::setModal(const bool& flag)
{
  if(flag)
    this->setWindowModality(Qt::WindowModal);
  else
    this->setWindowModality(Qt::NonModal);
}

void te::qt::widgets::QtProgress::cancel()
{
  resetCursor();

  this->setActive(false);
}

bool te::qt::widgets::QtProgress::eventFilter(QObject* obj, QEvent* event)
{
  if(obj == this && event->type() == QtProgressBarSetValueEvent::type())
  {
    QtProgressBarSetValueEvent* e = 
      static_cast<QtProgressBarSetValueEvent*>(event);

    QProgressDialog::setValue(e->m_value);

    te::common::AbstractProgress::pulse();

    return true;
  }
  else if(obj == this && event->type() == QtProgressBarSetMessageEvent::type())
  {
    QtProgressBarSetMessageEvent* e = 
      static_cast<QtProgressBarSetMessageEvent*>(event);

    QProgressDialog::setLabelText(e->m_value.c_str());

    te::common::AbstractProgress::pulse();

    return true;
  }
  else
  {
    return QProgressDialog::eventFilter(obj, event);
  }
}

void te::qt::widgets::QtProgress::enterEvent(QEvent* event)
{
  if(QApplication::overrideCursor())
  {
    if(QApplication::overrideCursor()->shape() != Qt::ArrowCursor)
    {
      QApplication::setOverrideCursor( Qt::ArrowCursor );
      m_numberOfCursors++;
    }
  }
}

void te::qt::widgets::QtProgress::leaveEvent(QEvent* event)
{
  if(QApplication::overrideCursor())
  {
    if(QApplication::overrideCursor()->shape() == Qt::ArrowCursor)
    {
      QApplication::restoreOverrideCursor();
      m_numberOfCursors--;
    }
  }
}

void te::qt::widgets::QtProgress::resetCursor()
{
  //Restore all cursors that were set by QtProgress.
  for (int i = 0; i < m_numberOfCursors; i++)
  {
    if(QApplication::overrideCursor())
      QApplication::restoreOverrideCursor();
  }
  m_numberOfCursors = 0;
}
