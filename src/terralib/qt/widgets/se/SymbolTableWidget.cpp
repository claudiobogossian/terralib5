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
  \file terralib/qt/widgets/se/SymbolTableWidget.cpp

  \brief A widget used to preview a symbol as separated layers.
*/

// TerraLib
#include "Symbol.h"
#include "SymbologyPreview.h"
#include "SymbolTableWidget.h"

// Qt
#include <QGridLayout>
#include <QHeaderView>
#include <QTableWidget>

// STL
#include <cassert>

te::qt::widgets::SymbolTableWidget::SymbolTableWidget(const QSize& size, QWidget* parent)
  : QWidget(parent),
    m_size(size)
{
  // Table preview
  m_previewTable = new QTableWidget(this);
  m_previewTable->setColumnCount(1);
#if (QT_VERSION >= 0x050000)
  m_previewTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
#else
  m_previewTable->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
#endif
  m_previewTable->horizontalHeader()->setDefaultSectionSize(m_size.width());
  m_previewTable->horizontalHeader()->hide();
#if (QT_VERSION >= 0x050000)
  m_previewTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
#else
  m_previewTable->verticalHeader()->setResizeMode(QHeaderView::Fixed);
#endif
  m_previewTable->verticalHeader()->setDefaultSectionSize(m_size.height());
  m_previewTable->setSelectionMode(QAbstractItemView::SingleSelection);
  m_previewTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_previewTable->setIconSize(m_size);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_previewTable);

  // Signals & slots
  connect(m_previewTable, SIGNAL(itemSelectionChanged()), SLOT(onPreviewTableItemSelectionChanged()));
}

te::qt::widgets::SymbolTableWidget::~SymbolTableWidget()
{
}

void te::qt::widgets::SymbolTableWidget::updatePreview(Symbol* symbol)
{
  assert(symbol);

  std::size_t nSymbolizers = symbol->getSymbolizersCount();

  m_previewTable->setRowCount(nSymbolizers);

  for(std::size_t i = 0; i < nSymbolizers; ++i)
  {
    QTableWidgetItem* item = new QTableWidgetItem(te::qt::widgets::SymbologyPreview::build(symbol->getSymbolizer(i), m_size), "");
    m_previewTable->setItem(i, 0, item);
  }
}

void te::qt::widgets::SymbolTableWidget::selectSymbolizer(const int& index)
{
  m_previewTable->selectRow(index);
}

QSize te::qt::widgets::SymbolTableWidget::sizeHint() const
{
  return QSize(m_size.width() + m_previewTable->verticalHeader()->size().width(), m_size.height() * 4);
}

void te::qt::widgets::SymbolTableWidget::onPreviewTableItemSelectionChanged()
{
  emit symbolizerClicked(m_previewTable->currentRow());
}
