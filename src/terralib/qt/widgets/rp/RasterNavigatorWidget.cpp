/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/rp/RasterNavigatorWidget.cpp

  \brief This file has the RasterNavigatorWidget class.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Point.h"
#include "../../../raster/Grid.h"
#include "../../../raster/Raster.h"
#include "../../widgets/tools/AbstractTool.h"
#include "../../widgets/tools/CoordTracking.h"
#include "../../widgets/tools/Pan.h"
#include "../../widgets/tools/PolygonAcquire.h"
#include "../../widgets/tools/PointPicker.h"
#include "../../widgets/tools/ReadPixelTool.h"
#include "../../widgets/tools/ZoomArea.h"
#include "../../widgets/tools/ZoomClick.h"
#include "../../widgets/tools/ZoomWheel.h"
#include "../canvas/Canvas.h"
#include "../canvas/MapDisplay.h"
#include "RasterNavigatorWidget.h"
#include "ui_RasterNavigatorWidgetForm.h"


te::qt::widgets::RasterNavigatorWidget::RasterNavigatorWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::RasterNavigatorWidgetForm),
    m_tool(0), m_panTool(0), m_zoomTool(0)
{
  m_ui->setupUi(this);

//build form
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_frame);
  m_mapDisplay = new te::qt::widgets::MapDisplay(m_ui->m_frame->size(), m_ui->m_frame);
  displayLayout->addWidget(m_mapDisplay);
  displayLayout->setContentsMargins(0,0,0,0);

// CoordTracking tool
  te::qt::widgets::CoordTracking* coordTracking = new te::qt::widgets::CoordTracking(m_mapDisplay, m_mapDisplay);
  m_mapDisplay->installEventFilter(coordTracking);

// signals & slots
  connect(m_ui->m_zoomInActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onZoomAreaToggled(bool)));
  connect(m_ui->m_zoomOutActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onZoomOutToggled(bool)));
  connect(m_ui->m_panActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onPanToggled(bool)));
  connect(m_ui->m_pointActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onPointPickerToggled(bool)));
  connect(m_ui->m_geomActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onGeomToggled(bool)));
  connect(m_ui->m_readPixelActionToolButton, SIGNAL(toggled(bool)), this, SLOT(onReadPixelToggled(bool)));
  connect(m_ui->m_recomposeActionToolButton, SIGNAL(clicked()), this, SLOT(onRecomposeClicked()));

  connect(coordTracking, SIGNAL(coordTracked(QPointF&)), this, SLOT(onCoordTrackedChanged(QPointF&)));
  connect(m_mapDisplay, SIGNAL(extentChanged()), this, SLOT(onMapDisplayExtentChanged()));

//pixmap
  m_ui->m_zoomInActionToolButtontoolButton->setIcon(QIcon::fromTheme("zoom-area"));
  m_ui->m_zoomOutActionToolButtontoolButton->setIcon(QIcon::fromTheme("zoom-out"));
  m_ui->m_panActionToolButtontoolButton->setIcon(QIcon::fromTheme("pan"));
  m_ui->m_recomposeActionToolButton->setIcon(QIcon::fromTheme("zoom-extent"));
  m_ui->m_pointActionToolButtontoolButton->setIcon(QIcon::fromTheme("placemark"));
  m_ui->m_geomActionToolButtontoolButton->setIcon(QIcon::fromTheme("edit-polygon"));
  m_ui->m_readPixelActionToolButton->setIcon(QIcon::fromTheme("color-picker"));
}

te::qt::widgets::RasterNavigatorWidget::~RasterNavigatorWidget()
{
  delete m_tool;
  delete m_panTool;
  delete m_zoomTool;
}

void te::qt::widgets::RasterNavigatorWidget::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  std::list<te::map::AbstractLayerPtr> list;

  list.push_back(m_layer);

  te::gm::Envelope e = m_layer->getExtent();

  m_mapDisplay->setMouseTracking(true);
  m_mapDisplay->setLayerList(list);
  m_mapDisplay->setSRID(m_layer->getSRID(), false);
  m_mapDisplay->setExtent(e, true);
}

te::gm::Envelope te::qt::widgets::RasterNavigatorWidget::getCurrentExtent()
{
  return m_mapDisplay->getExtent();
}

void te::qt::widgets::RasterNavigatorWidget::showAsPreview(bool asPreview)
{
  delete m_panTool;
  delete m_zoomTool;

  m_ui->m_toolsFrame->setVisible(!asPreview);
  m_ui->m_label->setVisible(!asPreview);

  if(asPreview)
  {
    m_panTool = new te::qt::widgets::Pan(m_mapDisplay, Qt::OpenHandCursor, Qt::ClosedHandCursor);
    m_zoomTool = new te::qt::widgets::ZoomWheel(m_mapDisplay, 1.5);

    m_mapDisplay->installEventFilter(m_panTool);
    m_mapDisplay->installEventFilter(m_zoomTool);
  }
}

