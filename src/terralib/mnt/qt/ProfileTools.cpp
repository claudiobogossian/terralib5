/*!
\file terralib/mnt/qt/ProfileTools.cpp

\brief This file defines the profile trajectory edition tools

*/

#include <algorithm>
// TerraLib
#include "../../edit/Feature.h"
#include "../../edit/qt/Renderer.h"
#include "../../edit/qt/Utils.h"
#include "../../edit/RepositoryManager.h"
#include "../../edit/Utils.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/MultiLineString.h"
#include "../../qt/af/ApplicationController.h"
#include "../../qt/af/BaseApplication.h"
#include "../../qt/widgets/canvas/Canvas.h"
#include "../../qt/widgets/canvas/MapDisplay.h"
#include "../../qt/widgets/Utils.h"

#include "ProfileTools.h"

#include <QMessageBox>


te::mnt::ProfileTools::ProfileTools(te::qt::widgets::MapDisplay* display, const te::map::AbstractLayerPtr& layer, QObject* parent)
: te::edit::VertexTool(display, layer),
m_type(NONE)
{
  te::edit::VertexTool::pickFeature(layer, layer->getExtent());
}

te::mnt::ProfileTools::~ProfileTools()
{
}

void te::mnt::ProfileTools::setType(EditType t)
{
  m_type = t;
  switch (m_type)
  {
    case VERTEX_MOVE:
    case VERTEX_DELETE:
      te::edit::VertexTool::setStage(VERTEX_SEARCH);
      break;
    case VERTEX_ADD:
      te::edit::VertexTool::setStage(VERTEX_READY_TO_ADD);
    default:
      break;
  }
}

void te::mnt::ProfileTools::setLines(std::vector<te::gm::LineString*> &l)
{ 
  m_lines = l;
  if (m_rtree.size())
  updateRTree();

  te::edit::RepositoryManager::getInstance().removeAll();
  for (size_t i = 0; i < m_lines.size(); i++)
   te::edit::RepositoryManager::getInstance().addGeometry(m_layer->getId(), m_lines[i], te::edit::GEOMETRY_CREATE);

}

bool te::mnt::ProfileTools::mousePressEvent(QMouseEvent* e)
{
  if (m_type == VERTEX_ADD)
    return true;

  if (m_type == VERTEX_DELETE)
    e->setModifiers(Qt::ShiftModifier);

  return (te::edit::VertexTool::mousePressEvent(e));
}

bool te::mnt::ProfileTools::mouseMoveEvent(QMouseEvent* e)
{
    m_feature = 0;
  if (m_type == LINE_INVERT || m_type == LINE_DELETE)
    return true;

  bool ret = (te::edit::VertexTool::mouseMoveEvent(e));
  return ret;
}

