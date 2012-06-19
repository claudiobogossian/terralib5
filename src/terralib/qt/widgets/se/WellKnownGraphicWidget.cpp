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
  \file terralib/qt/widgets/se/WellKnownGraphicWidget.cpp

  \brief A widget used to build a graphic associate with a well-known mark element.
*/

// TerraLib
#include "../../../se/Graphic.h"
#include "../../../se/Mark.h"
#include "WellKnownGraphicWidget.h"
#include "WellKnownMarkWidget.h"
#include "ui_GraphicWidgetForm.h"

// Qt
#include <QtGui/QGridLayout>

te::qt::widgets::WellKnownGraphicWidget::WellKnownGraphicWidget(QWidget* parent, Qt::WindowFlags f)
  : GraphicWidget(parent, f)
{
  // Well known mark Widget
  m_markWidget = new te::qt::widgets::WellKnownMarkWidget(this);

  // Well known mark Page
  QWidget* markPage = new QWidget(this);
  
  // Adjusting...
  QGridLayout* layout = new QGridLayout(markPage);
  layout->addWidget(m_markWidget);

  // Adding on graphic tab...
  m_ui->m_graphicTabWidget->addTab(markPage, tr("&Mark"));

  // Setups initial graphic
  m_graphic->add(m_markWidget->getMark());

  // Signals & slots
  connect(m_markWidget, SIGNAL(markChanged()), SLOT(onMarkChanged()));
}

te::qt::widgets::WellKnownGraphicWidget::~WellKnownGraphicWidget()
{
}

bool te::qt::widgets::WellKnownGraphicWidget::setGraphic(const te::se::Graphic* graphic)
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

  std::size_t found = name->find("://");
  if(found != std::string::npos)
    return false;

  // I know it!
  m_markWidget->setMark(marks[0]);

  return true;
}

QString te::qt::widgets::WellKnownGraphicWidget::getGraphicType() const
{
  return tr("Well Known Marker");
}

void te::qt::widgets::WellKnownGraphicWidget::onMarkChanged()
{
  m_graphic->setMark(0, m_markWidget->getMark());
  emit graphicChanged();
}
