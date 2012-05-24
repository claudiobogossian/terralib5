/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/MarkWidget.cpp

  \brief A widget used to build a mark element.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../maptools/AbstractMarkFactory.h"
#include "../../../se/Mark.h"
#include "../Utils.h"
#include "BasicFillWidget.h"
#include "BasicStrokeWidget.h"
#include "ui_MarkWidgetForm.h"
#include "MarkWidget.h"

// Qt
#include <QtGui/QImage>
#include <QtGui/QListWidget>

// STL
#include <cassert>

te::qt::widgets::MarkWidget::MarkWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::MarkWidgetForm),
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

  // Mark Contents
  m_contentsMarkWidget = new QListWidget(this);
  m_contentsMarkWidget->setViewMode(QListView::IconMode);
  m_contentsMarkWidget->setIconSize(QSize(32, 32));
  m_contentsMarkWidget->setResizeMode(QListWidget::Adjust);

  // Gets supported marks
  te::map::AbstractMarkFactory::SupportedMarks(m_supportedMarks);

  // Setups initial mark
  m_mark->setWellKnownName(new std::string(m_supportedMarks[0]));
  m_mark->setFill(m_fillWidget->getFill());
  m_mark->setStroke(m_strokeWidget->getStroke());

  // Creates mark itens
  for(std::size_t i = 0; i < m_supportedMarks.size(); ++i)
  {
    QListWidgetItem* markItem = new QListWidgetItem(m_contentsMarkWidget);
    markItem->setText(m_supportedMarks[i].c_str());
    markItem->setTextAlignment(Qt::AlignHCenter);
    markItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  }
  updateMarkIcons();
  m_contentsMarkWidget->setCurrentRow(0);

  // Adjusting...
  QGridLayout* contentsLayout = new QGridLayout(m_ui->m_markTypeGroupBox);
  contentsLayout->addWidget(m_contentsMarkWidget);
  m_ui->m_markTypeGroupBox->setMaximumWidth(128);

  // Signals & slots
  connect(m_contentsMarkWidget, SIGNAL(currentTextChanged(const QString&)), this, SLOT(onMarkTypeChanged(const QString&)));
  connect(m_fillWidget, SIGNAL(fillChanged()), SLOT(onFillChanged()));
  connect(m_ui->m_fillGroupBox, SIGNAL(toggled(bool)), this, SLOT(onFillGroupBoxToggled(bool)));
  connect(m_strokeWidget, SIGNAL(strokeChanged()), SLOT(onStrokeChanged()));
  connect(m_ui->m_strokeGroupBox, SIGNAL(toggled(bool)), this, SLOT(onStrokeGroupBoxToggled(bool)));
}

te::qt::widgets::MarkWidget::~MarkWidget()
{
  delete m_mark;
}

void te::qt::widgets::MarkWidget::setMark(const te::se::Mark* mark)
{
  assert(mark);

  delete m_mark;

  m_mark = mark->clone();

  updateUi();
}

te::se::Mark* te::qt::widgets::MarkWidget::getMark() const
{
  return m_mark->clone();
}

void te::qt::widgets::MarkWidget::updateUi()
{
  const std::string* name = m_mark->getWellKnownName();
  if(name)
  {
    QString qName(name->c_str());
    QList<QListWidgetItem*> itens = m_contentsMarkWidget->findItems(qName, Qt::MatchFixedString);
    assert(!itens.empty());
    m_contentsMarkWidget->setCurrentItem(itens[0]);
  }

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

  updateMarkIcons();
}

void te::qt::widgets::MarkWidget::updateMarkIcons()
{
  for(std::size_t i = 0; i < m_supportedMarks.size(); ++i)
  {
    te::se::Mark* temporaryMark = m_mark->clone();
    temporaryMark->setWellKnownName(new std::string(m_supportedMarks[i]));

    // Getting mark graphical representation
    std::size_t size = 32;
    te::color::RGBAColor** rgba = te::map::AbstractMarkFactory::make(temporaryMark, size);
    QImage* img = te::qt::widgets::GetImage(rgba, size, size);

    QListWidgetItem* markItem = m_contentsMarkWidget->item(i);
    markItem->setIcon(QPixmap::fromImage(*img));

    delete img;

    te::common::Free(rgba, size);

    delete temporaryMark;
  }
}

void te::qt::widgets::MarkWidget::onMarkTypeChanged(const QString& currentText)
{
  m_mark->setWellKnownName(new std::string(currentText.toStdString()));
}

void te::qt::widgets::MarkWidget::onStrokeChanged()
{
  m_mark->setStroke(m_strokeWidget->getStroke());

  updateMarkIcons();
}

void te::qt::widgets::MarkWidget::onStrokeGroupBoxToggled(bool on)
{
  if(on == false)
    m_mark->setStroke(0);
  else
    m_mark->setStroke(m_strokeWidget->getStroke());

  updateMarkIcons();
}

void te::qt::widgets::MarkWidget::onFillChanged()
{
  m_mark->setFill(m_fillWidget->getFill());

  updateMarkIcons();
}

void te::qt::widgets::MarkWidget::onFillGroupBoxToggled(bool on)
{
  if(on == false)
    m_mark->setFill(0);
  else
    m_mark->setFill(m_fillWidget->getFill());

  updateMarkIcons();
}
