
//TerraLib
#include "../../../geometry/GeometryProperty.h"
#include "../../../geometry/MultiPolygon.h"
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
#include "SubtractAreaTool.h"

// Qt
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QDebug>

// STL
#include <cassert>
#include <memory>

te::edit::SubtractAreaTool::SubtractAreaTool(te::edit::EditionManager* editionManager, te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent)
: CreateLineTool(editionManager, display, layer, Qt::ArrowCursor, 0),
m_layer(layer),
m_feature(0),
m_editionManager(editionManager)
{
}

te::edit::SubtractAreaTool::~SubtractAreaTool()
{
  delete m_feature;
}

bool te::edit::SubtractAreaTool::mousePressEvent(QMouseEvent* e)
{
  if (e->button() != Qt::LeftButton)
    return false;

  if (m_isFinished) // Is Finished?! So, start again...
  {
    te::edit::CreateLineTool::clear();
    m_isFinished = false;
  }

  if (m_feature == 0)
    pickFeature(m_layer, GetPosition(e));

  return te::edit::CreateLineTool::mousePressEvent(e);
}

bool te::edit::SubtractAreaTool::mouseMoveEvent(QMouseEvent* e)
{
  return te::edit::CreateLineTool::mouseMoveEvent(e);
}

bool te::edit::SubtractAreaTool::mouseReleaseEvent(QMouseEvent* e)
{
  return true;
}

bool te::edit::SubtractAreaTool::mouseDoubleClickEvent(QMouseEvent* e)
{
  try
  {
    if (e->button() != Qt::LeftButton)
      return false;

    if (m_coords.size() < 3) // Can not stop yet...
      return false;

    if (m_feature == 0) // Can not stop yet...
    {
      QMessageBox::critical(m_display, tr("Error"), QString(tr("Error subtracting area to the polygon")));
      return false;
    }

    m_isFinished = true;

    draw();

    storeEditedFeature();

    m_updateWatches.push_back(m_feature->clone());

    QUndoCommand* command = new UpdateCommand(m_editionManager, m_updateWatches, m_display, m_layer);
    m_editionManager->addUndoStack(command);

    return true;
  }
  catch (std::exception& e)
  {
      QMessageBox::critical(m_display, tr("Error"), QString(tr("Could not subtract.") + " %1.").arg(e.what()));
      return false;
  }
}

void te::edit::SubtractAreaTool::draw()
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
  renderer.drawRepository(m_editionManager,m_layer->getId(), env, m_display->getSRID());

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

void te::edit::SubtractAreaTool::drawPolygon()
{
  // Build the geometry
  m_feature->setGeometry(buildPolygon());

  // Draw the current geometry and the vertexes
  Renderer& renderer = Renderer::getInstance();
  renderer.draw(m_feature->getGeometry(), true);
}

te::gm::Geometry* te::edit::SubtractAreaTool::buildPolygon()
{
  // Build the geometry
  te::gm::LinearRing* ring = new te::gm::LinearRing(m_coords.size() + 1, te::gm::LineStringType);
  for (std::size_t i = 0; i < m_coords.size(); ++i)
    ring->setPoint(i, m_coords[i].x, m_coords[i].y);
  ring->setPoint(m_coords.size(), m_coords[0].x, m_coords[0].y); // Closing...

  te::gm::Polygon* pHole = new te::gm::Polygon(1, te::gm::PolygonType);
  pHole->setRingN(0, ring);

  pHole->setSRID(m_display->getSRID());

  te::gm::Geometry* mpol = 0;

  if (!pHole->intersects(m_feature->getGeometry()))
    return m_feature->getGeometry();

  mpol = Difference(m_feature->getGeometry(), pHole);

  //projection
  if (mpol->getSRID() == m_layer->getSRID())
    return mpol;

  // else, need conversion...
  mpol->transform(m_layer->getSRID());

  return mpol;
}

void te::edit::SubtractAreaTool::pickFeature(const te::map::AbstractLayerPtr& layer, const QPointF& pos)
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

      std::auto_ptr<te::gm::Geometry> geom = ds->getGeometry(geomProp->getName());
      te::gm::Envelope Env(*geom->getMBR());

      m_feature = PickFeature(m_editionManager, m_layer, Env, m_display->getSRID());

    }

  }
  catch (std::exception& e)
  {
    QMessageBox::critical(m_display, tr("Error"), QString(tr("The geometry cannot be selected from the layer. Details:") + " %1.").arg(e.what()));
  }
}

te::gm::Envelope te::edit::SubtractAreaTool::buildEnvelope(const QPointF& pos)
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

void te::edit::SubtractAreaTool::reset()
{
  delete m_feature;
  m_feature = 0;
}

void te::edit::SubtractAreaTool::onExtentChanged()
{
  draw();
}


void te::edit::SubtractAreaTool::storeEditedFeature()
{
  m_editionManager->m_repository->addGeometry(m_layer->getId(), m_feature->getId()->clone(), dynamic_cast<te::gm::Geometry*>(buildPolygon()->clone()));

  m_editionManager->m_operation[m_feature->getId()->getValueAsString()] = m_editionManager->updateOp;
}

te::gm::Geometry* te::edit::SubtractAreaTool::Difference(te::gm::Geometry* g1, te::gm::Geometry* g2)
{
  return g1->difference(g2);
}