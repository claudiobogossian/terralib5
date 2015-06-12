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
  \file terralib/qt/widgets/se/GlyphMarkWidget.cpp

  \brief A widget used to build a mark element represented by a glyph.
*/

// TerraLib
#include "../../../se/Mark.h"
#include "BasicFillWidget.h"
#include "BasicStrokeWidget.h"
#include "CharMapWidget.h"
#include "GlyphMarkRenderer.h"
#include "GlyphMarkWidget.h"
#include "ui_GlyphMarkWidgetForm.h"

// STL
#include <cassert>

te::qt::widgets::GlyphMarkWidget::GlyphMarkWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::GlyphMarkWidgetForm),
    m_mark(new te::se::Mark)
{
  m_ui->setupUi(this);

  // Fill Widget
  m_fillWidget = new te::qt::widgets::BasicFillWidget(this);

  // Adjusting...
  QGridLayout* fillLayout = new QGridLayout(m_ui->m_fillFrame);
  fillLayout->addWidget(m_fillWidget);

  // Char Map Widget
  m_charMapWidget = new te::qt::widgets::CharMapWidget;
  m_ui->m_charMapScrollArea->setWidget(m_charMapWidget);
  m_ui->m_charMapScrollArea->setFixedWidth(m_charMapWidget->width() + 18);
  m_ui->m_charMapScrollArea->setFixedHeight(m_charMapWidget->width() * 0.85);

  // Setups initial mark
  updateMarkName();
  m_mark->setFill(m_fillWidget->getFill());

  // Signals & slots
  connect(m_ui->m_fontComboBox, SIGNAL(currentFontChanged(const QFont&)), m_charMapWidget, SLOT(updateFont(const QFont&)));
  connect(m_ui->m_fontComboBox, SIGNAL(currentFontChanged(const QFont&)), SLOT(onCurrentFontChanged(const QFont&)));
  connect(m_charMapWidget, SIGNAL(charSelected(const unsigned int&)), SLOT(onCharSelected(const unsigned int&)));
  connect(m_fillWidget, SIGNAL(fillChanged()), SLOT(onFillChanged()));
}

te::qt::widgets::GlyphMarkWidget::~GlyphMarkWidget()
{
  delete m_mark;
}

void te::qt::widgets::GlyphMarkWidget::setMark(const te::se::Mark* mark)
{
  assert(mark);

  delete m_mark;

  m_mark = mark->clone();

  updateUi();
}

te::se::Mark* te::qt::widgets::GlyphMarkWidget::getMark() const
{
  return m_mark->clone();
}

void te::qt::widgets::GlyphMarkWidget::updateUi()
{
  const std::string* name = m_mark->getWellKnownName();
  assert(name); // TODO: Exception?

  QString qName(name->c_str());
  QString fontName;
  QChar charCode;
  try
  {
    te::qt::widgets::GlyphMarkRenderer::decode(qName, fontName, charCode);
  }
  catch(...)
  {
  }

  QFont font(fontName);
  m_ui->m_fontComboBox->setCurrentFont(font);
  // TODO: select the char on character map

  const te::se::Fill* fill = m_mark->getFill();
  if(fill)
    m_fillWidget->setFill(fill);
}

void te::qt::widgets::GlyphMarkWidget::updateMarkName()
{
  QString name = te::qt::widgets::GlyphMarkRenderer::encode(m_ui->m_fontComboBox->currentFont().family(), m_charMapWidget->getCurrentChar());
  m_mark->setWellKnownName(new std::string(name.toStdString()));
  emit markChanged();
}

void te::qt::widgets::GlyphMarkWidget::onCurrentFontChanged(const QFont& /*font*/)
{
  updateMarkName();
}

void te::qt::widgets::GlyphMarkWidget::onCharSelected(const unsigned int& /*charCode*/)
{
  updateMarkName();
}

void te::qt::widgets::GlyphMarkWidget::onFillChanged()
{
  m_mark->setFill(m_fillWidget->getFill());
  emit markChanged();
}
