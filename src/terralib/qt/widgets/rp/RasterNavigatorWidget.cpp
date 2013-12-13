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
#include "../../../color/RGBAColor.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/Point.h"
#include "../../../geometry/Utils.h"
#include "../../../maptools/Utils.h"
#include "../../../raster/Grid.h"
#include "../../../raster/Raster.h"
#include "../../../se/CoverageStyle.h"
#include "../../../se/ChannelSelection.h"
#include "../../../se/RasterSymbolizer.h"
#include "../../../se/Rule.h"
#include "../../../se/SelectedChannel.h"
#include "../../../se/Utils.h"
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
#include "../canvas/EyeBirdMapDisplayWidget.h"
#include "../canvas/MultiThreadMapDisplay.h"
#include "../canvas/ZoomInMapDisplayWidget.h"
#include "RasterNavigatorWidget.h"
#include "ui_RasterNavigatorWidgetForm.h"

//STL
#include <memory>


te::qt::widgets::RasterNavigatorWidget::RasterNavigatorWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::RasterNavigatorWidgetForm),
    m_symbolizer(0),
    m_tool(0), m_panTool(0), m_zoomTool(0)
{
  m_ui->setupUi(this);

//build form
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_frame);
  m_mapDisplay = new te::qt::widgets::MultiThreadMapDisplay(m_ui->m_frame->size(), m_ui->m_frame);
  m_mapDisplay->setResizePolicy(te::qt::widgets::MapDisplay::Center);
  displayLayout->addWidget(m_mapDisplay);
  displayLayout->setContentsMargins(0,0,0,0);

  QGridLayout* eyeBirdDisplayLayout = new QGridLayout(m_ui->m_eyeBirdFrame);
  m_eyeBirdMapDisplay = new te::qt::widgets::EyeBirdMapDisplayWidget(m_mapDisplay, m_ui->m_eyeBirdFrame);
  eyeBirdDisplayLayout->addWidget(m_eyeBirdMapDisplay);
  eyeBirdDisplayLayout->setContentsMargins(0,0,0,0);

  QGridLayout* zoomInDisplayLayout = new QGridLayout(m_ui->m_zoomInFrame);
  m_zoomInMapDisplay = new te::qt::widgets::ZoomInMapDisplayWidget(m_mapDisplay, m_ui->m_zoomInFrame);
  zoomInDisplayLayout->addWidget(m_zoomInMapDisplay);
  zoomInDisplayLayout->setContentsMargins(0,0,0,0);

// CoordTracking tool
  te::qt::widgets::CoordTracking* coordTracking = new te::qt::widgets::CoordTracking(m_mapDisplay, m_mapDisplay);
  m_mapDisplay->installEventFilter(coordTracking);

// signals & slots
  connect(m_ui->m_previewToolButton, SIGNAL(clicked()), this, SLOT(onPreviewClicked()));

  connect(m_ui->m_zoomInActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onZoomAreaToggled(bool)));
  connect(m_ui->m_zoomOutActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onZoomOutToggled(bool)));
  connect(m_ui->m_panActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onPanToggled(bool)));
  connect(m_ui->m_pointActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onPointPickerToggled(bool)));
  connect(m_ui->m_geomActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onGeomToggled(bool)));
  connect(m_ui->m_readPixelActionToolButton, SIGNAL(toggled(bool)), this, SLOT(onReadPixelToggled(bool)));
  connect(m_ui->m_extraDisplaysToolButton, SIGNAL(toggled(bool)), this, SLOT(onExtraDisplaysToggled(bool)));
  connect(m_ui->m_recomposeActionToolButton, SIGNAL(clicked()), this, SLOT(onRecomposeClicked()));

  connect(m_ui->m_redComboBox, SIGNAL(activated(int)), this, SLOT(onRedComboBoxActivated(int)));
  connect(m_ui->m_greenComboBox, SIGNAL(activated(int)), this, SLOT(onGreenComboBoxActivated(int)));
  connect(m_ui->m_blueComboBox, SIGNAL(activated(int)), this, SLOT(onBlueComboBoxActivated(int)));
  connect(m_ui->m_monoComboBox, SIGNAL(activated(int)), this, SLOT(onMonoComboBoxActivated(int)));
  connect(m_ui->m_monoToolButton, SIGNAL(clicked(bool)), this, SLOT(onMonoToolClicked(bool)));
  connect(m_ui->m_compositionToolButton, SIGNAL(clicked(bool)), this, SLOT(onCompositionToolClicked(bool)));
  connect(m_ui->m_redToolButton, SIGNAL(clicked(bool)), this, SLOT(onRedToolClicked(bool)));
  connect(m_ui->m_greenToolButton, SIGNAL(clicked(bool)), this, SLOT(onGreenToolClicked(bool)));
  connect(m_ui->m_blueToolButton, SIGNAL(clicked(bool)), this, SLOT(onBlueToolClicked(bool)));

  connect(coordTracking, SIGNAL(coordTracked(QPointF&)), this, SLOT(onCoordTrackedChanged(QPointF&)));
  connect(m_mapDisplay, SIGNAL(extentChanged()), this, SLOT(onMapDisplayExtentChanged()));

