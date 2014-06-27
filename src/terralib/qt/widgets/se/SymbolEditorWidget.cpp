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
  \file terralib/qt/widgets/se/SymbolEditorWidget.cpp

  \brief A widget used to edit symbols.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../se/Symbolizer.h"
#include "LineSymbolizerWidget.h"
#include "PointSymbolizerWidget.h"
#include "PolygonSymbolizerWidget.h"
#include "Symbol.h"
#include "SymbolEditorWidget.h"
#include "SymbolPreviewWidget.h"
#include "SymbolTableWidget.h"
#include "ui_SymbolEditorWidgetForm.h"

// Qt
#include <QStackedWidget>

// STL
#include <cassert>

te::qt::widgets::SymbolEditorWidget::SymbolEditorWidget(const SymbolType& type, QWidget* parent)
  : QWidget(parent),
    m_ui(new Ui::SymbolEditorWidgetForm),
    m_symbol(new Symbol),
    m_type(type)
{
  m_ui->setupUi(this);

  // Preview
  m_preview = new SymbolPreviewWidget(QSize(120, 120), this);

  // Adjusting...
  QGridLayout* previewLayout = new QGridLayout(m_ui->m_previewGroupBox);
  previewLayout->addWidget(m_preview);

  // Layers
  m_symbolTable = new SymbolTableWidget(QSize(120, 42), this);
  m_symbolTable->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

  // Adjusting...
  QGridLayout* symbolTableLayout = new QGridLayout(m_ui->m_symbolTableFrame);
  symbolTableLayout->setContentsMargins(0, 0, 0, 0);
  symbolTableLayout->addWidget(m_symbolTable);

  // Stacked symbolizers widgets
  m_symbolizersStackedWidget = new QStackedWidget(this);

  // Adjusting...
  QGridLayout* stackedLayout = new QGridLayout(m_ui->m_propertiesGroupBox);
  stackedLayout->addWidget(m_symbolizersStackedWidget);

  initialize();

  // Signals & slots
  connect(m_ui->m_addToolButton, SIGNAL(clicked()), SLOT(onAddToolButtonClicked()));
  connect(m_ui->m_removeToolButton, SIGNAL(clicked()), SLOT(onRemoveToolButtonClicked()));
  connect(m_ui->m_upToolButton, SIGNAL(clicked()), SLOT(onUpToolButtonClicked()));
  connect(m_ui->m_downToolButton, SIGNAL(clicked()), SLOT(onDownToolButtonClicked()));
  connect(m_symbolTable, SIGNAL(symbolizerClicked(int)), m_symbolizersStackedWidget, SLOT(setCurrentIndex(int)));
}

te::qt::widgets::SymbolEditorWidget::~SymbolEditorWidget()
{
  delete m_symbol;
}

void te::qt::widgets::SymbolEditorWidget::initialize()
{
  // Creating the first symbolizer
  createNewSymbolizer();

  // Setup icons
  m_ui->m_addToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeToolButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_upToolButton->setIcon(QIcon::fromTheme("go-up"));
  m_ui->m_downToolButton->setIcon(QIcon::fromTheme("go-down"));
}

void te::qt::widgets::SymbolEditorWidget::updateUi()
{
  m_preview->updatePreview(m_symbol);
  m_symbolTable->updatePreview(m_symbol);
}

void te::qt::widgets::SymbolEditorWidget::createNewSymbolizer()
{
  QWidget* symbolizerWidget = 0;
  switch(m_type)
  {
    case PointSymbol:
    {
      PointSymbolizerWidget* pts = new PointSymbolizerWidget;
      m_symbol->addSymbolizer(pts->getSymbolizer());
      symbolizerWidget = pts;
    }
    break;

    case LineSymbol:
    {
      LineSymbolizerWidget* ls = new LineSymbolizerWidget;
      m_symbol->addSymbolizer(ls->getSymbolizer());
      symbolizerWidget = ls;
    }
    break;

    case PolygonSymbol:
    {
      PolygonSymbolizerWidget* ps = new PolygonSymbolizerWidget;
      m_symbol->addSymbolizer(ps->getSymbolizer());
      symbolizerWidget = ps;
    }
    break;
  }
  // Adding on stack
  m_symbolizersStackedWidget->addWidget(symbolizerWidget);
  // Connecting signal & slot
  connect(symbolizerWidget, SIGNAL(symbolizerChanged()), SLOT(onSymbolizerChanged()));
  // Makes this new widget the current
  m_symbolizersStackedWidget->setCurrentIndex(m_symbolizersStackedWidget->count() - 1);

  // Updating preview
  updateUi();

  // Select on symbolizer table
  m_symbolTable->selectSymbolizer(m_symbol->getSymbolizersCount() - 1);
}

