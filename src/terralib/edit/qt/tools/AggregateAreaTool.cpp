
//TerraLib
#include "../../../common/STLUtils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../geometry/MultiPolygon.h"
#include "../../../geometry/Utils.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../qt/af/events/LayerEvents.h"
#include "../../../qt/af/events/MapEvents.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../Feature.h"
#include "../../RepositoryManager.h"
#include "../../Utils.h"
#include "../Renderer.h"
#include "../Utils.h"
#include "../core/command/UpdateCommand.h"
#include "../core/UndoStackManager.h"
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

  // Signals & slots
  connect(m_display, SIGNAL(extentChanged()), SLOT(onExtentChanged()));

}

te::edit::AggregateAreaTool::~AggregateAreaTool()
{
  delete m_feature;
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

  pickFeature(m_layer);

  return te::edit::CreateLineTool::mousePressEvent(e);
}

bool te::edit::AggregateAreaTool::mouseMoveEvent(QMouseEvent* e)
{
  return te::edit::CreateLineTool::mouseMoveEvent(e);
}

bool te::edit::AggregateAreaTool::mouseReleaseEvent(QMouseEvent* e)
{
  return false;
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
      QMessageBox::critical(m_display, tr("Error"), QString(tr("Error aggregating area to the polygon")));
      return false;
    }

    m_isFinished = true;

    draw();

    storeEditedFeature();

    storeUndoCommand();

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
    // Build the geometry
    te::gm::LinearRing* ring = new te::gm::LinearRing(m_coords.size() + 1, te::gm::LineStringType);
    for (std::size_t i = 0; i < m_coords.size(); ++i)
      ring->setPoint(i, m_coords[i].x, m_coords[i].y);
    ring->setPoint(m_coords.size(), m_coords[0].x, m_coords[0].y); // Closing...

    te::gm::Polygon* polygon = new te::gm::Polygon(1, te::gm::PolygonType);
    polygon->setRingN(0, ring);

    polygon->setSRID(m_display->getSRID());

    te::gm::Geometry* geo = 0;

    if (!polygon->intersects(m_feature->getGeometry()))
      return dynamic_cast<te::gm::Geometry*>(m_feature->getGeometry()->clone());

    geo = Union(polygon, m_feature);

    //projection
    if(polygon->getSRID() == m_layer->getSRID())
      return geo;

    //else, need conversion...
    geo->transform(m_layer->getSRID());

    return geo;

}

void te::edit::AggregateAreaTool::pickFeature(const te::map::AbstractLayerPtr& layer)
{
  reset();

  try
  {
    std::auto_ptr<te::da::DataSetType> dt(layer->getSchema());

    const te::da::ObjectIdSet* objSet = layer->getSelected();

    std::auto_ptr<te::da::DataSet> ds(layer->getData(objSet));

    te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dt.get());

    if (ds->moveNext())
    {

      te::gm::Coord2D coord(0, 0);

      std::auto_ptr<te::gm::Geometry> geom = ds->getGeometry(geomProp->getName());
      te::gm::Envelope auxEnv(*geom->getMBR());

      // Try finds the geometry centroid
      switch (geom->getGeomTypeId())
      {
        case te::gm::PolygonType:
        {
          te::gm::Polygon* p = dynamic_cast<te::gm::Polygon*>(geom.get());
          coord = *p->getCentroidCoord();

          break;
        }
        case te::gm::MultiPolygonType:
        {
          te::gm::MultiPolygon* mp = dynamic_cast<te::gm::MultiPolygon*>(geom.get());
          coord = *mp->getCentroidCoord();
        
          break;
        }
      }

      // Build the search envelope
      te::gm::Envelope e(coord.getX(), coord.getY(), coord.getX(), coord.getY());

      m_feature = PickFeature(m_layer, e, m_display->getSRID(), te::edit::GEOMETRY_UPDATE);

      m_updateWatches.push_back(m_feature->clone());

    }

  }
  catch (std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The geometry cannot be selected from the layer. Details:") + " %1.").arg(e.what()));
  }
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
  m_feature = 0;
}

void te::edit::AggregateAreaTool::onExtentChanged()
{
  draw();
}

void te::edit::AggregateAreaTool::storeEditedFeature()
{
  RepositoryManager::getInstance().addGeometry(m_layer->getId(), m_feature->getId()->clone(), dynamic_cast<te::gm::Geometry*>(buildPolygon()->clone()), te::edit::GEOMETRY_UPDATE);
}

te::gm::Geometry* te::edit::AggregateAreaTool::Union(te::gm::Geometry* g1, Feature* feature_g2)
{
  std::vector<std::string> oidPropertyNames;
  const te::gm::Envelope* env = g1->getMBR();

  g1 = g1->Union(feature_g2->getGeometry());

  std::auto_ptr<const te::map::LayerSchema> schema(m_layer->getSchema());
  if (!schema->hasGeom())
    return 0;

  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(schema.get());

  // Gets the dataset
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData(gp->getName(), env, te::gm::INTERSECTS);
  assert(ds.get());

  // Generates a geometry from the given extent. It will be used to refine the results
  std::auto_ptr<te::gm::Geometry> geometryFromEnvelope(te::gm::GetGeomFromEnvelope(env, m_display->getSRID()));

  // For while, first geometry property. TODO: get which geometry property the symbolizer references
  std::size_t gpos = te::da::GetPropertyPos(ds.get(), gp->getName());

  while (ds->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> g(ds->getGeometry(gpos));

    if (g->getSRID() == TE_UNKNOWN_SRS)
      g->setSRID(m_layer->getSRID());

    if (!g->intersects(geometryFromEnvelope.get()))
      continue;

    te::da::GetOIDPropertyNames(m_layer->getSchema().get(), oidPropertyNames);

    int colType = ds->getPropertyDataType(te::da::GetPropertyPos(ds.get(), oidPropertyNames[0]));

    // Feature found! Building the list of property values...
    for (std::size_t i = 0; i < ds->getNumProperties(); ++i)
    {
      if (ds->getPropertyName(i) == oidPropertyNames[0])
      {
        std::string value;

        if (colType == te::dt::INT16_TYPE || colType == te::dt::INT32_TYPE || colType == te::dt::INT64_TYPE || colType == te::dt::DOUBLE_TYPE)
        {
          value = boost::lexical_cast<std::string>(ds->getInt32(oidPropertyNames[0]));
        }
        else
        {
          value = ds->getString(oidPropertyNames[0]);
        }

        if (value != feature_g2->getId()->getValueAsString())
          g1 = g1->difference(g.get());
      }
    }

  }

  return g1;
}

void te::edit::AggregateAreaTool::storeUndoCommand()
{
  m_updateWatches.push_back(m_feature->clone());

  QUndoCommand* command = new UpdateCommand(m_updateWatches, m_display, m_layer);
  UndoStackManager::getInstance().addUndoStack(command);

}