//pixmap
  m_ui->m_previewToolButton->setIcon(QIcon::fromTheme("check"));
  m_ui->m_zoomInActionToolButtontoolButton->setIcon(QIcon::fromTheme("zoom-area"));
  m_ui->m_zoomOutActionToolButtontoolButton->setIcon(QIcon::fromTheme("zoom-out"));
  m_ui->m_panActionToolButtontoolButton->setIcon(QIcon::fromTheme("pan"));
  m_ui->m_recomposeActionToolButton->setIcon(QIcon::fromTheme("zoom-extent"));
  m_ui->m_pointActionToolButtontoolButton->setIcon(QIcon::fromTheme("placemark"));
  m_ui->m_geomActionToolButtontoolButton->setIcon(QIcon::fromTheme("edit-polygon"));
  m_ui->m_readPixelActionToolButton->setIcon(QIcon::fromTheme("color-picker"));
  m_ui->m_extraDisplaysToolButton->setIcon(QIcon::fromTheme("view-map-display-extra"));
  m_ui->m_monoLabel->setPixmap(QIcon::fromTheme("bullet-black").pixmap(16,16));
  m_ui->m_redLabel->setPixmap(QIcon::fromTheme("bullet-red").pixmap(16,16));
  m_ui->m_greenLabel->setPixmap(QIcon::fromTheme("bullet-green").pixmap(16,16));
  m_ui->m_blueLabel->setPixmap(QIcon::fromTheme("bullet-blue").pixmap(16,16));
  m_ui->m_monoToolButton->setIcon(QIcon::fromTheme("channel-gray"));
  m_ui->m_compositionToolButton->setIcon(QIcon::fromTheme("channels"));
  m_ui->m_redToolButton->setIcon(QIcon::fromTheme("channel-red"));
  m_ui->m_greenToolButton->setIcon(QIcon::fromTheme("channel-green"));
  m_ui->m_blueToolButton->setIcon(QIcon::fromTheme("channel-blue"));

  m_ui->m_previewToolButton->setVisible(false);

  onExtraDisplaysToggled(false);
}

te::qt::widgets::RasterNavigatorWidget::~RasterNavigatorWidget()
{
  if(m_layer.get())
    m_layer->setVisibility(m_visibility);

  delete m_tool;
  delete m_panTool;
  delete m_zoomTool;
}

void te::qt::widgets::RasterNavigatorWidget::set(te::map::AbstractLayerPtr layer, bool setFullScaleBox)
{
  m_layer = layer;

  m_visibility = m_layer->getVisibility();

  m_layer->setVisibility(te::map::VISIBLE);

  std::list<te::map::AbstractLayerPtr> list;
  list.push_back(m_layer);

  te::gm::Envelope e = m_layer->getExtent();

  m_mapDisplay->setMouseTracking(true);
  m_mapDisplay->setLayerList(list);
  m_mapDisplay->setSRID(m_layer->getSRID(), false);
  
  if(!setFullScaleBox)
    m_mapDisplay->setExtent(e, false);
  else
  {
    //get raster
    std::auto_ptr<te::da::DataSet> ds(m_layer->getData());
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    //calculate extent full scale
    int w = m_mapDisplay->getWidth();
    int h = m_mapDisplay->getHeight();
    double resX = inputRst->getResolutionX();
    double resY = inputRst->getResolutionY();
    te::gm::Coord2D cc(inputRst->getExtent()->getCenter().x, inputRst->getExtent()->getCenter().y);
    te::gm::Coord2D bowerLeftWorld(cc.x - (resX * ( w / 2 )), cc.y - (resY * ( h / 2 )));
    te::gm::Coord2D bowerUpperWorld(cc.x + (resX * ( w / 2 )), cc.y + (resY * ( h / 2 )));

    te::gm::Envelope env(bowerLeftWorld.x, bowerLeftWorld.y, bowerUpperWorld.x, bowerUpperWorld.y);

    m_mapDisplay->setExtent(env, false);
  }

  m_zoomInMapDisplay->setList(list, m_layer->getSRID());
  m_eyeBirdMapDisplay->setList(list, m_layer->getSRID());

// list bands
  listBands();

// get band composition information
  getCompositionInfo();
}

