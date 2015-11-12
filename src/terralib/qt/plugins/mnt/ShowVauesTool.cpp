/*!
\file terralib/mnt/core/ShowValuesTool.cpp

\brief This file contains a class to Show MNT Values Tool.

*/

#include "Config.h"

//terralib
#include "../../widgets/Utils.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../maptools/MapDisplay.h"
#include "../../../maptools/RasterTransform.h"
#include "../../../raster.h"

#include "ShowValuesTool.h"

// Qt
#include <QMouseEvent>
#include <QtWidgets/QToolTip>

te::qt::plugins::mnt::ShowValuesTool::ShowValuesTool(te::qt::widgets::MapDisplay* display, te::qt::af::BaseApplication* app, QObject* parent)
  : te::qt::widgets::AbstractTool(display, parent)
{
  m_app = app;
}

te::qt::plugins::mnt::ShowValuesTool::~ShowValuesTool()
{
}

bool te::qt::plugins::mnt::ShowValuesTool::mouseMoveEvent(QMouseEvent* e)
{
  std::list<te::map::AbstractLayerPtr> layers = te::qt::widgets::GetSelectedLayersOnly(m_app->getLayerExplorer());
  if (!layers.size())
  {
    return false;
  }

  m_layer = dynamic_cast<te::map::AbstractLayer*>((*layers.begin()).get());

  std::auto_ptr<te::da::DataSetType> dsType = m_layer->getSchema();
  if (!dsType->hasRaster())
    return false;

  // Converts clicked point to world coordinates
#if QT_VERSION >= 0x050000
  QPointF qpoint = m_display->transform(e->localPos());
#else
  QPointF qpoint = m_display->transform(e->posF());
#endif

  std::auto_ptr<te::da::DataSet> inds = m_layer->getData();
  std::size_t rpos = te::da::GetFirstPropertyPos(inds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> raster(inds->getRaster(rpos).release());

  //create the raster transform
  te::map::RasterTransform rt(raster.get(), 0);

  te::gm::Coord2D coord = raster->getGrid()->geoToGrid(qpoint.x(), qpoint.y());

  int x = te::rst::Round(coord.x);
  int y = te::rst::Round(coord.y);

  if (x >= 0 && y >= 0 && x < (int)raster->getNumberOfColumns() && y < (int)raster->getNumberOfRows())
  {

    double val;
    raster.get()->getValue(x, y, val, 0);

    QString information = QString("%1").arg(val);

    // Show attributes
    QToolTip::showText(QCursor::pos(), information, m_display, m_display->rect());
  }

  raster.release();
  inds.release();
  dsType.release();

  return true;
}