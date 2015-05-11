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
  \file SelectionTool.cpp

  \brief An example of MapDisplay Tool. The only purpose of this tool is to show how you can implement a new tool. Do not consider it as a final application.
*/

// Example
#include "SelectionTool.h"

// TerraLib
#include <terralib/common/STLUtils.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/maptools/DataSetLayer.h>
#include <terralib/qt/widgets/canvas/Canvas.h>
#include <terralib/qt/widgets/canvas/MapDisplay.h>

// Qt
#include <QMouseEvent>
#include <QToolTip>

// STL
#include <cassert>
#include <memory>

SelectionTool::SelectionTool(te::qt::widgets::MapDisplay* display, te::map::DataSetLayer* layer, QObject* parent) 
  : te::qt::widgets::AbstractTool(display, parent)
{
  assert(layer);

  m_layer = dynamic_cast<te::map::DataSetLayer*>(layer);
  assert(m_layer);

  // Signals & slots
  connect(m_display, SIGNAL(extentChanged()), SLOT(onExtentChanged()));
}

SelectionTool::~SelectionTool()
{
  te::common::FreeContents(m_geoms);

  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);
}

bool SelectionTool::mouseReleaseEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  // Converts clicked point to world coordinates
#if QT_VERSION >= 0x050000
  QPointF qpoint = m_display->transform(e->localPos());
#else
  QPointF qpoint = m_display->transform(e->posF());
#endif

  // Get the datasource and the transactor
  te::da::DataSourcePtr dataSource = te::da::DataSourceManager::getInstance().find(m_layer->getDataSourceId());
  std::auto_ptr<te::da::DataSourceTransactor> transactor(dataSource->getTransactor());

  // Bulding restriction geometry
  te::gm::Point point(qpoint.x(), qpoint.y());

  // Gets the dataset
  std::auto_ptr<te::da::DataSetType> datasetType = transactor->getDataSetType(m_layer->getDataSetName());
  te::dt::Property* p = datasetType->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE);
  std::string propname = p->getName();

  std::auto_ptr<te::da::DataSet> dataset = transactor->getDataSet(m_layer->getDataSetName(),propname, &point, te::gm::INTERSECTS);
  
  // Clear the geometries
  te::common::FreeContents(m_geoms);
  m_geoms.clear();

  // For feature attributes
  std::size_t nproperties = dataset->getNumProperties();
  QString information("<h2>Attributes</h2><ul>");

  // Find the geometries that really intersect the clicked point
  while(dataset->moveNext())
  {
    std::size_t pos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::GEOMETRY_TYPE);
    std::auto_ptr<te::gm::Geometry> g = dataset->getGeometry(pos);
    if(g->intersects(&point))
    {
      // Stores the geometry
      m_geoms.push_back(g.get());

      // Building the features attributes text
      for(std::size_t i = 0; i < nproperties; ++i)
      {
        int propertyType = dataset->getPropertyDataType(i);
        if(propertyType != te::dt::GEOMETRY_TYPE)
          information += "<li><b>" + QString::fromStdString(dataset->getPropertyName(i)) + ":</b> " + QString::fromStdString(dataset->getAsString(i, 3)) + "</li>";
      }
    }
  }
  information += "</ul>";

  // Draws the selected geometries
  drawGeometries();

  // Show attributes
  if(!m_geoms.empty())
    QToolTip::showText(QCursor::pos(), information, m_display, m_display->rect());

  return true;
}

void SelectionTool::drawGeometries()
{
  // Clear!
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  // Prepares the canvas
  te::gm::Envelope env(m_display->getExtent());
  te::qt::widgets::Canvas canvas(draft);
  canvas.setWindow(env.m_llx, env.m_lly, env.m_urx, env.m_ury);
  canvas.setPolygonFillColor(te::color::RGBAColor(0, 200, 0, TE_OPAQUE));
  canvas.setPolygonContourColor(te::color::RGBAColor(0, 120, 0, TE_OPAQUE));
  canvas.setPolygonContourWidth(3);

  // Let's draw!
  for(std::size_t i = 0; i < m_geoms.size(); ++i)
    canvas.draw(m_geoms[i]);

  // Updates the display
  m_display->repaint();
}

void SelectionTool::onExtentChanged()
{
  drawGeometries();
}
