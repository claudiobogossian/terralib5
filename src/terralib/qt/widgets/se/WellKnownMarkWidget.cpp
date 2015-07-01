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
  \file terralib/qt/widgets/se/WellKnownMarkWidget.cpp

  \brief A widget used to build a well known mark element.
*/

// TerraLib
#include "../../../maptools/MarkRendererManager.h"
#include "../../../se/Mark.h"
#include "BasicFillWidget.h"
#include "BasicStrokeWidget.h"
#include "ui_WellKnownMarkWidgetForm.h"
#include "WellKnownMarkWidget.h"

// STL
#include <cassert>

te::qt::widgets::WellKnownMarkWidget::WellKnownMarkWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::WellKnownMarkWidgetForm),
    m_mark(new te::se::Mark)
{
  m_ui->setupUi(this);

  // Fill Widget
  m_fillWidget = new te::qt::widgets::BasicFillWidget(this);

  // Adjusting...
  QGridLayout* fillLayout = new QGridLayout(m_ui->m_fillGroupBox);
  fillLayout->addWidget(m_fillWidget);

  // Stroke Widget
  m_strokeWidget = new te::qt::widgets::BasicStrokeWidget(this);

  // Adjusting...
  QGridLayout* strokeLayout = new QGridLayout(m_ui->m_strokeGroupBox);
  strokeLayout->addWidget(m_strokeWidget);

  // Gets supported marks
  te::map::MarkRendererManager::getInstance().getAllSupportedMarks(m_supportedMarks);
  for(std::size_t i = 0; i < m_supportedMarks.size(); ++i)
    m_ui->m_markTypeComboBox->addItem(m_supportedMarks[i].c_str());

  // Setups initial mark
  m_mark->setWellKnownName(new std::string(m_supportedMarks[0]));
  m_mark->setFill(m_fillWidget->getFill());
  m_mark->setStroke(m_strokeWidget->getStroke());

  // Signals & slots
  connect(m_ui->m_markTypeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onMarkTypeComboBoxCurrentIndexChanged(const QString&)));
  connect(m_fillWidget, SIGNAL(fillChanged()), SLOT(onFillChanged()));
  connect(m_ui->m_fillGroupBox, SIGNAL(toggled(bool)), this, SLOT(onFillGroupBoxToggled(bool)));
  connect(m_strokeWidget, SIGNAL(strokeChanged()), SLOT(onStrokeChanged()));
  connect(m_ui->m_strokeGroupBox, SIGNAL(toggled(bool)), this, SLOT(onStrokeGroupBoxToggled(bool)));
}

te::qt::widgets::WellKnownMarkWidget::~WellKnownMarkWidget()
{
  delete m_mark;
}

void te::qt::widgets::WellKnownMarkWidget::setMark(const te::se::Mark* mark)
{
  assert(mark);

  delete m_mark;

  m_mark = mark->clone();

  updateUi();
}

te::se::Mark* te::qt::widgets::WellKnownMarkWidget::getMark() const
{
  return m_mark->clone();
}

void te::qt::widgets::WellKnownMarkWidget::updateUi()
{
  const std::string* name = m_mark->getWellKnownName();
  assert(name); // TODO: Exception?

  QString qName(name->c_str());
  int index = m_ui->m_markTypeComboBox->findText(qName, Qt::MatchFixedString);
  assert(index != -1); // TODO: Exception?
  m_ui->m_markTypeComboBox->setCurrentIndex(index);

  const te::se::Stroke* stroke = m_mark->getStroke();
  if(stroke)
  {
    m_strokeWidget->setStroke(stroke);
    m_ui->m_strokeGroupBox->setChecked(true);
  }
  else
    m_ui->m_strokeGroupBox->setChecked(false);

  const te::se::Fill* fill = m_mark->getFill();
  if(fill)
  {
    m_fillWidget->setFill(fill);
    m_ui->m_fillGroupBox->setChecked(true);
  }
  else
    m_ui->m_fillGroupBox->setChecked(false);
}

void te::qt::widgets::WellKnownMarkWidget::onMarkTypeComboBoxCurrentIndexChanged(const QString& currentText)
{
  m_mark->setWellKnownName(new std::string(currentText.toStdString()));
  emit markChanged();
}

void te::qt::widgets::WellKnownMarkWidget::onStrokeChanged()
{
  m_mark->setStroke(m_strokeWidget->getStroke());
  emit markChanged();
}

void te::qt::widgets::WellKnownMarkWidget::onStrokeGroupBoxToggled(bool on)
{
  if(on == false)
    m_mark->setStroke(0);
  else
    m_mark->setStroke(m_strokeWidget->getStroke());
  emit markChanged();
}

void te::qt::widgets::WellKnownMarkWidget::onFillChanged()
{
  m_mark->setFill(m_fillWidget->getFill());
  emit markChanged();
}

void te::qt::widgets::WellKnownMarkWidget::onFillGroupBoxToggled(bool on)
{
  if(on == false)
    m_mark->setFill(0);
  else
    m_mark->setFill(m_fillWidget->getFill());
  emit markChanged();
}
