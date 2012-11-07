#include "MapDisplay.h"

//! TerraLib include files
#include <terralib/geometry/Geometry.h>
#include <terralib/geometry/Envelope.h>
#include <terralib/geometry/Utils.h>

#include <terralib/maptools/Layer.h>

#include <terralib/qt/widgets/canvas/MapDisplay.h>
#include <terralib/qt/widgets/canvas/Canvas.h>

#include <terralib/qt/widgets/tools/AbstractTool.h>
#include <terralib/qt/widgets/tools/ZoomWheel.h>
#include <terralib/qt/widgets/tools/CoordTracking.h>

#include <terralib/qt/af/CoreApplication.h>
#include <terralib/qt/af/events/DisplayResized.h>
#include <terralib/qt/af/events/CurrentToolChanged.h>
#include <terralib/qt/af/events/TrackedCoordinate.h>
#include <terralib/qt/af/events/PointGeometries.h>
#include <terralib/qt/af/events/LayerSelected.h>
#include <terralib/qt/af/events/StyleChanged.h>

//! Qt include files
#include <QResizeEvent>

std::vector<te::gm::Geometry*> getGeometryVector(const std::map<std::string, te::gm::Geometry*>* geoms)
{
  std::vector<te::gm::Geometry*> g;
  std::map<std::string, te::gm::Geometry*>::const_iterator it;

  for(it = geoms->begin(); it != geoms->end(); ++it)
    g.push_back(it->second);

  return g;
}

namespace te
{
  namespace qt
  {
    namespace af
    {
      MapDisplay::MapDisplay(te::qt::widgets::MapDisplay* display) :
      QObject(display),
      m_display(display),
      m_current_tool(0),
      m_hl_visible(false)
      {
        te::qt::widgets::CoordTracking* coordTracking = new te::qt::widgets::CoordTracking(m_display, this);

        m_display->installEventFilter(this);
        m_display->installEventFilter(new te::qt::widgets::ZoomWheel(m_display, 2., this));
        m_display->installEventFilter(coordTracking);
        connect(coordTracking, SIGNAL(coordTracked(QPointF&)), SLOT(onCoordTracked(QPointF&)));
        connect (m_display, SIGNAL(extentChanged()), SLOT(redrawHighlight()));

        std::vector<te::gm::Geometry*> geoms_p, geoms_q, geoms_pq;
        m_geoms.push_back(geoms_p);
        m_geoms.push_back(geoms_q);
        m_geoms.push_back(geoms_pq);

        m_colors.push_back(QColor(0,255,0));
        m_colors.push_back(QColor(255,0,0));
        m_colors.push_back(QColor(0,0,255));
      }

      MapDisplay::~MapDisplay()
      {
        delete m_current_tool;
      }

      bool MapDisplay::eventFilter(QObject* watched, QEvent* evt)
      {
        if(watched == m_display)
        {
          switch(evt->type())
          {
            case QEvent::Resize:
              {
                QResizeEvent* e = static_cast<QResizeEvent*>(evt);
                te::qt::af::teApp::getInstance().broadCast(&te::qt::af::DisplayResized(m_display, e->size(), e->oldSize()));
              }
            break;

            default:
            break;
          }
        }

        return QObject::eventFilter(watched, evt);
      }

      void MapDisplay::drawLayers(const std::map<int, te::map::AbstractLayer*>& layers)
      {
        std::list<te::map::AbstractLayer*> lay_list;
        std::map<int, te::map::AbstractLayer*>::const_reverse_iterator it;

        for(it=layers.rbegin(); it!=layers.rend(); ++it)
          lay_list.push_back(it->second);

        m_display->setLayerList(lay_list);

        m_display->setExtent(*(*lay_list.begin())->getExtent());
      }

      void MapDisplay::setCurrentTool(te::qt::widgets::AbstractTool* tool)
      {
        teApp::getInstance().broadCast(&CurrentToolChanged(tool, m_current_tool));

        delete m_current_tool;
        m_current_tool = tool;

        m_display->installEventFilter(m_current_tool);
      }

      te::qt::widgets::MapDisplay* MapDisplay::getDisplayComponent()
      {
        return m_display;
      }

      void MapDisplay::onApplicationTriggered(te::qt::af::Event* evt)
      {
        switch(evt->getId())
        {
          case evt::POINT_GEOMETRIES:
            {
              PointGeometries* e = static_cast<PointGeometries*>(evt);

              te::common::FreeContents(m_geoms[POINTED]);

              m_geoms[POINTED] = getGeometryVector(e->m_geoms);

              m_hl_visible = (e->m_layer->getVisibility() == te::map::VISIBLE);

              redrawHighlight();
            }
          break;

          case evt::LAYER_SELECTED:
            {
              std::vector< std::vector<te::gm::Geometry*> >::iterator it;

              for(it = m_geoms.begin(); it != m_geoms.end(); ++it)
              {
                te::common::FreeContents(*it);
                (*it).clear();
              }
            }
          break;

          case evt::STYLE_CHANGED:
            {
              StyleChanged* e = static_cast<StyleChanged*> (evt);
              m_colors[e->m_group] = *(e->m_color);
              redrawHighlight();
            }
          break;

          default:
          break;
        }
      }

      void MapDisplay::onCoordTracked(QPointF& pos)
      {
        teApp::getInstance().broadCast(&TrackedCoordinate(pos));
      }

      void MapDisplay::redrawHighlight()
      {
        if(!m_hl_visible)
          return;

        // Clear!
        QPixmap* draft = m_display->getDraftPixmap();
        draft->fill(Qt::transparent);

        std::vector< std::vector<te::gm::Geometry*> >::iterator vgIt;
        std::vector<te::gm::Geometry*>::iterator gIt;
        std::vector<QColor>::iterator cIt;

            // Prepares the canvas
        const te::gm::Envelope* env = m_display->getExtent();
        te::qt::widgets::Canvas canvas(m_display->width(), m_display->height());
        canvas.setDevice(draft, false);
        canvas.setWindow(env->m_llx, env->m_lly, env->m_urx, env->m_ury);

        for(vgIt=m_geoms.begin(); vgIt != m_geoms.end(); ++vgIt)
        {
          std::vector<te::gm::Geometry*>* g = &(*vgIt);

          if(g->empty())
            continue;

          size_t pos = vgIt - m_geoms.begin();
          QColor* c = &(*(m_colors.begin()+pos));

          switch((*g->begin())->getGeomTypeId())
          {
            case te::gm::PolygonType:
            case te::gm::MultiPolygonType:
              canvas.setPolygonFillColor(te::color::RGBAColor(c->red(), c->green(), c->blue(), c->alpha()));
              canvas.setPolygonContourColor(te::color::RGBAColor(0, 0, 0, TE_OPAQUE));
              canvas.setPolygonContourWidth(1);
            break;

            case te::gm::LineStringType:
            case te::gm::MultiLineStringType:
            {
              canvas.setLineColor(te::color::RGBAColor(c->red(), c->green(), c->blue(), c->alpha()));
              canvas.setLineWidth(2);
            }
            break;

            case te::gm::PointType:
            case te::gm::MultiPointType:
            {
              canvas.setPointColor(te::color::RGBAColor(c->red(), c->green(), c->blue(), c->alpha()));
            }
            break;

            default:
            break;
          }

          for(gIt = g->begin(); gIt != g->end(); ++gIt)
          {
            te::gm::Geometry* geo = te::gm::GetGeomFromEnvelope(env, m_display->getSRID());
            if(geo->intersects(*gIt))
              canvas.draw(*gIt);
          }
        }

        m_display->repaint();
      }
    }
  }
}