void te::qt::widgets::RasterNavigatorWidget::hideEditionTools(bool hide)
{
  hidePickerTool(hide);
  hideGeomTool(hide);
  hideInfoTool(hide);
}

void te::qt::widgets::RasterNavigatorWidget::hidePickerTool(bool hide)
{
  m_ui->m_pointActionToolButtontoolButton->setVisible(!hide);
}

void te::qt::widgets::RasterNavigatorWidget::hideGeomTool(bool hide)
{
  m_ui->m_geomActionToolButtontoolButton->setVisible(!hide);
}

void te::qt::widgets::RasterNavigatorWidget::hideInfoTool(bool hide)
{
  m_ui->m_readPixelActionToolButton->setVisible(!hide);
}

void te::qt::widgets::RasterNavigatorWidget::onCoordTrackedChanged(QPointF& coordinate)
{
  assert(m_layer.get());

  //get input raster
  te::da::DataSet* ds = m_layer->getData();

  if(ds)
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds, te::dt::RASTER_TYPE);
    te::rst::Raster* inputRst = ds->getRaster(rpos);

    if(inputRst)
    {
      // find the selected pixel location for component
      te::gm::Coord2D pixelLocation = inputRst->getGrid()->geoToGrid((double) coordinate.rx(), (double)coordinate.ry());
      m_currentColumn = pixelLocation.x;
      m_currentRow = pixelLocation.y;

      QString xStr("X: ");
      xStr.append(QString::number(coordinate.rx()));
      QString yStr("Y: ");
      yStr.append(QString::number(coordinate.ry()));
      QString cStr(tr("Column: "));
      cStr.append(QString::number(m_currentColumn));
      QString lStr(tr("Line: "));
      lStr.append(QString::number(m_currentRow));
      QString label;
      label.append(xStr);
      label.append(" - ");
      label.append(yStr);
      label.append(" / ");
      label.append(cStr);
      label.append(" - ");
      label.append(lStr);

      m_ui->m_label->setText(label);
    }
  }

  delete ds;
}

void te::qt::widgets::RasterNavigatorWidget::onGeomAquired(te::gm::Polygon* poly)
{
  //emit signal
  geomAquired(poly, m_mapDisplay);
}

void te::qt::widgets::RasterNavigatorWidget::onPointPicked(QPointF& point)
{
  //emit signal
  emit pointPicked(point.x(), point.y(), m_mapDisplay);
}

void te::qt::widgets::RasterNavigatorWidget::onMapDisplayExtentChanged()
{
  //emit signal
  emit mapDisplayExtentChanged();
}

void te::qt::widgets::RasterNavigatorWidget::onZoomAreaToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::ZoomArea* zoomArea = new te::qt::widgets::ZoomArea(m_mapDisplay, Qt::BlankCursor);
  setCurrentTool(zoomArea);
}

void te::qt::widgets::RasterNavigatorWidget::onZoomOutToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::ZoomClick* zoomOut = new te::qt::widgets::ZoomClick(m_mapDisplay, 2.0, te::qt::widgets::Zoom::Out);
  setCurrentTool(zoomOut);
}

void te::qt::widgets::RasterNavigatorWidget::onPanToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::Pan* pan = new te::qt::widgets::Pan(m_mapDisplay, Qt::OpenHandCursor, Qt::ClosedHandCursor);
  setCurrentTool(pan);
}

void te::qt::widgets::RasterNavigatorWidget::onPointPickerToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::PointPicker* pp = new te::qt::widgets::PointPicker(m_mapDisplay, Qt::CrossCursor);
  setCurrentTool(pp);

  connect(pp, SIGNAL(pointPicked(QPointF&)), this, SLOT(onPointPicked(QPointF&)));
}

void te::qt::widgets::RasterNavigatorWidget::onGeomToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::PolygonAcquire* pa = new te::qt::widgets::PolygonAcquire(m_mapDisplay);
  setCurrentTool(pa);

  connect(pa, SIGNAL(polygonAquired(te::gm::Polygon*)), this, SLOT(onGeomAquired(te::gm::Polygon*)));
}

void te::qt::widgets::RasterNavigatorWidget::onReadPixelToggled(bool checked)
{
  if(!checked)
    return;

  te::qt::widgets::ReadPixelTool* pa = new te::qt::widgets::ReadPixelTool(m_mapDisplay, m_layer);
  setCurrentTool(pa);
}

void te::qt::widgets::RasterNavigatorWidget::onRecomposeClicked()
{
  te::gm::Envelope env = m_layer->getExtent();

  m_mapDisplay->setExtent(env);
}

void te::qt::widgets::RasterNavigatorWidget::setCurrentTool(te::qt::widgets::AbstractTool* tool)
{
  delete m_tool;
  m_tool = tool;

  m_mapDisplay->installEventFilter(m_tool);
}