te::gm::Envelope te::qt::widgets::RasterNavigatorWidget::getCurrentExtent()
{
  return m_mapDisplay->getExtent();
}

te::qt::widgets::MapDisplay* te::qt::widgets::RasterNavigatorWidget::getDisplay()
{
  return m_mapDisplay;
}

te::rst::Raster* te::qt::widgets::RasterNavigatorWidget::getExtentRaster(bool fullScale)
{
  //get box info
  te::gm::Envelope reprojectedBBOX(m_mapDisplay->getExtent());
  reprojectedBBOX.transform(m_mapDisplay->getSRID(), m_layer->getSRID());
  te::gm::Envelope ibbox = reprojectedBBOX.intersection(m_layer->getExtent());

  //get raster
  std::auto_ptr<te::da::DataSet> ds(m_layer->getData());
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  te::rst::Raster* raster = 0;

  if(fullScale)
  {
    te::gm::Coord2D startGrid = inputRst->getGrid()->geoToGrid(ibbox.getLowerLeftX(), ibbox.getLowerLeftY());
    te::gm::Coord2D endGrid = inputRst->getGrid()->geoToGrid(ibbox.getUpperRightX(), ibbox.getUpperRightY());

    int w = (int)endGrid.x - (int)startGrid.x;
    int h = (int)startGrid.y - (int)endGrid.y;

    raster = te::map::GetExtentRaster(inputRst.get(), w, h, reprojectedBBOX, m_mapDisplay->getSRID(), ibbox, m_layer->getSRID());
  }
  else
  {
    raster = te::map::GetExtentRaster(inputRst.get(), m_mapDisplay->getWidth(), m_mapDisplay->getHeight(), reprojectedBBOX, m_mapDisplay->getSRID(), ibbox, m_layer->getSRID());
  }

  return raster;
}

void te::qt::widgets::RasterNavigatorWidget::drawRaster(te::rst::Raster* rst, te::se::Style* style)
{
  const te::gm::Envelope& env = m_mapDisplay->getExtent();
  const te::gm::Envelope& envRst = *rst->getExtent();

  QPixmap* draft = m_mapDisplay->getDraftPixmap();
  draft->fill(Qt::transparent);

  // Prepares the canvas
  Canvas canvas(m_mapDisplay->width(), m_mapDisplay->height());
  canvas.setDevice(draft, false);
  canvas.setWindow(env.m_llx, env.m_lly, env.m_urx, env.m_ury);

  bool hasToDelete = false;
  //style
  if(!style)
  {
    style = te::se::CreateCoverageStyle(rst->getNumberOfBands());

    hasToDelete = true;
  }

  te::se::CoverageStyle* cs = dynamic_cast<te::se::CoverageStyle*>(style);

  // Draw raster
  te::map::DrawRaster(rst, &canvas, env, m_mapDisplay->getSRID(), envRst, rst->getSRID(), cs);

  if(hasToDelete)
    delete style;

  m_mapDisplay->repaint();
}

void te::qt::widgets::RasterNavigatorWidget::showAsPreview(bool asPreview)
{
  delete m_panTool;
  delete m_zoomTool;

  m_ui->m_toolsFrame->setVisible(!asPreview);
  m_ui->m_label->setVisible(!asPreview);
  m_ui->m_previewToolButton->setVisible(asPreview);

  hideExtraDisplaysTool(asPreview);

  if(asPreview)
  {
    m_panTool = new te::qt::widgets::Pan(m_mapDisplay, Qt::OpenHandCursor, Qt::ClosedHandCursor);
    m_zoomTool = new te::qt::widgets::ZoomWheel(m_mapDisplay, 1.5);

    m_mapDisplay->installEventFilter(m_panTool);
    m_mapDisplay->installEventFilter(m_zoomTool);
  }
}

