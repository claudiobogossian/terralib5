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
  \file terralib/qt/widgets/slider/SliderPropertiesDialog.h

  \brief A widget used to alow the user to control the visualization of temporal data
*/


//Terralib
#include "../../../datatype/TimeInstant.h"
#include "../../../datatype/TimePeriod.h"
#include "SliderPropertiesDialog.h"
#include "ui_SliderPropertiesDialogForm.h"

//QT
#include <QMessageBox>

//Utility functions

QDateTime timeInstant2QDate(te::dt::TimeInstant time)
{
  QDate qdatei(time.getDate().getYear(), time.getDate().getMonth(), time.getDate().getDay());
  QTime qtimei(time.getTime().getHours(), time.getTime().getMinutes(), time.getTime().getSeconds());
  QDateTime qdate(qdatei, qtimei);
  return qdate;
}

te::dt::TimeInstant qdate2TimeInstant(QDateTime qdate)
{
  te::dt::Date date(qdate.date().year(), qdate.date().month(), qdate.date().day());
  te::dt::TimeDuration duration(qdate.time().hour(), qdate.time().minute(), qdate.time().second());
  te::dt::TimeInstant time(date, duration);
  return time;
}

te::qt::widgets::SliderPropertiesDialog::SliderPropertiesDialog(te::dt::TimePeriod temporalExtent, QWidget* parent,  Qt::WindowFlags f)
  : QDialog(parent, f),
    m_temporalExtent(temporalExtent),
    m_ui(new Ui::SliderPropertiesDialogForm)
{
    m_ui->setupUi(this);

  m_ui->m_opacitySpinBox->setMinimum(0);
  m_ui->m_opacitySpinBox->setMaximum(255);
  m_ui->m_opacitySpinBox->setValue(255);
  m_ui->m_opacitySpinBox->setSingleStep(5);
  m_ui->m_forwardRadioButton->setCheckable(true);

  m_ui->m_forwardColorPushButton->installEventFilter(this);
  m_ui->m_backwardColorPushButton->installEventFilter(this);
  m_ui->m_initialAnimationDateTimeEdit->installEventFilter(this);
  m_ui->m_finalAnimationDateTimeEdit->installEventFilter(this);
  m_ui->m_initialAnimationDateTimeEdit->setDisplayFormat("dd/MMM/yyyy hh:mm:ss");
  m_ui->m_finalAnimationDateTimeEdit->setDisplayFormat("dd/MMM/yyyy hh:mm:ss");

  connect(m_ui->m_autoPanCheckBox, SIGNAL(clicked(bool) ), this, SLOT(onAutoPanCheckBoxClicked(bool)));
  connect(m_ui->m_trajectoryColorComboBox, SIGNAL(activated(int) ), this, SLOT(onTrajectoryColorComboBoxActivated(int)));
  connect(m_ui->m_opacityComboBox, SIGNAL(activated(int) ), this, SLOT(onOpacityComboBoxActivated(int)));
  connect(m_ui->m_opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOpacityValueChanged(int)));
  connect(m_ui->m_resetInitialTimePushButton, SIGNAL(clicked()), this, SLOT(onResetInitialTimePushButtonClicked()));
  connect(m_ui->m_resetFinalTimePushButton, SIGNAL(clicked()), this, SLOT(onResetFinalTimePushButtonClicked()));

  //connect signal and slots
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
}

te::qt::widgets::SliderPropertiesDialog::~SliderPropertiesDialog()
{
}

void te::qt::widgets::SliderPropertiesDialog::populateUi(te::dt::TimePeriod currentTemporalExtent, QList<QGraphicsItem*> items, bool forward, bool loop, bool goBack)
{
  m_ui->m_initialAnimationDateTimeEdit->setDateTime(timeInstant2QDate(currentTemporalExtent.getInitialTimeInstant()));
  m_ui->m_finalAnimationDateTimeEdit->setDateTime(timeInstant2QDate(currentTemporalExtent.getFinalTimeInstant()));

  // set mimimum & maximum datetime
  m_ui->m_initialAnimationDateTimeEdit->setMinimumDateTime(timeInstant2QDate(currentTemporalExtent.getInitialTimeInstant()));
  m_ui->m_finalAnimationDateTimeEdit->setMaximumDateTime(timeInstant2QDate(currentTemporalExtent.getFinalTimeInstant()));

  //Adjusting ui elements
  m_ui->m_forwardRadioButton->setChecked(forward);
  m_ui->m_backwardRadioButton->setChecked(!forward);
  m_ui->m_loopCheckBox->setChecked(loop);
  m_ui->m_goAndBackCheckBox->setChecked(goBack);

  m_trajectories = items;
  QList<QGraphicsItem*>::iterator it;

  for(it = m_trajectories.begin(); it != m_trajectories.end(); ++it)
  {
    m_ui->m_trajectoryColorComboBox->addItem(((te::qt::widgets::TrajectoryItem*)(*it))->m_title);
    onTrajectoryColorComboBoxActivated(items.size()-1);

    m_ui->m_opacityComboBox->addItem(((te::qt::widgets::TrajectoryItem*)(*it))->m_title);
    onOpacityComboBoxActivated(items.size()-1);
  }
}

