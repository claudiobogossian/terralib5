#include "GeometriesUpdateTool.h"

#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../geometry/Geometry.h"
#include "../../../maptools/AbstractLayer.h"

#include "../../Feature.h"
#include "../../RepositoryManager.h"
#include "../core/UndoStackManager.h"

te::edit::GeometriesUpdateTool::GeometriesUpdateTool(te::qt::widgets::MapDisplay* display, te::map::AbstractLayer* layer, QObject* parent) :
  te::qt::widgets::AbstractTool(display, parent),
  m_isInUse(true),
  m_feature(0),
  m_layer(layer)
{

}

te::edit::GeometriesUpdateTool::~GeometriesUpdateTool()
{
//  delete m_feature;

  emit toolDeleted();
}

void te::edit::GeometriesUpdateTool::setLayer(te::map::AbstractLayer* layer)
{
  m_layer = layer;
}

void te::edit::GeometriesUpdateTool::saveGeometry(te::gm::Geometry* gm, const int& op)
{
  RepositoryManager::getInstance().addGeometry(m_layer->getId(), gm, (OperationType) op);

  m_feature = RepositoryManager::getInstance().getFeature(m_layer->getId(), *gm->getMBR(), gm->getSRID());

  emit geometriesEdited();
}

bool te::edit::GeometriesUpdateTool::isInUse() const
{
  return m_isInUse;
}

void te::edit::GeometriesUpdateTool::setInUse(const bool& status)
{
  m_isInUse = status;
}