void te::qt::widgets::RasterNavigatorWidget::hideColorCompositionTool(bool hide)
{
  m_ui->m_ccFrame->setVisible(!hide);
}

void te::qt::widgets::RasterNavigatorWidget::hideEditionTools(bool hide)
{
  m_ui->m_toolLine->setVisible(!hide);
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

void te::qt::widgets::RasterNavigatorWidget::hideExtraDisplaysTool(bool hide)
{
  m_ui->m_extraDisplaysToolButton->setChecked(false);

  m_ui->m_extraLine->setVisible(!hide);
  m_ui->m_extraDisplaysToolButton->setVisible(!hide);
}

void te::qt::widgets::RasterNavigatorWidget::onCoordTrackedChanged(QPointF& coordinate)
{
  assert(m_layer.get());

  if(m_ui->m_extraDisplaysToolButton->isChecked())
  {
    //draw cursor position
    m_zoomInMapDisplay->drawCursorPosition((double) coordinate.rx(), (double)coordinate.ry());
    

    //draw zoom in rectangle
    te::gm::Envelope ext = m_zoomInMapDisplay->getCurrentExtent();

    m_mapDisplay->getDraftPixmap()->fill(QColor(0, 0, 0, 0));
    const te::gm::Envelope& mapExt = m_mapDisplay->getExtent();
    te::qt::widgets::Canvas canvasInstance(m_mapDisplay->getDraftPixmap());

    canvasInstance.setWindow(mapExt.m_llx, mapExt.m_lly, mapExt.m_urx, mapExt.m_ury);
    canvasInstance.setPolygonContourColor(te::color::RGBAColor(255,0,0,TE_OPAQUE));
    canvasInstance.setPolygonFillColor(te::color::RGBAColor(0,0,0,TE_TRANSPARENT));

    te::gm::Geometry* geom = te::gm::GetGeomFromEnvelope(&ext, m_layer->getSRID());
    canvasInstance.draw(geom);
    delete geom;

    m_mapDisplay->repaint();
  }

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
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
  te::gm::Envelope e = m_mapDisplay->getExtent();

  //emit signal
  if(e.isValid())
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

  te::qt::widgets::ZoomClick* zoomOut = new te::qt::widgets::ZoomClick(m_mapDisplay, Qt::BlankCursor, 2.0, te::qt::widgets::Zoom::Out);
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

void te::qt::widgets::RasterNavigatorWidget::onExtraDisplaysToggled(bool checked)
{
  m_ui->m_extraDisplaysFrame->setVisible(checked);
  m_eyeBirdMapDisplay->setEnabled(checked);
  m_zoomInMapDisplay->setEnabled(checked);
}

void te::qt::widgets::RasterNavigatorWidget::onRecomposeClicked()
{
  te::gm::Envelope env = m_layer->getExtent();

  m_mapDisplay->setExtent(env);

  //m_eyeBirdMapDisplay->recompose(env);
}

void te::qt::widgets::RasterNavigatorWidget::onRedComboBoxActivated(int index)
{
  std::string name = m_ui->m_redComboBox->itemText(index).toStdString();

  m_symbolizer->getChannelSelection()->getRedChannel()->setSourceChannelName(name);

  m_mapDisplay->refresh();
}

void te::qt::widgets::RasterNavigatorWidget::onGreenComboBoxActivated(int index)
{
  std::string name = m_ui->m_greenComboBox->itemText(index).toStdString();

  m_symbolizer->getChannelSelection()->getGreenChannel()->setSourceChannelName(name);

  m_mapDisplay->refresh();
}

void te::qt::widgets::RasterNavigatorWidget::onBlueComboBoxActivated(int index)
{
  std::string name = m_ui->m_blueComboBox->itemText(index).toStdString();

  m_symbolizer->getChannelSelection()->getBlueChannel()->setSourceChannelName(name);

  m_mapDisplay->refresh();
}

void te::qt::widgets::RasterNavigatorWidget::onMonoComboBoxActivated(int index)
{
  std::string name = m_ui->m_monoComboBox->itemText(index).toStdString();

  m_symbolizer->getChannelSelection()->getGrayChannel()->setSourceChannelName(name);

  m_mapDisplay->refresh();
}

void te::qt::widgets::RasterNavigatorWidget::onMonoToolClicked(bool flag)
{
  if(m_symbolizer->getChannelSelection()->getGrayChannel() == 0)
  {
    te::se::SelectedChannel* scMono = new te::se::SelectedChannel();
    scMono->setSourceChannelName(m_ui->m_monoComboBox->currentText().toStdString());
    m_symbolizer->getChannelSelection()->setGrayChannel(scMono);
  }

  m_symbolizer->getChannelSelection()->setColorCompositionType(te::se::GRAY_COMPOSITION);

  getCompositionInfo();

  m_mapDisplay->refresh();
}

void te::qt::widgets::RasterNavigatorWidget::onRedToolClicked(bool flag)
{
  if(m_symbolizer->getChannelSelection()->getRedChannel() == 0)
  {
    te::se::SelectedChannel* scRed = new te::se::SelectedChannel();
    scRed->setSourceChannelName(m_ui->m_redComboBox->currentText().toStdString());
    m_symbolizer->getChannelSelection()->setRedChannel(scRed);
  }

  m_symbolizer->getChannelSelection()->setColorCompositionType(te::se::RED_COMPOSITION);

  getCompositionInfo();

  m_mapDisplay->refresh();
}

void te::qt::widgets::RasterNavigatorWidget::onGreenToolClicked(bool flag)
{
  if(m_symbolizer->getChannelSelection()->getGreenChannel() == 0)
  {
    te::se::SelectedChannel* scGreen = new te::se::SelectedChannel();
    scGreen->setSourceChannelName(m_ui->m_greenComboBox->currentText().toStdString());
    m_symbolizer->getChannelSelection()->setGreenChannel(scGreen);
  }

  m_symbolizer->getChannelSelection()->setColorCompositionType(te::se::GREEN_COMPOSITION);

  getCompositionInfo();

  m_mapDisplay->refresh();
}

void te::qt::widgets::RasterNavigatorWidget::onBlueToolClicked(bool flag)
{
  if(m_symbolizer->getChannelSelection()->getBlueChannel() == 0)
  {
    te::se::SelectedChannel* scBlue = new te::se::SelectedChannel();
    scBlue->setSourceChannelName(m_ui->m_blueComboBox->currentText().toStdString());
    m_symbolizer->getChannelSelection()->setBlueChannel(scBlue);
  }

  m_symbolizer->getChannelSelection()->setColorCompositionType(te::se::BLUE_COMPOSITION);

  getCompositionInfo();

  m_mapDisplay->refresh();
}

void te::qt::widgets::RasterNavigatorWidget::onCompositionToolClicked(bool flag)
{
  if(m_symbolizer->getChannelSelection()->getRedChannel() == 0)
  {
    te::se::SelectedChannel* scRed = new te::se::SelectedChannel();
    scRed->setSourceChannelName(m_ui->m_redComboBox->currentText().toStdString());
    m_symbolizer->getChannelSelection()->setRedChannel(scRed);
  }

  if(m_symbolizer->getChannelSelection()->getGreenChannel() == 0)
  {
    te::se::SelectedChannel* scGreen = new te::se::SelectedChannel();
    scGreen->setSourceChannelName(m_ui->m_greenComboBox->currentText().toStdString());
    m_symbolizer->getChannelSelection()->setGreenChannel(scGreen);
  }

  if(m_symbolizer->getChannelSelection()->getBlueChannel() == 0)
  {
    te::se::SelectedChannel* scBlue = new te::se::SelectedChannel();
    scBlue->setSourceChannelName(m_ui->m_blueComboBox->currentText().toStdString());
    m_symbolizer->getChannelSelection()->setBlueChannel(scBlue);
  }

  m_symbolizer->getChannelSelection()->setColorCompositionType(te::se::RGB_COMPOSITION);

  getCompositionInfo();

  m_mapDisplay->refresh();
}

void te::qt::widgets::RasterNavigatorWidget::onPreviewClicked()
{
  emit previewClicked();
}

void te::qt::widgets::RasterNavigatorWidget::setCurrentTool(te::qt::widgets::AbstractTool* tool)
{
  delete m_tool;
  m_tool = tool;

  m_mapDisplay->installEventFilter(m_tool);
}

void te::qt::widgets::RasterNavigatorWidget::listBands()
{
  m_ui->m_redComboBox->clear();
  m_ui->m_greenComboBox->clear();
  m_ui->m_blueComboBox->clear();
  m_ui->m_monoComboBox->clear();

  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
      {
        m_ui->m_redComboBox->addItem(QString::number(i));
        m_ui->m_greenComboBox->addItem(QString::number(i));
        m_ui->m_blueComboBox->addItem(QString::number(i));
        m_ui->m_monoComboBox->addItem(QString::number(i));
      }
    }
  }
}

