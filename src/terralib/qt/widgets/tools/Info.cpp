/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/tools/Info.cpp

  \brief This class implements a concrete tool to get informations about a specified feature using pointing operation.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../geometry/Point.h"
#include "../../../geometry/Utils.h"
#include "../../../maptools/MarkRendererManager.h"
#include "../../../raster/Raster.h"
#include "../../../raster/RasterProperty.h"
#include "../../../raster/Utils.h"
#include "../../../se/Fill.h"
#include "../../../se/Stroke.h"
#include "../../../se/Mark.h"
#include "../../../se/Utils.h"
#include "../../../srs/Config.h"
#include "../canvas/Canvas.h"
#include "../canvas/MapDisplay.h"
#include "../Utils.h"
#include "Info.h"

// Qt
#include <QtCore/QPointF>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtGui/QMessageBox>
#include <QtGui/QMouseEvent>

// STL
#include <cassert>
#include <memory>

te::qt::widgets::Info::Info(te::qt::widgets::MapDisplay* display, const QCursor& cursor, const std::list<te::map::AbstractLayerPtr>& layers, QObject* parent) 
  : AbstractTool(display, parent),
    m_layers(layers),
    m_infoWidget(new QTreeWidget(display))
{
  setCursor(cursor);

  // Setup the widget that will be used to show the informations
  m_infoWidget->setWindowTitle(tr("Information"));
  m_infoWidget->setWindowFlags(Qt::Tool);
  m_infoWidget->setAlternatingRowColors(true);
  m_infoWidget->setMinimumSize(300, 300);
  m_infoWidget->setColumnCount(2);

  QStringList labels;
  labels << tr("Property") << tr("Value");
  m_infoWidget->setHeaderLabels(labels);
}

te::qt::widgets::Info::~Info()
{
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  m_infoWidget->close();

  delete m_infoWidget;
}

bool te::qt::widgets::Info::mouseReleaseEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  if(m_layers.empty())
    return false;

  QPointF pixelOffset(4.0, 4.0);
  QRectF rect = QRectF(e->posF() - pixelOffset, e->posF() + pixelOffset);

  // Converts rect boundary to world coordinates
  QPointF ll(rect.left(), rect.bottom());
  QPointF ur(rect.right(), rect.top());
  ll = m_display->transform(ll);
  ur = m_display->transform(ur);

  // Bulding the query box
  te::gm::Envelope envelope(ll.x(), ll.y(), ur.x(), ur.y());

  // Clear draft!
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  // Clear info widget!
  m_infoWidget->clear();

  // For each layer, get info!
  std::list<te::map::AbstractLayerPtr>::const_iterator it;
  for(it = m_layers.begin(); it != m_layers.end(); ++it)
    getInfo(*it, envelope);

  if(m_infoWidget->topLevelItemCount() > 0)
  {
    m_infoWidget->expandAll();
    m_infoWidget->resizeColumnToContents(0);
    m_infoWidget->show();
  }
  else
    m_infoWidget->hide();

  m_display->repaint();

  return true;
}

void te::qt::widgets::Info::setLayers(const std::list<te::map::AbstractLayerPtr>& layers)
{
  m_layers = layers;
}

