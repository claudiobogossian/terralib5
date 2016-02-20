/*!
\file terralib/mnt/core/ShowValuesTool.cpp

\brief This file contains a class to Show MNT Values Tool.

*/

#include "Config.h"

//terralib
#include "../../widgets/tools/CoordTracking.h"
#include "../../widgets/Utils.h"
#include "../../../qt/widgets/canvas/MapDisplay.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../maptools/MapDisplay.h"
#include "../../../maptools/RasterTransform.h"
#include "../../../raster.h"

#include "ShowValuesAction.h"
#include "ShowValuesTool.h"

// Qt
#include <QMessageBox>
#include <QMouseEvent>
#include <QtWidgets/QToolTip>

#include <iostream>

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
  QStatusBar* sb = m_app->statusBar();

  std::list<te::map::AbstractLayerPtr> layers = te::qt::widgets::GetSelectedLayersOnly(m_app->getLayerExplorer());
  if (!layers.size())
  {
    QMessageBox::information(0, tr("Show Values"), TE_TR("Select a layer!"));
    sb->clearMessage();
    m_action->triggered();
    return false;
  }

  m_layer = dynamic_cast<te::map::AbstractLayer*>((*layers.begin()).get());

  std::auto_ptr<te::da::DataSetType> dsType = m_layer->getSchema();
  if (!dsType->hasRaster())
  {
    QMessageBox::information(0, tr("Show Values"), TE_TR("Select a raster layer!"));
    sb->clearMessage();
    m_action->triggered();
    return false;
  }

  if (m_layer->getSRID() != m_display->getSRID())
  {
    QMessageBox::information(0, tr("Show Values"), TE_TR("SRID layer is different SRID display!"));
    sb->clearMessage();
    m_action->triggered();
    return false;
  }

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
    raster->getValue(x, y, val, 0);

    QString information;
    if (val != raster->getBand(0)->getProperty()->m_noDataValue)
     information = QString(tr("Coord: %1, %2 Value: %3")).arg(qpoint.x(), 0, 'f', 2).arg(qpoint.y(), 0, 'f', 2).arg(val);
    else
      information = QString(tr("Coord: %1, %2 NODATA")).arg(qpoint.x(), 0, 'f', 2).arg(qpoint.y(), 0, 'f', 2);

    sb->showMessage(information);
  }
  else
    sb->showMessage("");

  raster.release();
  inds.release();
  dsType.release();

  return true;
}