void te::qt::widgets::RasterNavigatorWidget::getCompositionInfo()
{
  assert(m_layer);

  te::se::Style* style = m_layer->getStyle();
  assert(style);

// should I render this style?
  te::se::CoverageStyle* cs = dynamic_cast<te::se::CoverageStyle*>(style);
  assert(cs);

// get the raster symbolizer
  std::size_t nRules = cs->getRules().size();
  assert(nRules >= 1);

// for while, consider one rule
  const te::se::Rule* rule = cs->getRule(0);

  const std::vector<te::se::Symbolizer*>& symbolizers = rule->getSymbolizers();
  assert(!symbolizers.empty());

// for while, consider one raster symbolizer
  m_symbolizer = dynamic_cast<te::se::RasterSymbolizer*>(symbolizers[0]);
  assert(m_symbolizer);

  if(m_symbolizer->getChannelSelection())
  {
    m_ui->m_monoComboBox->setEnabled(false);
    m_ui->m_redComboBox->setEnabled(false);
    m_ui->m_greenComboBox->setEnabled(false);
    m_ui->m_blueComboBox->setEnabled(false);

    te::se::ChannelSelection* cs = m_symbolizer->getChannelSelection();

    if(cs->getColorCompositionType() == te::se::RGB_COMPOSITION)
    {
      m_ui->m_compositionToolButton->setChecked(true);

      std::string name = cs->getRedChannel()->getSourceChannelName();
      setComboBoxText(m_ui->m_redComboBox, name);
      name = cs->getGreenChannel()->getSourceChannelName();
      setComboBoxText(m_ui->m_greenComboBox, name);
      name = cs->getBlueChannel()->getSourceChannelName();
      setComboBoxText(m_ui->m_blueComboBox, name);

      m_ui->m_redComboBox->setEnabled(true);
      m_ui->m_greenComboBox->setEnabled(true);
      m_ui->m_blueComboBox->setEnabled(true);
    }
    else if(cs->getColorCompositionType() == te::se::GRAY_COMPOSITION)
    {
      m_ui->m_monoToolButton->setChecked(true);

      std::string name = cs->getGrayChannel()->getSourceChannelName();
      setComboBoxText(m_ui->m_redComboBox, name);

      m_ui->m_monoComboBox->setEnabled(true);
    }
    else if(cs->getColorCompositionType() == te::se::RED_COMPOSITION)
    {
      m_ui->m_redToolButton->setChecked(true);

      std::string name = cs->getRedChannel()->getSourceChannelName();
      setComboBoxText(m_ui->m_redComboBox, name);

      m_ui->m_redComboBox->setEnabled(true);
    }
    else if(cs->getColorCompositionType() == te::se::GREEN_COMPOSITION)
    {
      m_ui->m_greenToolButton->setChecked(true);

      std::string name = cs->getGreenChannel()->getSourceChannelName();
      setComboBoxText(m_ui->m_greenComboBox, name);

      m_ui->m_greenComboBox->setEnabled(true);
    }
    else if(cs->getColorCompositionType() == te::se::BLUE_COMPOSITION)
    {
      m_ui->m_blueToolButton->setChecked(true);

      std::string name = cs->getBlueChannel()->getSourceChannelName();
      setComboBoxText(m_ui->m_blueComboBox, name);

      m_ui->m_blueComboBox->setEnabled(true);
    }
  }
}

void te::qt::widgets::RasterNavigatorWidget::setComboBoxText(QComboBox* cb, std::string value)
{
  QString name = value.c_str();

  bool found = false;

  for(int i = 0; i < cb->count(); ++i)
  {
    if(cb->itemText(i) == name)
    {
      cb->setCurrentIndex(i);
      found = true;
      break;
    }
  }

  if(!found)
  {
    cb->addItem(name);
    cb->setCurrentIndex(cb->count() - 1);
  }
}
