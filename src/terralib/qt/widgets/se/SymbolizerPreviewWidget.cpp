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
  \file terralib/qt/widgets/se/SymbolizerPreviewWidget.cpp

  \brief A widget used to preview a symbolizer element.
*/

// TerraLib
#include "../../../geometry/Line.h"
#include "../../../geometry/LinearRing.h"
#include "../../../geometry/Point.h"
#include "../../../geometry/Polygon.h"
#include "../../../maptools/CanvasConfigurer.h"
#include "../../../se/Symbolizer.h"
#include "../canvas/Canvas.h"
#include "SymbologyPreview.h"
#include "SymbolizerPreviewWidget.h"

// Qt
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

// STL
#include <cassert>

te::qt::widgets::SymbolizerPreviewWidget::SymbolizerPreviewWidget(const QSize& size, const te::se::SymbolizerType& type, QWidget* parent)
  : QWidget(parent),
    m_geom(0),
    m_size(size)
{
  // Building the geometry to preview
  setSymbolizerType(type);

  // Label preview
  m_previewLabel = new QLabel(this);
  m_previewLabel->setFixedSize(m_size);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->addWidget(m_previewLabel);
}

te::qt::widgets::SymbolizerPreviewWidget::~SymbolizerPreviewWidget()
{
  delete m_geom;
}

void te::qt::widgets::SymbolizerPreviewWidget::updatePreview(te::se::Symbolizer* symb)
{
  m_previewLabel->setPixmap(te::qt::widgets::SymbologyPreview::build(symb, m_geom, m_size));
}

void te::qt::widgets::SymbolizerPreviewWidget::updatePreview(const std::vector<te::se::Symbolizer*>& symbs)
{
  m_previewLabel->setPixmap(te::qt::widgets::SymbologyPreview::build(symbs, m_geom, m_size));
}

void te::qt::widgets::SymbolizerPreviewWidget::setSymbolizerType(const te::se::SymbolizerType& type)
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
      te::gm::LineString* line = new te::gm::LineString(4, te::gm::LineStringType);
      line->setPoint(0, offset, offset);
      line->setPoint(1, m_size.width() - offset, offset);
      line->setPoint(2, offset, m_size.height() - offset);
      line->setPoint(3, m_size.width() - offset, m_size.height() - offset);
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
    break;
      
    case te::se::RASTER_SYMBOLIZER:
    case te::se::TEXT_SYMBOLIZER:
    {}
    break;
  }
}
