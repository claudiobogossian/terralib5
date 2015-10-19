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
  \file terralib/qt/widgets/se/SymbolPreviewWidget.cpp

  \brief A widget used to preview symbol elements.
*/

// TerraLib
#include "../../../geometry/Line.h"
#include "../../../geometry/LinearRing.h"
#include "../../../geometry/Point.h"
#include "../../../geometry/Polygon.h"
#include "../../../maptools/CanvasConfigurer.h"
#include "../canvas/Canvas.h"
#include "Symbol.h"
#include "SymbologyPreview.h"
#include "SymbolPreviewWidget.h"

// Qt
#include <QGridLayout>
#include <QLabel>

// STL
#include <cassert>

te::qt::widgets::SymbolPreviewWidget::SymbolPreviewWidget(const QSize& size, QWidget* parent)
  : QWidget(parent),
    m_geom(0),
    m_size(size)
{
  // Label preview
  m_previewLabel = new QLabel(this);
  m_previewLabel->setFixedSize(m_size);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->addWidget(m_previewLabel);
}

te::qt::widgets::SymbolPreviewWidget::~SymbolPreviewWidget()
{
  delete m_geom;
}

void te::qt::widgets::SymbolPreviewWidget::updatePreview(Symbol* symbol)
{
  updatePreview(symbol->getSymbolizers());
}

void te::qt::widgets::SymbolPreviewWidget::updatePreview(const std::vector<te::se::Symbolizer*>& symbolizers)
{
  m_previewLabel->setPixmap(te::qt::widgets::SymbologyPreview::build(symbolizers, m_size));
}

void te::qt::widgets::SymbolPreviewWidget::updatePreview(te::se::Symbolizer* symbolizer)
{
  m_previewLabel->setPixmap(te::qt::widgets::SymbologyPreview::build(symbolizer, m_size));
}

void te::qt::widgets::SymbolPreviewWidget::clear()
{
  QPixmap pix(m_size);
  pix.fill(Qt::transparent);

  m_previewLabel->setPixmap(pix);
}