bool te::mnt::ProfileTools::mouseReleaseEvent(QMouseEvent* e)
{
  if (m_type == VERTEX_ADD)
    return true;

  if (te::edit::VertexTool::m_currentStage == FEATURE_SELECTION)
    te::edit::VertexTool::setStage(VERTEX_SEARCH);

  if (m_type == LINE_INVERT)
  {
    te::gm::Envelope env = buildEnvelope(te::edit::GetPosition(e));

    te::gm::LineString env_line(5, te::gm::LineStringType, m_layer->getSRID());
    env_line.setPoint(0, env.getLowerLeftX(), env.getLowerLeftY());
    env_line.setPoint(1, env.getLowerLeftX(), env.getUpperRightY());
    env_line.setPoint(2, env.getUpperRightX(), env.getUpperRightY());
    env_line.setPoint(3, env.getUpperRightX(), env.getLowerLeftY());
    env_line.setPoint(4, env.getLowerLeftX(), env.getLowerLeftY());

    te::color::RGBAColor green(0, 255, 0, 255);
    for (size_t i = 0; i < m_lines.size(); i++)
    {
      if (m_lines[i]->intersects(&env_line))
      {
        const te::gm::Envelope& displayExtent = m_display->getExtent();
        te::qt::widgets::Canvas canvas(m_display->getDisplayPixmap());
        canvas.setWindow(displayExtent.m_llx, displayExtent.m_lly, displayExtent.m_urx, displayExtent.m_ury);
        canvas.setLineColor(green);
        canvas.setLineWidth(3);
        canvas.setLineDashStyle(te::map::SolidLine);
        canvas.draw(m_lines[i]);
        te::qt::widgets::Config2DrawPoints(&canvas, "circle", 16, QColor(green.getRgba()), QColor(green.getRgba()), 1);
        canvas.draw(m_lines[i]->getStartPoint());
        te::qt::widgets::Config2DrawPoints(&canvas, "star", 16, QColor(green.getRgba()), QColor(green.getRgba()), 1);
        canvas.draw(m_lines[i]->getEndPoint());

        if (QMessageBox::question(0, tr("Profile"), tr("Invert this trajectory?")) == QMessageBox::Yes)
        {
          te::gm::Coord2D *coords = m_lines[i]->getCoordinates();
          std::vector<te::gm::Coord2D> pts;
          for (size_t j = 0; j <  m_lines[i]->getNPoints(); j++)
          {
            pts.push_back(coords[j]);
          }
          std::reverse(pts.begin(),pts.end());
          for (size_t j = 0; j < m_lines[i]->getNPoints(); j++)
          {
            coords[j].x = pts[j].getX();
            coords[j].y = pts[j].getY();
          }
        }
        break;
      }
    }
  }

  if (m_type == LINE_DELETE)
  {
    te::gm::Envelope env = buildEnvelope(te::edit::GetPosition(e));

    te::gm::LineString env_line(5, te::gm::LineStringType, m_layer->getSRID());
    env_line.setPoint(0, env.getLowerLeftX(), env.getLowerLeftY());
    env_line.setPoint(1, env.getLowerLeftX(), env.getUpperRightY());
    env_line.setPoint(2, env.getUpperRightX(), env.getUpperRightY());
    env_line.setPoint(3, env.getUpperRightX(), env.getLowerLeftY());
    env_line.setPoint(4, env.getLowerLeftX(), env.getLowerLeftY());

    te::color::RGBAColor red(255, 0, 0, 255);
    for (size_t i = 0; i < m_lines.size(); i++)
    {
      if (m_lines[i]->intersects(&env_line))
      {
        const te::gm::Envelope& displayExtent = m_display->getExtent();
        te::qt::widgets::Canvas canvas(m_display->getDisplayPixmap());
        canvas.setWindow(displayExtent.m_llx, displayExtent.m_lly, displayExtent.m_urx, displayExtent.m_ury);
        canvas.setLineColor(red);
        canvas.setLineWidth(3);
        canvas.setLineDashStyle(te::map::SolidLine);
        canvas.draw(m_lines[i]);

        if (QMessageBox::question(0, tr("Profile"), tr("Delete this trajectory?")) == QMessageBox::Yes)
        {
          m_lines.erase(m_lines.begin() + i);
        }
        break;
      }
    }
  }

  bool ret = te::edit::VertexTool::mouseReleaseEvent(e);

  emit geometriesEdited();

  te::edit::VertexTool::setStage(VERTEX_SEARCH);
  return ret;
}

bool te::mnt::ProfileTools::mouseDoubleClickEvent(QMouseEvent* e)
{
  if (m_type == VERTEX_ADD)
  {
    te::edit::VertexTool::setStage(VERTEX_READY_TO_ADD);
    bool ret = te::edit::VertexTool::mouseDoubleClickEvent(e);
    te::edit::VertexTool::setStage(VERTEX_READY_TO_ADD);
    return ret;
  }

  return (te::edit::VertexTool::mouseDoubleClickEvent(e));
}


