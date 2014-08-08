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
#include "TimeSliderWidget.h"

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

te::qt::widgets::SliderPropertiesDialog::SliderPropertiesDialog(QWidget* parent,  Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::SliderPropertiesDialogForm)
{
  m_ui->setupUi(this);
  m_tsw = (TimeSliderWidget*)parent;

  m_ui->m_opacitySpinBox->setMinimum(0);
  m_ui->m_opacitySpinBox->setMaximum(255);
  m_ui->m_opacitySpinBox->setValue(255);
  m_ui->m_opacitySpinBox->setSingleStep(5);
  m_ui->m_forwardRadioButton->setCheckable(true);

  m_ui->m_initialAnimationDateTimeEdit->installEventFilter(m_tsw);
  m_ui->m_finalAnimationDateTimeEdit->installEventFilter(m_tsw);
  m_ui->m_initialAnimationDateTimeEdit->setDisplayFormat("dd/MMM/yyyy hh:mm:ss");
  m_ui->m_finalAnimationDateTimeEdit->setDisplayFormat("dd/MMM/yyyy hh:mm:ss");

  connect(m_ui->m_drawTrailCheckBox, SIGNAL(clicked(bool) ), m_tsw, SLOT(onDrawTrailCheckBoxClicked(bool)));
  connect(m_ui->m_applyAnimationItemPushButton, SIGNAL(clicked(bool) ), m_tsw, SLOT(onApplyAnimationItemPushButtonClicked(bool)));
  connect(m_ui->m_autoPanCheckBox, SIGNAL(clicked(bool) ), m_tsw, SLOT(onAutoPanCheckBoxClicked(bool)));
  connect(m_ui->m_opacitySpinBox, SIGNAL(valueChanged(int)), m_tsw, SLOT(onOpacityValueChanged(int)));
  connect(m_ui->m_forwardRadioButton, SIGNAL(clicked(bool) ), m_tsw, SLOT(onForwardRadioButtonClicked(bool)));
  connect(m_ui->m_backwardRadioButton, SIGNAL(clicked(bool) ), m_tsw, SLOT(onBackwardRadioButtonClicked(bool)));
  connect(m_ui->m_loopCheckBox, SIGNAL(clicked(bool) ), m_tsw, SLOT(onLoopCheckBoxClicked(bool)));
  connect(m_ui->m_goAndBackCheckBox, SIGNAL(clicked(bool) ), m_tsw, SLOT(onGoAndBackCheckBoxClicked(bool)));
  connect(m_ui->m_applyTimeIntervalPushButton, SIGNAL(clicked(bool) ), m_tsw, SLOT(onApplyTimeIntervalPushButtonClicked(bool)));
  connect(m_ui->m_frontPushButton, SIGNAL(clicked(bool)), m_tsw, SLOT(onFrontPushButtonClicked(bool)));
  connect(m_ui->m_backPushButton, SIGNAL(clicked(bool)), m_tsw, SLOT(onBackPushButtonClicked(bool)));
  connect(m_ui->m_opacityComboBox, SIGNAL(activated(int) ), m_tsw, SLOT(onOpacityComboBoxActivated(int)));
  connect(m_ui->m_trajectoryColorComboBox, SIGNAL(activated(int) ), m_tsw, SLOT(onTrajectoryColorComboBoxActivated(int)));
  connect(m_ui->m_resetInitialTimePushButton, SIGNAL(clicked()), m_tsw, SLOT(onResetInitialTimePushButtonClicked()));
  connect(m_ui->m_resetFinalTimePushButton, SIGNAL(clicked()), m_tsw, SLOT(onResetFinalTimePushButtonClicked()));

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));

}

te::qt::widgets::SliderPropertiesDialog::~SliderPropertiesDialog()
{
}

void te::qt::widgets::SliderPropertiesDialog::populateUi()
{
  TimeSliderWidget *tsw = (TimeSliderWidget*)parent();
  te::dt::TimeInstant ti = tsw->m_temporalAnimationExtent.getInitialTimeInstant();
  QDate qdatei(ti.getDate().getYear(), ti.getDate().getMonth(), ti.getDate().getDay());
  QTime qtimei(ti.getTime().getHours(), ti.getTime().getMinutes(), ti.getTime().getSeconds());
  QDateTime qdatetimei(qdatei, qtimei);
  m_ui->m_initialAnimationDateTimeEdit->setDateTime(qdatetimei);

  te::dt::TimeInstant tf = tsw->m_temporalAnimationExtent.getFinalTimeInstant();
  QDate qdatef(tf.getDate().getYear(), tf.getDate().getMonth(), tf.getDate().getDay());
  QTime qtimef(tf.getTime().getHours(), tf.getTime().getMinutes(), tf.getTime().getSeconds());
  QDateTime qdatetimef(qdatef, qtimef);
  m_ui->m_finalAnimationDateTimeEdit->setDateTime(qdatetimef);

  // set mimimum datetime
  te::dt::TimeInstant tmin = tsw->m_temporalExtent.getInitialTimeInstant();
  QDateTime minimum(QDate(tmin.getDate().getYear(), tmin.getDate().getMonth(), tmin.getDate().getDay()),
                    QTime(tmin.getTime().getHours(), tmin.getTime().getMinutes(), tmin.getTime().getSeconds()));
  m_ui->m_initialAnimationDateTimeEdit->setMinimumDateTime(minimum);

  // set maximum datetime
  te::dt::TimeInstant tmax = tsw->m_temporalExtent.getFinalTimeInstant();
  QDateTime maximum(QDate(tmax.getDate().getYear(), tmax.getDate().getMonth(), tmax.getDate().getDay()),
                    QTime(tmax.getTime().getHours(), tmax.getTime().getMinutes(), tmax.getTime().getSeconds()));
  m_ui->m_finalAnimationDateTimeEdit->setMaximumDateTime(maximum);

  if(tsw->m_direction == QAbstractAnimation::Forward)
  {
    if(m_ui->m_forwardRadioButton->isChecked() == false)
      m_ui->m_forwardRadioButton->toggle();
  }

  if(tsw->m_loop)
    m_ui->m_loopCheckBox->setChecked(true);
  if(tsw->m_goAndBack)
    m_ui->m_goAndBackCheckBox->setChecked(true);
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

void te::qt::widgets::SliderPropertiesDialog::onDrawTrackCheckBoxClicked(bool b)
{
}

void te::qt::widgets::SliderPropertiesDialog::onHelpPushButtonClicked()
{
}
