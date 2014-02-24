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
#include "Symbol.h"
#include "SymbolPreviewWidget.h"
#include "SymbolSelectorDialog.h"

// Qt
#include <QtGui/QMessageBox>

// STL
#include <cassert>

te::qt::widgets::StyleControllerWidget::StyleControllerWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::StyleControllerWidgetForm)
{
  m_ui->setupUi(this);

  // Add the StyleExplorer
  m_explorer = new te::qt::widgets::StyleExplorer(m_ui->m_explorerFrame);
  QGridLayout* le = new QGridLayout(m_ui->m_explorerFrame);
  le->setContentsMargins(0, 0, 0, 0);
  le->addWidget(m_explorer);

  // Signals & slots
  connect(m_ui->m_iconSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(changeLegendIconSize(int)));
  connect(m_ui->m_addSymbToolButton, SIGNAL(clicked()), this, SLOT(onAddSymbolizerClicked()));
  connect(m_ui->m_removeSymbToolButton, SIGNAL(clicked()), this, SLOT(onRemoveSymbolizerClicked()));
  connect(m_ui->m_upSymbToolButton, SIGNAL(clicked()), this, SLOT(onUpSymbolizerClicked()));
  connect(m_ui->m_downSymbToolButton, SIGNAL(clicked()), this, SLOT(onDownSymbolizerClicked()));
  connect(m_ui->m_libManagerToolButton, SIGNAL(clicked()), this, SLOT(onLibraryManagerClicked()));
  connect(m_ui->m_mapRefreshToolButton, SIGNAL(clicked()), this, SLOT(onMapRefreshClicked()));

  updateUi();
}

te::qt::widgets::StyleControllerWidget::~StyleControllerWidget()
{
}

void te::qt::widgets::StyleControllerWidget::setStyle(te::se::Style* style)
{
  m_explorer->setStyle(style);
}

te::qt::widgets::StyleExplorer* te::qt::widgets::StyleControllerWidget::getStyleExplorer() const
{
  return m_explorer;
}

void te::qt::widgets::StyleControllerWidget::updateUi()
{
  m_ui->m_upSymbToolButton->setIcon(QIcon::fromTheme("go-up").pixmap(16,16));
  m_ui->m_downSymbToolButton->setIcon(QIcon::fromTheme("go-down").pixmap(16,16));
  m_ui->m_addSymbToolButton->setIcon(QIcon::fromTheme("list-add").pixmap(16,16));
  m_ui->m_removeSymbToolButton->setIcon(QIcon::fromTheme("list-remove").pixmap(16,16));
  m_ui->m_libManagerToolButton->setIcon(QIcon::fromTheme("library").pixmap(16,16));
  m_ui->m_mapRefreshToolButton->setIcon(QIcon::fromTheme("map-draw").pixmap(16,16));
}

void te::qt::widgets::StyleControllerWidget::onAddSymbolizerClicked()
{
  te::se::Rule* rule = m_explorer->getCurrentRule();

  if(rule == 0)
  {
    QMessageBox::information(this, tr("Style"), tr("Select a rule first."));
    return;
  }

  te::se::Symbolizer* symb = m_explorer->getCurrentSymbolizer();
 
  if(symb)
    rule->push_back(symb->clone());
  else
  {
    assert(!rule->getSymbolizers().empty());

    const te::se::Symbolizer* symb = rule->getSymbolizer(0);
    assert(symb);

    rule->push_back(symb->clone());
  }

  m_explorer->updateStyleTree();
}

void te::qt::widgets::StyleControllerWidget::onRemoveSymbolizerClicked()
{
   te::se::Symbolizer* symb = m_explorer->getCurrentSymbolizer();

  if(symb == 0)
  {
    QMessageBox::information(this, tr("Style"), tr("Select a symbol first."));
    return;
  }

  te::se::Rule* rule = m_explorer->getCurrentRule();
  assert(rule);

  if(rule->getSymbolizers().size() == 1)
  {
    QMessageBox::information(this, tr("Style"), tr("The rule must have at least one symbol."));
    return;
  }

  for(std::size_t i = 0; i < rule->getSymbolizers().size(); ++i)
  {
    if(rule->getSymbolizer(i) == symb)
    {
      rule->removeSymbolizer(i);
      break;
    }
  }

  m_explorer->updateStyleTree();
}

void te::qt::widgets::StyleControllerWidget::onUpSymbolizerClicked()
{
  te::se::Symbolizer* symb = m_explorer->getCurrentSymbolizer();

  if(symb == 0)
  {
    QMessageBox::information(this, tr("Style"), tr("Select a symbol first."));
    return;
  }

  m_explorer->goUpSymbolizer();
}

void te::qt::widgets::StyleControllerWidget::onDownSymbolizerClicked()
{
  te::se::Symbolizer* symb = m_explorer->getCurrentSymbolizer();

  if(symb == 0)
  {
    QMessageBox::information(this, tr("Style"), tr("Select a symbol first."));
    return;
  }

  m_explorer->goDownSymbolizer();
}

void te::qt::widgets::StyleControllerWidget::onLibraryManagerClicked()
{
  te::se::Rule* rule = m_explorer->getCurrentRule();

  if(!rule)
  {
    QMessageBox::information(this, tr("Style"), tr("Select a rule first."));
    return;
  }

  Symbol* symbol = te::qt::widgets::SymbolSelectorDialog::getSymbol(this, tr("Symbol Selector"));
  if(symbol == 0)
    return;

  std::vector<te::se::Symbolizer*> symbolizers;
  for(std::size_t i = 0; i < symbol->getSymbolizersCount(); ++i)
    symbolizers.push_back(symbol->getSymbolizer(i)->clone());

  rule->setSymbolizers(symbolizers);

  m_explorer->updateStyleTree();
}

void te::qt::widgets::StyleControllerWidget::onMapRefreshClicked()
{
  emit mapRefresh();
}

void te::qt::widgets::StyleControllerWidget::changeLegendIconSize(int size)
{
  m_explorer->setLegendIconSize(size);
}
