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
  \file terralib/qt/widgets/se/GlyphGraphicWidget.cpp

  \brief A widget used to build a graphic associate with a glyph mark element.
*/

// TerraLib
#include "../../../se/Graphic.h"
#include "../../../se/Mark.h"
#include "GlyphGraphicWidget.h"
#include "GlyphMarkWidget.h"
#include "ui_GlyphMarkWidgetForm.h"
#include "ui_GraphicWidgetForm.h"

// Qt
#include <QtGui/QGridLayout>

te::qt::widgets::GlyphGraphicWidget::GlyphGraphicWidget(QWidget* parent, Qt::WindowFlags f)
  : GraphicWidget(parent, f)
{
  // Glyph Mark Widget
  m_glyphMarkWidget = new te::qt::widgets::GlyphMarkWidget(this);

  // Adjusting...
  Ui::GlyphMarkWidgetForm* glyphForm = m_glyphMarkWidget->getForm();

  // Character
  QWidget* charWidget = new QWidget(this);
  QGridLayout* charLayout = new QGridLayout(charWidget);
  charLayout->setContentsMargins(0, 0, 0, 0);
  charLayout->addWidget(m_glyphMarkWidget->getForm()->m_charFrame);
  m_ui->m_graphicTabWidget->addTab(charWidget, tr("&Character"));

  // Fill and Stroke
  QWidget* fillAndStrokeWidget = new QWidget(this);
  QGridLayout* fillAndStrokeLayout = new QGridLayout(fillAndStrokeWidget);
  fillAndStrokeLayout->addWidget(m_glyphMarkWidget->getForm()->m_fillGroupBox, 0, 0);
  fillAndStrokeLayout->addWidget(m_glyphMarkWidget->getForm()->m_strokeGroupBox, 1, 0);
  m_ui->m_graphicTabWidget->addTab(fillAndStrokeWidget, tr("&Fill and Stroke"));
  m_glyphMarkWidget->hide();

  // Setups initial graphic
  m_graphic->add(m_glyphMarkWidget->getMark());

  // Signals & slots
  connect(m_glyphMarkWidget, SIGNAL(markChanged()), SLOT(onMarkChanged()));
}

te::qt::widgets::GlyphGraphicWidget::~GlyphGraphicWidget()
{
}

bool te::qt::widgets::GlyphGraphicWidget::setGraphic(const te::se::Graphic* graphic)
{
  GraphicWidget::setGraphic(graphic);

  // Verifying if this widget can deal with the given graphic...
  const std::vector<te::se::Mark*> marks = m_graphic->getMarks();
  if(marks.empty())
    return false;

  te::se::Mark* mark = marks[0];
  const std::string* name = mark->getWellKnownName();
  if(name == 0)
    return false;

  std::size_t found = name->find("ttf://");
  if(found == std::string::npos)
    return false;

  // I know it!
  m_glyphMarkWidget->setMark(marks[0]);

  return true;
}

QString te::qt::widgets::GlyphGraphicWidget::getGraphicType() const
{
  return tr("Glyph Marker");
}

void te::qt::widgets::GlyphGraphicWidget::onMarkChanged()
{
  m_graphic->setMark(0, m_glyphMarkWidget->getMark());
  emit graphicChanged();
}