void te::qt::widgets::Info::getInfo(const te::map::AbstractLayerPtr& layer, const te::gm::Envelope& e)
{
  if(layer->getVisibility() != te::map::VISIBLE)
    return;

  bool needRemap = false;
  te::gm::Envelope reprojectedEnvelope(e);

  if((layer->getSRID() != TE_UNKNOWN_SRS) &&
     (m_display->getSRID() != TE_UNKNOWN_SRS) &&
     (layer->getSRID() != m_display->getSRID()))
  {
    needRemap = true;
    reprojectedEnvelope.transform(m_display->getSRID(), layer->getSRID());
  }

  if(!reprojectedEnvelope.within(layer->getExtent()))
    return;

  // The Layer item
  QTreeWidgetItem* layerItem = new QTreeWidgetItem;
  layerItem->setIcon(0, QIcon::fromTheme("layer"));
  layerItem->setText(0, tr("Layer"));
  layerItem->setText(1, layer->getTitle().c_str());

  try
  {
    // Gets the Layer Schema
    std::auto_ptr<const te::map::LayerSchema> ls(layer->getSchema());

    // Gets the name of the referenced spatial property
    std::string spatialPropertyName = layer->getGeomPropertyName();

    if(ls->hasGeom())
    {
      te::gm::GeometryProperty* gp = 0;

      if(spatialPropertyName.empty())
        gp = te::da::GetFirstGeomProperty(ls.get());
     else
        gp = dynamic_cast<te::gm::GeometryProperty*>(ls->getProperty(spatialPropertyName));

      assert(gp);

      // Retrieves the data from layer
      std::auto_ptr<te::da::DataSet> dataset(layer->getData(gp->getName(), &reprojectedEnvelope, te::gm::INTERSECTS).release());

      getGeometryInfo(layerItem, dataset.get(), layer->getGeomPropertyName(), reprojectedEnvelope, layer->getSRID(), needRemap);
    }

    if(ls->hasRaster())
    {
      te::rst::RasterProperty* rp = 0;

      if(spatialPropertyName.empty())
        rp = te::da::GetFirstRasterProperty(ls.get());
      else
        rp = dynamic_cast<te::rst::RasterProperty*>(ls->getProperty(spatialPropertyName));

      assert(rp);

      // Retrieves the data from layer
      std::auto_ptr<te::da::DataSet> dataset(layer->getData(rp->getName(), &reprojectedEnvelope, te::gm::INTERSECTS).release());

      if(!dataset->moveNext())
        return;

      std::size_t rpos = te::da::GetPropertyPos(dataset.get(), spatialPropertyName);
      assert(rpos != std::string::npos);

      std::auto_ptr<te::rst::Raster> raster(dataset->getRaster(rpos));
      assert(raster.get());

      getRasterInfo(layerItem, raster.get(), reprojectedEnvelope, layer->getSRID(), needRemap);
    }

    layerItem->childCount() != 0 ? m_infoWidget->addTopLevelItem(layerItem) : delete layerItem;
  }
  catch(...)
  {
    delete layerItem;
    // TODO: catch the exceptions...
  }
}

void te::qt::widgets::Info::getGeometryInfo(QTreeWidgetItem* layerItem, te::da::DataSet* dataset, const std::string& geomPropertyName, const te::gm::Envelope& e, int srid, bool needRemap)
{
  // Generates a geometry from the given extent. It will be used to refine the results
  std::auto_ptr<te::gm::Geometry> geometryFromEnvelope(te::gm::GetGeomFromEnvelope(&e, srid));

  // The restriction point. It will be used to refine the results
  te::gm::Coord2D center = e.getCenter();
  te::gm::Point point(center.x, center.y, srid);

  std::size_t gpos = std::string::npos;
  geomPropertyName.empty() ? gpos = te::da::GetFirstPropertyPos(dataset, te::dt::GEOMETRY_TYPE): gpos = te::da::GetPropertyPos(dataset, geomPropertyName);

  while(dataset->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> g(dataset->getGeometry(gpos));

    if(g->contains(&point) || g->crosses(geometryFromEnvelope.get()) || geometryFromEnvelope->contains(g.get()))
    {
      // Feature found! Building the list of property values...
      for(std::size_t i = 0; i < dataset->getNumProperties(); ++i)
      {
        if(dataset->getPropertyDataType(i) == te::dt::RASTER_TYPE)
          continue;

        QTreeWidgetItem* propertyItem = new QTreeWidgetItem(layerItem);
        propertyItem->setText(0, dataset->getPropertyName(i).c_str());

        if(dataset->getPropertyDataType(i) == te::dt::GEOMETRY_TYPE)
          propertyItem->setIcon(0, QIcon::fromTheme("geometry"));

        if(!dataset->isNull(i))
        {
          QString qvalue;

          if(dataset->getPropertyDataType(i) == te::dt::STRING_TYPE)
          {
            std::string value = dataset->getString(i);
            te::common::CharEncoding encoding = dataset->getPropertyCharEncoding(i);
            qvalue = Convert2Qt(value, encoding);
          }
          else
            qvalue = dataset->getAsString(i, 3).c_str();

          propertyItem->setText(1, qvalue);
        }
        else // property null value!
          propertyItem->setText(1, "");
      }

      if(needRemap)
      {
        g->setSRID(srid);
        g->transform(m_display->getSRID());
      }

      drawGeometry(g.get()); // to show feedback!

      break;
    }
  }
}

