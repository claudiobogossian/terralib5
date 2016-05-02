
//TerraLib
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry.h"
#include "../../../qt/af/events/LayerEvents.h"
#include "../../../qt/af/events/MapEvents.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../Feature.h"
#include "../../RepositoryManager.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "AggregateAreaTool.h"

// Qt
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QDebug>

// STL
#include <cassert>
#include <memory>
#include <iostream>

te::edit::AggregateAreaTool::AggregateAreaTool(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent)
  : CreateLineTool(display, layer, Qt::ArrowCursor, parent),
  m_updateWatches(0)
{
}

te::edit::AggregateAreaTool::~AggregateAreaTool()
{
  reset();
}

bool te::edit::AggregateAreaTool::mousePressEvent(QMouseEvent* e)
{
  if (e->button() != Qt::LeftButton)
    return false;

  if (m_isFinished) // Is Finished?! So, start again...
  {
    te::edit::CreateLineTool::clear();
    m_isFinished = false;
  }

  pickFeature(m_layer, GetPosition(e));

  return te::edit::CreateLineTool::mousePressEvent(e);
}

bool te::edit::AggregateAreaTool::mouseMoveEvent(QMouseEvent* e)
{
  return te::edit::CreateLineTool::mouseMoveEvent(e);
}

bool te::edit::AggregateAreaTool::mouseDoubleClickEvent(QMouseEvent* e)
{
  try
  {
    if (e->button() != Qt::LeftButton)
      return false;

    if (m_coords.size() < 3) // Can not stop yet...
      return false;

    if (m_feature == 0) // Can not stop yet...
    {
      te::edit::CreateLineTool::clear();
      QMessageBox::critical(m_display, tr("Error"), QString(tr("Error aggregating area to the polygon")));
      return false;
    }

    m_isFinished = true;

    draw();

    storeFeature();

    return true;
  }
  catch (std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("Could not join.") + " %1.").arg(e.what()));
    return false;
  }
}

void te::edit::AggregateAreaTool::draw()
{
  
  const te::gm::Envelope& env = m_display->getExtent();
  if (!env.isValid())
    return;

  // Clear!
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  // Initialize the renderer
  Renderer& renderer = Renderer::getInstance();
  renderer.begin(draft, env, m_display->getSRID());

  // Draw the layer edited geometries
  renderer.drawRepository(m_layer->getId(), env, m_display->getSRID());

  if (!m_coords.empty())
  {
    if (m_coords.size() > 3)
      drawPolygon();

    if (m_continuousMode == false)
      m_coords.pop_back();
  }

  renderer.end();

  m_display->repaint();
  
}

void te::edit::AggregateAreaTool::drawPolygon()
{
  // Build the geometry
  m_feature->setGeometry(buildPolygon());

  // Draw the current geometry and the vertexes
  Renderer& renderer = Renderer::getInstance();
  renderer.draw(m_feature->getGeometry(), true);

}

te::gm::Geometry* te::edit::AggregateAreaTool::buildPolygon()
{
    te::gm::Geometry* geoUnion = 0;

    // Build the geometry
    te::gm::LinearRing* ring = new te::gm::LinearRing(m_coords.size() + 1, te::gm::LineStringType);
    for (std::size_t i = 0; i < m_coords.size(); ++i)
      ring->setPoint(i, m_coords[i].x, m_coords[i].y);
    ring->setPoint(m_coords.size(), m_coords[0].x, m_coords[0].y); // Closing...

    te::gm::Polygon* polygon = new te::gm::Polygon(1, te::gm::PolygonType);
    polygon->setRingN(0, ring);

    polygon->setSRID(m_display->getSRID());

    if (polygon->getSRID() != m_layer->getSRID())
      polygon->transform(m_layer->getSRID());

    if (polygon->getSRID() != m_feature->getGeometry()->getSRID())
      m_feature->getGeometry()->transform(polygon->getSRID());

    if (!polygon->intersects(m_feature->getGeometry()))
      return dynamic_cast<te::gm::Geometry*>(m_feature->getGeometry()->clone());

    geoUnion = convertGeomType(m_layer, unionGeometry(polygon, m_feature->getGeometry()));

    geoUnion->setSRID(m_display->getSRID());

    if (geoUnion->getSRID() == m_layer->getSRID())
      return geoUnion;

    // else, need conversion...
    geoUnion->transform(m_layer->getSRID());

    return geoUnion;

}

te::gm::Envelope te::edit::AggregateAreaTool::buildEnvelope(const QPointF& pos)
{
  QPointF pixelOffset(4.0, 4.0);

  QRectF rect(pos - pixelOffset, pos + pixelOffset);

  // Converts rect boundary to world coordinates
  QPointF ll(rect.left(), rect.bottom());
  QPointF ur(rect.right(), rect.top());
  ll = m_display->transform(ll);
  ur = m_display->transform(ur);

  te::gm::Envelope env(ll.x(), ll.y(), ur.x(), ur.y());

  return env;
}

void te::edit::AggregateAreaTool::reset()
{
  delete m_feature;
}

void te::edit::AggregateAreaTool::onExtentChanged()
{
  draw();
}

void te::edit::AggregateAreaTool::storeFeature()
{
  RepositoryManager::getInstance().addFeature(m_layer->getId(), m_feature->clone());
}

te::gm::Geometry* te::edit::AggregateAreaTool::unionGeometry(te::gm::Geometry* g1, te::gm::Geometry* g2)
{
  return g1->Union(g2);
}

void te::edit::AggregateAreaTool::pickFeature(const te::map::AbstractLayerPtr& layer, const QPointF& pos)
{
  te::gm::Envelope env = buildEnvelope(pos);

  try
  {
    if (m_feature == 0)
    {
      m_feature = PickFeature(layer, env, m_display->getSRID(), te::edit::GEOMETRY_UPDATE);

      if (m_feature){
        m_updateWatches.push_back(m_feature->clone());
        m_oidsSet.insert(m_feature->getId()->getValueAsString());
      }
    }
    else
    {
      Feature* feature = PickFeature(layer, env, m_display->getSRID(), te::edit::GEOMETRY_UPDATE);
      if (feature)
      {
        if (m_oidsSet.find(feature->getId()->clone()->getValueAsString()) == m_oidsSet.end())
        {
          m_updateWatches.push_back(feature->clone());
          m_oidsSet.insert(feature->getId()->clone()->getValueAsString());
          m_feature = feature;
        }
        else
        {
          if (m_feature->getId()->clone()->getValueAsString() != feature->getId()->clone()->getValueAsString())
            m_feature = feature;
        }
      }
    }

  }
  catch (std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The geometry cannot be selected from the layer. Details:") + " %1.").arg(e.what()));
  }
}