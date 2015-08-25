#include "GeometriesUpdateTool.h"

#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../geometry/Geometry.h"
#include "../../../maptools/AbstractLayer.h"

#include "../../Feature.h"
#include "../../RepositoryManager.h"
#include "../core/UndoStackManager.h"

te::edit::GeometriesUpdateTool::GeometriesUpdateTool(te::qt::widgets::MapDisplay* display, te::map::AbstractLayer* layer, QObject* parent) :
  te::qt::widgets::AbstractTool(display, parent),
  m_feature(0),
  m_layer(layer)
{}

te::edit::GeometriesUpdateTool::~GeometriesUpdateTool()
{
  emit toolDeleted();
}

void te::edit::GeometriesUpdateTool::setLayer(te::map::AbstractLayer* layer)
{
  m_layer = layer;
}

void te::edit::GeometriesUpdateTool::saveGeometry(te::gm::Geometry* gm)
{
  RepositoryManager::getInstance().addGeometry(m_layer->getId(), gm, te::edit::GEOMETRY_UPDATE);

  m_feature = RepositoryManager::getInstance().getFeature(m_layer->getId(), *gm->getMBR(), gm->getSRID());

  emit geometriesEdited();
}