void te::qt::widgets::SymbolEditorWidget::swapSymbolizers(const int& indexFirst, const int& indexSecond)
{
  // Swap symbolizers
  m_symbol->swapSymbolizers(indexFirst, indexSecond);

  // Swap widget symbolizers
  QWidget* firstWidget = m_symbolizersStackedWidget->widget(indexFirst);
  QWidget* secondWidget = m_symbolizersStackedWidget->widget(indexSecond);
  m_symbolizersStackedWidget->insertWidget(indexFirst, secondWidget);
  m_symbolizersStackedWidget->insertWidget(indexSecond, firstWidget);
}

void te::qt::widgets::SymbolEditorWidget::onAddToolButtonClicked()
{
  createNewSymbolizer();
}

void te::qt::widgets::SymbolEditorWidget::onRemoveToolButtonClicked()
{
  if(m_symbol->getSymbolizersCount() == 1) // We have only one symbolizer. No remove!
    return;

  QWidget* w = m_symbolizersStackedWidget->currentWidget();

  int index  = m_symbolizersStackedWidget->indexOf(w);
  assert(index >= 0 && index < static_cast<int>(m_symbol->getSymbolizersCount()));

  // Removing symbolizer
  m_symbol->removeSymbolizer(index);
  
  // Removing widget associated with the removed symbolizer
  m_symbolizersStackedWidget->removeWidget(w);
  delete w;

  updateUi();

  // Selection to upper symbolizer...
  m_symbolTable->selectSymbolizer(index - 1);
}

void te::qt::widgets::SymbolEditorWidget::onUpToolButtonClicked()
{
  int index  = m_symbolizersStackedWidget->currentIndex();
  assert(index >= 0 && index < static_cast<int>(m_symbol->getSymbolizersCount()));
  
  if(index == 0) // Not go-up a top symbolizer!
    return;

  // Swap
  swapSymbolizers(index, index - 1);

  updateUi();

  // Selection to upper symbolizer...
  m_symbolTable->selectSymbolizer(index - 1);
}

void te::qt::widgets::SymbolEditorWidget::onDownToolButtonClicked()
{
  int index  = m_symbolizersStackedWidget->currentIndex();
  assert(index >= 0 && index < static_cast<int>(m_symbol->getSymbolizersCount()));
  
  if(index == m_symbol->getSymbolizersCount() - 1) // Not go-down a down symbolizer!
    return;

  // Swap
  swapSymbolizers(index, index + 1);

  updateUi();

  // Selection to down symbolizer...
  m_symbolTable->selectSymbolizer(index + 1);
}

void te::qt::widgets::SymbolEditorWidget::onSymbolizerChanged()
{
  /* (Uba, 29/06/2012)
     TODO: We can pass the new symbolizer on signal?!
     So, we'll have:
     te::qt::widgets::SymbolEditorWidget::onSymbolizerChanged(te::se::Symbolizer* symb)
     {
       int index = m_symbolizersStackedWidget->currentIndex();
       assert(index >= 0 && index < static_cast<int>(m_symbs.size()));
       m_symbol->setSymbolizer(index, symb);
     }
  */

  int index = m_symbolizersStackedWidget->currentIndex();
  assert(index >= 0 && index < static_cast<int>(m_symbol->getSymbolizersCount()));

  QWidget* w = m_symbolizersStackedWidget->currentWidget();
  switch(m_type)
  {
    case PointSymbol:
      m_symbol->setSymbolizer(index, static_cast<PointSymbolizerWidget*>(w)->getSymbolizer());
    break;

    case LineSymbol:
      m_symbol->setSymbolizer(index, static_cast<LineSymbolizerWidget*>(w)->getSymbolizer());
    break;

    case PolygonSymbol:
      m_symbol->setSymbolizer(index, static_cast<PolygonSymbolizerWidget*>(w)->getSymbolizer());
    break;
  }
  updateUi();
}
