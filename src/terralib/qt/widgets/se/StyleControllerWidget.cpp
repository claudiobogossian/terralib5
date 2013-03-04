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
  \file terralib/qt/widgets/se/StyleControllerWidget.cpp

  \brief A widget used to controll the style se element.
*/

// TerraLib
#include "../../../se/Rule.h"
#include "../../../se/Symbolizer.h"
#include "ui_StyleControllerWidgetForm.h"
#include "StyleControllerWidget.h"

#include "StyleExplorer.h"
#include "SymbolizerPreviewWidget.h"

// STL
#include <cassert>

te::qt::widgets::StyleControllerWidget::StyleControllerWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::StyleControllerWidgetForm)
{
  m_ui->setupUi(this);

  //add explorer
  m_explorer = new te::qt::widgets::StyleExplorer(m_ui->m_explorerTab);
  QGridLayout* le = new QGridLayout(m_ui->m_explorerTab);
  le->addWidget(m_explorer);

  connect(m_explorer, SIGNAL(symbolizerClicked(te::se::Symbolizer*)), this, SLOT(onSymbolizerClicked(te::se::Symbolizer*)));
  connect(m_explorer, SIGNAL(ruleClicked(const te::se::Rule*)), this, SLOT(onRuleClicked(const te::se::Rule*)));

  //add rule preview
  m_rulePreview = new te::qt::widgets::SymbolizerPreviewWidget(QSize(121,121), te::se::POLYGON_SYMBOLIZER, m_ui->m_ruleFrame);
  QGridLayout* lr = new QGridLayout(m_ui->m_ruleFrame);
  lr->setContentsMargins(0,0,0,0);
  lr->addWidget(m_rulePreview);

  //add symbolizer preview
  m_symbPreview = new te::qt::widgets::SymbolizerPreviewWidget(QSize(121,121), te::se::POLYGON_SYMBOLIZER, m_ui->m_symbFrame);
  QGridLayout* ls = new QGridLayout(m_ui->m_symbFrame);
  ls->setContentsMargins(0,0,0,0);
  ls->addWidget(m_symbPreview);

  //connect buttons
  connect(m_ui->m_addSymbToolButton, SIGNAL(clicked()), this, SLOT(onAddSymbolizerClicked()));
  connect(m_ui->m_delSymbToolButton, SIGNAL(clicked()), this, SLOT(onDelSymbolizerClicked()));
  connect(m_ui->m_upSymbToolButton, SIGNAL(clicked()), this, SLOT(onUpSymbolizerClicked()));
  connect(m_ui->m_downSymbToolButton, SIGNAL(clicked()), this, SLOT(onDownSymbolizerClicked()));

  m_ui->m_tabWidget->setCurrentWidget(m_ui->m_explorerTab);

  updateUi();
}

te::qt::widgets::StyleControllerWidget::~StyleControllerWidget()
{
}

void te::qt::widgets::StyleControllerWidget::setStyle(te::se::Style* style)
{
  m_explorer->setStyle(style);
}

void te::qt::widgets::StyleControllerWidget::updateUi()
{
  m_ui->m_upSymbToolButton->setIcon(QIcon::fromTheme("go-up").pixmap(16,16));
  m_ui->m_downSymbToolButton->setIcon(QIcon::fromTheme("go-down").pixmap(16,16));
  m_ui->m_addSymbToolButton->setIcon(QIcon::fromTheme("list-add").pixmap(16,16));
  m_ui->m_delSymbToolButton->setIcon(QIcon::fromTheme("list-remove").pixmap(16,16));
}

void te::qt::widgets::StyleControllerWidget::updateSymbolizerType(te::se::Symbolizer* s)
{
  if(s->getType() == "PolygonSymbolizer")
  {
    m_rulePreview->setSymbolizerType(te::se::POLYGON_SYMBOLIZER);
    m_symbPreview->setSymbolizerType(te::se::POLYGON_SYMBOLIZER);
  }
  else if(s->getType() == "LineSymbolizer")
  {
    m_rulePreview->setSymbolizerType(te::se::LINE_SYMBOLIZER);
    m_symbPreview->setSymbolizerType(te::se::LINE_SYMBOLIZER);
  }
  else if(s->getType() == "PointSymbolizer")
  {
    m_rulePreview->setSymbolizerType(te::se::POINT_SYMBOLIZER);
    m_symbPreview->setSymbolizerType(te::se::POINT_SYMBOLIZER);
  }
  else if(s->getType() == "RasterSymbolizer")
  {
    m_rulePreview->setSymbolizerType(te::se::RASTER_SYMBOLIZER);
    m_symbPreview->setSymbolizerType(te::se::RASTER_SYMBOLIZER);
  }
}

void te::qt::widgets::StyleControllerWidget::onAddSymbolizerClicked()
{
  te::se::Rule* rule = m_explorer->getCurrentRule();

  te::se::Symbolizer* symb = m_explorer->getCurrentSymbolizer();

  if(rule && symb)
  {
    rule->push_back(symb->clone());

    m_explorer->updateStyleTree();
  }
}

void te::qt::widgets::StyleControllerWidget::onDelSymbolizerClicked()
{
  // TODO
}

void te::qt::widgets::StyleControllerWidget::onUpSymbolizerClicked()
{
  m_explorer->goUpSymbolizer();
}

void te::qt::widgets::StyleControllerWidget::onDownSymbolizerClicked()
{
  m_explorer->goDownSymbolizer();
}

void te::qt::widgets::StyleControllerWidget::onSymbolizerChanged(te::se::Symbolizer* s)
{
  m_explorer->onSymbolizerChanged(s);
  m_symbPreview->updatePreview(s);

  te::se::Rule* rule = m_explorer->getCurrentRule();

  if(rule)
    onRuleClicked(rule);
}

void te::qt::widgets::StyleControllerWidget::onSymbolizerClicked(te::se::Symbolizer* symb)
{
  updateSymbolizerType(symb);

  m_symbPreview->updatePreview(symb);

  te::se::Rule* rule = m_explorer->getCurrentRule();

  if(rule)
    onRuleClicked(rule);

  emit symbolizerSelected(symb);
}

void te::qt::widgets::StyleControllerWidget::onRuleClicked(const te::se::Rule* rule)
{
  if(rule->getSymbolizers().empty() ==  false)
  {
    updateSymbolizerType(rule->getSymbolizers()[0]);

    m_rulePreview->updatePreview(rule->getSymbolizers());
  }
}
