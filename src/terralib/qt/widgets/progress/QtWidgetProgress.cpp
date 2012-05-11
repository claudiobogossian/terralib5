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
  \file QtProgress.cpp

  \brief The QtProgress is a qt implementation of a progress class used to define the main progress functions.
*/

// TerraLib
#include "QtWidgetProgress.h"
#include "QtProgressBarSetValueEvent.h"

#include "terralib.h"

// Qt
#include <QtCore/QCoreApplication>


te::qt::widgets::QtWidgetProgress::QtWidgetProgress(QWidget* parent) : QProgressBar(parent), te::common::AbstractProgress()
{
  //set default range
  QProgressBar::setRange(0, 100);

  //enable this object to handle events
  installEventFilter(this);

  //connect signal cancel
  connect(this, SIGNAL(canceled()), this, SLOT(cancel()));
}

te::qt::widgets::QtWidgetProgress::~QtWidgetProgress()
{  

}

void te::qt::widgets::QtWidgetProgress::setCurrentStep(const int& step)
{
  te::common::AbstractProgress::setCurrentStep(step);

  if(this->hasToUpdate())
  {
    int value = this->getCurrentProportionalStep();
    if(m_isMultiThread)
    {
      QCoreApplication::postEvent(this, new te::qt::widgets::QtProgressBarSetValueEvent(value));

      QCoreApplication::processEvents();
    }
    else
    {
      QProgressBar::setValue(value);
    }
	}
}

void te::qt::widgets::QtWidgetProgress::reset()
{
  QProgressBar::reset();

  te::common::AbstractProgress::reset();
}

void te::qt::widgets::QtWidgetProgress::cancel()
{
  this->setActive(false);
}

bool te::qt::widgets::QtWidgetProgress::eventFilter(QObject* obj, QEvent* event)
{
  if(obj == this && event->type() == te::qt::widgets::QtProgressBarSetValueEvent::type())
  {
    te::qt::widgets::QtProgressBarSetValueEvent* e = 
      static_cast<te::qt::widgets::QtProgressBarSetValueEvent*>(event);

    QProgressBar::setValue(e->m_value);

    te::common::AbstractProgress::pulse();

    return true;
  }
  else
  {
    return QProgressBar::eventFilter(obj, event);
  }
}