void te::qt::widgets::Info::getRasterInfo(QTreeWidgetItem* layerItem, te::rst::Raster* raster, const te::gm::Envelope& e, int /*srid*/, bool /*needRemap*/)
{
  te::gm::Coord2D center = e.getCenter();
  te::gm::Coord2D coord = raster->getGrid()->geoToGrid(center.x, center.y);

  int x = te::rst::Round(coord.x);
  int y = te::rst::Round(coord.y);

  if((x < 0 && x >= static_cast<int>(raster->getNumberOfColumns())) ||
     (y < 0 && y >= static_cast<int>(raster->getNumberOfRows())))
     return;

  // Raster location informations
  QTreeWidgetItem* lineItem = new QTreeWidgetItem(layerItem);
  lineItem->setText(0, tr("Line"));
  lineItem->setText(1, QString::number(y));

  QTreeWidgetItem* columnItem = new QTreeWidgetItem(layerItem);
  columnItem->setText(0, tr("Column"));
  columnItem->setText(1, QString::number(x));

  QTreeWidgetItem* coordXItem = new QTreeWidgetItem(layerItem);
  coordXItem->setText(0, tr("Coordinate X"));
  coordXItem->setText(1, QString::number(center.x));

  QTreeWidgetItem* coordYItem = new QTreeWidgetItem(layerItem);
  coordYItem->setText(0, tr("Coordinate Y"));
  coordYItem->setText(1, QString::number(center.y));

  // Raster value informations for each band
  double value = 0.0;
  for(std::size_t b = 0; b < raster->getNumberOfBands(); ++b)
  {
    raster->getValue(x, y, value, b);

    QTreeWidgetItem* bandValueItem = new QTreeWidgetItem(layerItem);
    bandValueItem->setIcon(0, QIcon::fromTheme("channel-gray"));
    bandValueItem->setText(0, tr("Band") + " " + QString::number(b));
    bandValueItem->setText(1, QString::number(value));
  }
}

void te::qt::widgets::Info::drawGeometry(te::gm::Geometry* g)
{
  QPixmap* draft = m_display->getDraftPixmap();

  const te::gm::Envelope& displayExtent = m_display->getExtent();

  Canvas canvas(draft);
  canvas.setWindow(displayExtent.m_llx, displayExtent.m_lly, displayExtent.m_urx, displayExtent.m_ury);
  canvas.setRenderHint(QPainter::Antialiasing, true);

  switch(g->getGeomTypeId())
  {
    case te::gm::PolygonType:
    case te::gm::PolygonZType:
    case te::gm::PolygonMType:
    case te::gm::PolygonZMType:
    case te::gm::MultiPolygonType:
    case te::gm::MultiPolygonZType:
    case te::gm::MultiPolygonMType:
    case te::gm::MultiPolygonZMType:
    {
      canvas.setPolygonContourWidth(2);
      canvas.setPolygonContourColor(te::color::RGBAColor(255, 0, 0, 128));
      canvas.setPolygonFillColor(te::color::RGBAColor(255, 255, 255, 128));
    }
    break;

    case te::gm::LineStringType:
    case te::gm::LineStringZType:
    case te::gm::LineStringMType:
    case te::gm::LineStringZMType:
    case te::gm::MultiLineStringType:
    case te::gm::MultiLineStringZType:
    case te::gm::MultiLineStringMType:
    case te::gm::MultiLineStringZMType:
    {
      canvas.setLineColor(te::color::RGBAColor(255, 0, 0, 128));
      canvas.setLineWidth(6);
    }
    break;

    case te::gm::PointType:
    case te::gm::PointZType:
    case te::gm::PointMType:
    case te::gm::PointZMType:
    case te::gm::MultiPointType:
    case te::gm::MultiPointZType:
    case te::gm::MultiPointMType:
    case te::gm::MultiPointZMType:
    {
      std::size_t size = 24;

      te::se::Stroke* stroke = te::se::CreateStroke("#FF0000", "2","0.5");
      te::se::Fill* fill = te::se::CreateFill("#FFFFFF", "0.5");
      te::se::Mark* mark = te::se::CreateMark("square", stroke, fill);

      te::color::RGBAColor** rgba= te::map::MarkRendererManager::getInstance().render(mark, size);

      canvas.setPointColor(te::color::RGBAColor(0, 0, 0, TE_TRANSPARENT));
      canvas.setPointPattern(rgba, size, size);

      te::common::Free(rgba, size);
      delete mark;
    }
    break;

    default:
      return;
  }

  canvas.draw(g);
}