bool te::qt::widgets::SliderPropertiesDialog::isForwardChecked()
{
  return m_ui->m_forwardRadioButton->isChecked();
}

bool te::qt::widgets::SliderPropertiesDialog::isLoopChecked()
{
  return m_ui->m_loopCheckBox->isChecked();
}

bool te::qt::widgets::SliderPropertiesDialog::isGoBackChecked()
{
  return m_ui->m_goAndBackCheckBox->isChecked();
}

QDateTime te::qt::widgets::SliderPropertiesDialog::getInitialQDate()
{
  return m_ui->m_initialAnimationDateTimeEdit->dateTime();
}

QDateTime te::qt::widgets::SliderPropertiesDialog::getFinalQDate()
{
  return m_ui->m_finalAnimationDateTimeEdit->dateTime();
}

te::dt::TimeInstant te::qt::widgets::SliderPropertiesDialog::getInitialTime()
{
  return qdate2TimeInstant(m_ui->m_initialAnimationDateTimeEdit->dateTime());
}

te::dt::TimeInstant te::qt::widgets::SliderPropertiesDialog::getFinalTime()
{
  return qdate2TimeInstant(m_ui->m_finalAnimationDateTimeEdit->dateTime());
}

void te::qt::widgets::SliderPropertiesDialog::onOpacityValueChanged(int v)
{
  QString titlecb = m_ui->m_opacityComboBox->currentText();

  QList<QGraphicsItem*>::iterator it;

  for(it = m_trajectories.begin(); it != m_trajectories.end(); ++it)
  {
    te::qt::widgets::AnimationItem* ai = (AnimationItem*)(*it);
    QString title = ai->m_title;
    if(title.contains(titlecb))
    {
      ai->m_opacity = v;
      break;
    }
  }
}

void te::qt::widgets::SliderPropertiesDialog::onAutoPanCheckBoxClicked(bool b)
{
  QString title = m_ui->m_trajectoryColorComboBox->currentText();

  QList<QGraphicsItem*>::iterator it;
  for(it = m_trajectories.begin(); it != m_trajectories.end(); ++it)
  {
    AnimationItem* ai = (AnimationItem*)(*it);
    if(ai->m_title == title)
      ai->m_automaticPan = !ai->m_automaticPan; // TOGGLE
    else
      ai->m_automaticPan = false;
  }
}

void te::qt::widgets::SliderPropertiesDialog::onResetInitialTimePushButtonClicked()
{
  m_ui->m_initialAnimationDateTimeEdit->setMinimumDateTime(timeInstant2QDate(m_temporalExtent.getInitialTimeInstant()));
  m_ui->m_initialAnimationDateTimeEdit->setDateTime(timeInstant2QDate(m_temporalExtent.getInitialTimeInstant()));
}

void te::qt::widgets::SliderPropertiesDialog::onResetFinalTimePushButtonClicked()
{
  m_ui->m_finalAnimationDateTimeEdit->setMaximumDateTime(timeInstant2QDate(m_temporalExtent.getFinalTimeInstant()));
  m_ui->m_finalAnimationDateTimeEdit->setDateTime(timeInstant2QDate(m_temporalExtent.getFinalTimeInstant()));
}

void te::qt::widgets::SliderPropertiesDialog::onTrajectoryColorComboBoxActivated(int i)
{
  QString titlecb = m_ui->m_trajectoryColorComboBox->currentText();

  QList<QGraphicsItem*>::iterator it;
  te::qt::widgets::TrajectoryItem* ti = 0;

  for(it = m_trajectories.begin(); it != m_trajectories.end(); ++it)
  {
    ti = (te::qt::widgets::TrajectoryItem*)(*it);
    QString title = ti->m_title;
    if(title == titlecb)
      break;
  }
  if(ti == 0)
    return;

  m_ui->m_forwardColorPushButton->setPalette(QPalette(ti->m_forwardColor));
  m_ui->m_forwardColorPushButton->update();
  m_ui->m_backwardColorPushButton->setPalette(QPalette(ti->m_backwardColor));
  m_ui->m_backwardColorPushButton->update();
  m_ui->m_autoPanCheckBox->setChecked(ti->m_automaticPan);
}

void te::qt::widgets::SliderPropertiesDialog::onOpacityComboBoxActivated(int i)
{
  QString titlecb = m_ui->m_opacityComboBox->currentText();
  QList<QGraphicsItem*>::iterator it;
  te::qt::widgets::TrajectoryItem* ti = 0;

  for(it = m_trajectories.begin(); it != m_trajectories.end(); ++it)
  {
    ti = (te::qt::widgets::TrajectoryItem*)(*it);
    QString title = ti->m_title;
    if(title.contains(titlecb))
    {
      int op = ti->m_opacity;
      m_ui->m_opacitySpinBox->setValue(op);
      break;
    }
  }
}

void te::qt::widgets::SliderPropertiesDialog::onHelpPushButtonClicked()
{
}

void te::qt::widgets::SliderPropertiesDialog::onOkPushButtonClicked()
{
  if(m_ui->m_initialAnimationDateTimeEdit->dateTime() >= m_ui->m_finalAnimationDateTimeEdit->dateTime())
  {
    QMessageBox::warning(this, "Time Extent Error", "Initial time can not be larger than the end time!", QMessageBox::Ok);
    return;
  }
  this->accept();
}
