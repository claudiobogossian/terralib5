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
  \file terralib/qt/widgets/slider/TimeSliderWidget.cpp

  \brief A widget used to alow the user to controle the visualization of temporal data
*/

//Terralib
#include "TimePropertiesDialog.h"
#include "TimeSliderEventFilter.h"
#include "TimeSliderWidget.h"
#include "ui_TimeSliderWidgetForm.h"

//QT
#include <qgraphicseffect.h>
#include <QtGui/QSlider>
#include <QMouseEvent>

te::qt::widgets::TimeSliderWidget::TimeSliderWidget(QWidget* parent,  Qt::WindowFlags f, int minimum, int maximum)
  : QWidget(parent, f),
    m_ui(new Ui::TimeSliderWidgetForm)
{
    m_ui->setupUi(this);

  //Adjusting the slider's range
  m_ui->m_TemporalHorizontalSlider->setMinimum(minimum);
  m_ui->m_TemporalHorizontalSlider->setMaximum(maximum);

  //Adjusting the Timer
  m_timer.reset(new QTimer(this));
  connect(m_timer.get(), SIGNAL(timeout()), this, SLOT(play()));

  //The opacity used to fade the widget if the mouse is not over it
  QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect;
  opacityEffect->setOpacity(0.3);
  this->setGraphicsEffect(opacityEffect);

  //An event filter used to know when the mouse is over the widget
  this->installEventFilter(new te::qt::widgets::TimeSliderEventFilter(this));

  //Adjusting the icons and the graphic effects
  m_ui->m_exitToolButton->setIcon(QIcon::fromTheme("system-log-out"));
  m_ui->m_settingsToolButton->setIcon(QIcon::fromTheme("preferences-system"));
  m_ui->m_playToolButton->setIcon(QIcon::fromTheme("media-playback-start"));
  m_ui->m_skipBackwardToolButton->setIcon(QIcon::fromTheme("media-skip-backward"));
  m_ui->m_skipForwardToolButton->setIcon(QIcon::fromTheme("media-skip-forward"));

  // connect signal and slots
  connect(m_ui->m_exitToolButton, SIGNAL(clicked()), this, SLOT(onExitToolButtonnClicked()));
  connect(m_ui->m_settingsToolButton, SIGNAL(clicked()), this, SLOT(onSettingsToolButtonnClicked()));
  connect(m_ui->m_playToolButton, SIGNAL(clicked()), this, SLOT(onPlayToolButtonnClicked()));
  connect(m_ui->m_skipBackwardToolButton, SIGNAL(clicked()), this, SLOT(onSkipBackwardToolButtonnClicked()));
  connect(m_ui->m_skipForwardToolButton, SIGNAL(clicked()), this, SLOT(onSkipForwardToolButtonnClicked()));

  setAttribute(Qt::WA_DeleteOnClose, true);

  m_loop = false;   //By default, the animation will not be looping.
  m_state = false;  //By default, the amimation state will be false, as in not playing.
  m_speed = 100;    //By default, the animation speed will be 100 miliseconds.
}

te::qt::widgets::TimeSliderWidget::~TimeSliderWidget()
{ 
}

void te::qt::widgets::TimeSliderWidget::onExitToolButtonnClicked()
{
  this->close();
}

void te::qt::widgets::TimeSliderWidget::onSettingsToolButtonnClicked()
{
  std::auto_ptr<te::qt::widgets::TimePropertiesDialog> dlg (new te::qt::widgets::TimePropertiesDialog(this));
  int res = dlg->exec();
  if (res == QDialog::Accepted)
  {
    m_loop = dlg->isLoopChecked();
    m_speed  = 10000/dlg->getValue();
  }
}

void te::qt::widgets::TimeSliderWidget::onPlayToolButtonnClicked()
{
  if(!m_state)
  {
    //The timer will bradcast a timeout for every step per <m_speed> miliseconds
    m_timer->start(m_ui->m_TemporalHorizontalSlider->singleStep() * m_speed);
    m_state = true;
    m_ui->m_playToolButton->setIcon(QIcon::fromTheme("media-playback-stop"));
  }
  else
  {
    m_state = false;
    m_timer->stop();
    m_ui->m_playToolButton->setIcon(QIcon::fromTheme("media-playback-start"));
  }
}

void te::qt::widgets::TimeSliderWidget::onSkipBackwardToolButtonnClicked()
{
  m_ui->m_TemporalHorizontalSlider->setValue(m_ui->m_TemporalHorizontalSlider->minimum());
}

void te::qt::widgets::TimeSliderWidget::onSkipForwardToolButtonnClicked()
{
  m_ui->m_TemporalHorizontalSlider->setValue(m_ui->m_TemporalHorizontalSlider->maximum());
}

void te::qt::widgets::TimeSliderWidget::play()
{
  int step = m_ui->m_TemporalHorizontalSlider->value() + m_ui->m_TemporalHorizontalSlider->singleStep();

  m_ui->m_TemporalHorizontalSlider->setValue(step);

  if(step > m_ui->m_TemporalHorizontalSlider->maximum())
  {
    if(m_loop)
      m_ui->m_TemporalHorizontalSlider->setValue(m_ui->m_TemporalHorizontalSlider->minimum());
    else
    {
      m_timer->stop();
      m_state = false;
      m_ui->m_playToolButton->setIcon(QIcon::fromTheme("media-playback-start"));
    }
  }
}