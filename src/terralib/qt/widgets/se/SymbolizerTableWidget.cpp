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
  \file terralib/qt/widgets/se/SymbolizerTableWidget.cpp

  \brief A widget used to preview a set of symbolizer elements as separated visual layers.
*/

// TerraLib
#include "../../../geometry/Line.h"
#include "../../../geometry/LinearRing.h"
#include "../../../geometry/Point.h"
#include "../../../geometry/Polygon.h"
#include "../../../maptools/CanvasConfigurer.h"
#include "../../../se/Symbolizer.h"
#include "../canvas/Canvas.h"
#include "StylePreview.h"
#include "SymbolizerTableWidget.h"

// Qt
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTableWidget>

// STL
#include <cassert>

te::qt::widgets::SymbolizerTableWidget::SymbolizerTableWidget(const QSize& size, const te::se::SymbolizerType& type, QWidget* parent)
  : QWidget(parent),
    m_geom(0),
    m_size(size)
{
  // Building the geometry to preview
  setSymbolizerType(type);

  // Table preview
  m_previewTable = new QTableWidget(this);
  m_previewTable->setColumnCount(1);
  m_previewTable->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
  m_previewTable->horizontalHeader()->setDefaultSectionSize(m_size.width());
  m_previewTable->horizontalHeader()->hide();
  m_previewTable->verticalHeader()->setResizeMode(QHeaderView::Fixed);
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

te::qt::widgets::SymbolizerTableWidget::~SymbolizerTableWidget()
{
  delete m_geom;
}

void te::qt::widgets::SymbolizerTableWidget::updatePreview(const std::vector<te::se::Symbolizer*>& symbs)
{
  assert(!symbs.empty());

  m_previewTable->setRowCount(symbs.size());
  for(std::size_t i = 0; i < symbs.size(); ++i)
  {
    QTableWidgetItem* item = new QTableWidgetItem(te::qt::widgets::StylePreview::build(symbs[i], m_geom, m_size), "");
    m_previewTable->setItem(i, 0, item);
  }
}

void te::qt::widgets::SymbolizerTableWidget::setSymbolizerType(const te::se::SymbolizerType& type)
{
  delete m_geom;

  // Offset to adjust geometries on preview size
  double offset = 10.0;

  // Building...
  switch(type)
  {
    case te::se::POINT_SYMBOLIZER:
      m_geom = new te::gm::Point(m_size.width() * 0.5, m_size.height() * 0.5);
    break;

    case te::se::LINE_SYMBOLIZER:
    {
      te::gm::LineString* line = new te::gm::LineString(2, te::gm::LineStringType);
      line->setPoint(0, offset, m_size.height() * 0.5);
      line->setPoint(1, m_size.width() - offset, m_size.height() * 0.5);
      m_geom = line;
    }
    break;

    case te::se::POLYGON_SYMBOLIZER:
    {
      te::gm::Polygon* polygon = new te::gm::Polygon(1, te::gm::PolygonType);
      te::gm::LinearRing* ring = new te::gm::LinearRing(5, te::gm::LineStringType);
      ring->setPoint(0, offset, offset);
      ring->setPoint(1, m_size.width() - offset, offset);
      ring->setPoint(2, m_size.width() - offset, m_size.height() - offset);
      ring->setPoint(3, offset, m_size.height() - offset);
      ring->setPoint(4, offset, offset);
      polygon->setRingN(0, ring);
      m_geom = polygon;
    }
  }
}

void te::qt::widgets::SymbolizerTableWidget::selectSymbolizer(const int& index)
{
  m_previewTable->selectRow(index);
}

QSize te::qt::widgets::SymbolizerTableWidget::sizeHint() const
{
  return QSize(m_size.width() + m_previewTable->verticalHeader()->size().width(), m_size.height() * 4);
}

void te::qt::widgets::SymbolizerTableWidget::onPreviewTableItemSelectionChanged()
{
  emit symbolizerClicked(m_previewTable->currentRow());
}
