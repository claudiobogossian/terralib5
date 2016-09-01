/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/rp/RpToolsWidget.cpp

  \brief This file has the RpToolsWidget class.
*/

// TerraLib
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
#include "../../widgets/tools/ExtentAcquire.h"
#include "../../widgets/tools/PolygonAcquire.h"
#include "../../widgets/tools/PointPicker.h"
#include "../../widgets/tools/ReadPixelTool.h"
#include "../canvas/Canvas.h"
#include "../canvas/EyeBirdMapDisplayWidget.h"
#include "../canvas/MultiThreadMapDisplay.h"
#include "../canvas/ZoomInMapDisplayWidget.h"
#include "RpToolsWidget.h"
#include "ui_RpToolsWidgetForm.h"


//QT
#include <QUndoCommand>
#include <qactiongroup.h>

//STL
#include <memory>


te::qt::widgets::RpToolsWidget::RpToolsWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::RpToolsWidgetForm),
    m_symbolizer(0),
    m_draftOriginal(0),
    m_mapDisplay(0),
    m_actionGroup(0),
    m_checkedTool(0)
{
  m_ui->setupUi(this);
  m_pointCursor = Qt::CrossCursor;

//signals & slots
  connect(m_ui->m_undoActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onUndoToggled(bool)));
  connect(m_ui->m_redoActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onRedoToggled(bool)));

  connect(m_ui->m_geomActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onGeomToggled(bool)));
  connect(m_ui->m_extentActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onBoxToggled(bool)));

  connect(m_ui->m_pointActionToolButtontoolButton, SIGNAL(toggled(bool)), this, SLOT(onPointPickerToggled(bool)));
  connect(m_ui->m_readPixelActionToolButton, SIGNAL(toggled(bool)), this, SLOT(onReadPixelToggled(bool)));

//pixmap
  m_ui->m_undoActionToolButtontoolButton->setIcon(QIcon::fromTheme("edit-undo"));
  m_ui->m_redoActionToolButtontoolButton->setIcon(QIcon::fromTheme("edit-redo"));

  m_ui->m_geomActionToolButtontoolButton->setIcon(QIcon::fromTheme("edit-polygon"));
  m_ui->m_extentActionToolButtontoolButton->setIcon(QIcon::fromTheme("edit-box"));

  m_ui->m_pointActionToolButtontoolButton->setIcon(QIcon::fromTheme("placemark"));
  m_ui->m_readPixelActionToolButton->setIcon(QIcon::fromTheme("color-picker"));

  m_ui->m_undoActionToolButtontoolButton->setVisible(false);
  m_ui->m_redoActionToolButtontoolButton->setVisible(false);
  m_ui->m_toolLine->setVisible(false);
}

te::qt::widgets::RpToolsWidget::~RpToolsWidget()
{
  if(m_layer.get())
    m_layer->setVisibility(m_visibility);

  if (m_checkedTool)
  {
    m_mapDisplay->setCursor(Qt::ArrowCursor);
    m_mapDisplay->setCurrentTool(0, false);
  }

  delete m_draftOriginal;
}

void te::qt::widgets::RpToolsWidget::set(te::map::AbstractLayerPtr layer, bool setFullScaleBox)
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
}

void te::qt::widgets::RpToolsWidget::setVectorial(te::map::AbstractLayerPtr layer)
{
  std::list<te::map::AbstractLayerPtr> list;
  list.push_back(layer);
  list.push_back(m_layer);

  m_mapDisplay->setLayerList(list);

  te::gm::Envelope e = m_mapDisplay->getExtent();
  m_mapDisplay->setExtent(e, true);
}

void te::qt::widgets::RpToolsWidget::setExtent(te::gm::Envelope env)
{
  //m_mapDisplay->setExtent(env, true);
}

void te::qt::widgets::RpToolsWidget::removeVectorial()
{
  std::list<te::map::AbstractLayerPtr> list;
  list.push_back(m_layer);

  m_mapDisplay->setLayerList(list);

  te::gm::Envelope e = m_mapDisplay->getExtent();
  m_mapDisplay->setExtent(e, true);
}

te::gm::Envelope te::qt::widgets::RpToolsWidget::getCurrentExtent()
{
  return m_mapDisplay->getExtent();
}

void te::qt::widgets::RpToolsWidget::setMapDisplay(te::qt::widgets::MapDisplay* mapDisplay)
{
	m_mapDisplay = mapDisplay;
}

void te::qt::widgets::RpToolsWidget::setActionGroup(QActionGroup* actionGroup)
{
  m_actionGroup = actionGroup;
}

te::rst::Raster* te::qt::widgets::RpToolsWidget::getExtentRaster(bool fullScale)
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

void te::qt::widgets::RpToolsWidget::drawRaster(te::rst::Raster* rst, te::se::Style* style)
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
  te::map::DrawRaster(rst, &canvas, env, m_mapDisplay->getSRID(), envRst, rst->getSRID(), cs, m_mapDisplay->getScale());

  if(m_draftOriginal)
    delete m_draftOriginal;

  m_draftOriginal = new QPixmap(*m_mapDisplay->getDraftPixmap());

  if(hasToDelete)
    delete style;

  m_mapDisplay->repaint();
}

void te::qt::widgets::RpToolsWidget::hideEditionTools(bool hide)
{
  m_ui->m_toolLine->setVisible(!hide);
  hidePickerTool(hide);
  hideGeomTool(hide);
  hideBoxTool(hide);
  hideInfoTool(hide);
}

void te::qt::widgets::RpToolsWidget::hideUndoTool(bool hide)
{
  m_ui->m_undoActionToolButtontoolButton->setVisible(!hide);
}

void te::qt::widgets::RpToolsWidget::hideRedoTool(bool hide)
{
  m_ui->m_redoActionToolButtontoolButton->setVisible(!hide);
}

void te::qt::widgets::RpToolsWidget::hidePickerTool(bool hide)
{
  m_ui->m_pointActionToolButtontoolButton->setVisible(!hide);
}

void te::qt::widgets::RpToolsWidget::hideGeomTool(bool hide)
{
  m_ui->m_geomActionToolButtontoolButton->setVisible(!hide);
}

void te::qt::widgets::RpToolsWidget::hideBoxTool(bool hide)
{
  m_ui->m_extentActionToolButtontoolButton->setVisible(!hide);
}

void te::qt::widgets::RpToolsWidget::hideInfoTool(bool hide)
{
  m_ui->m_readPixelActionToolButton->setVisible(!hide);
}

void te::qt::widgets::RpToolsWidget::enableUndoAction()
{
  QAction* undo = new QAction(this);
  undo->setIcon(QIcon::fromTheme("edit-undo"));
  undo->setToolTip(tr("Undo"));
  undo->setCheckable(true);
  undo->setEnabled(true);

  m_ui->m_undoActionToolButtontoolButton->setDefaultAction(undo);

  m_actionGroup->addAction(m_ui->m_undoActionToolButtontoolButton->defaultAction());
}

void te::qt::widgets::RpToolsWidget::enableRedoAction()
{
  QAction* redo = new QAction(this);
  redo->setIcon(QIcon::fromTheme("edit-undo"));
  redo->setToolTip(tr("Redo"));
  redo->setCheckable(true);
  redo->setEnabled(true);;

  m_ui->m_redoActionToolButtontoolButton->setDefaultAction(redo);

  m_actionGroup->addAction(m_ui->m_redoActionToolButtontoolButton->defaultAction());
}

void te::qt::widgets::RpToolsWidget::enablePickerAction()
{
  QAction* pointPicker = new QAction(this);
  pointPicker->setIcon(QIcon::fromTheme("placemark"));
  pointPicker->setToolTip(tr("Create by Point"));
  pointPicker->setCheckable(true);
  pointPicker->setEnabled(true);

  m_ui->m_pointActionToolButtontoolButton->setDefaultAction(pointPicker);

  m_actionGroup->addAction(m_ui->m_pointActionToolButtontoolButton->defaultAction());
}

void te::qt::widgets::RpToolsWidget::enableGeomAction()
{
  QAction* geom = new QAction(this);
  geom->setIcon(QIcon::fromTheme("edit-polygon"));
  geom->setToolTip(tr("Create by Polygon"));
  geom->setCheckable(true);
  geom->setEnabled(true);

  m_ui->m_geomActionToolButtontoolButton->setDefaultAction(geom);

  m_actionGroup->addAction(m_ui->m_geomActionToolButtontoolButton->defaultAction());
}

void te::qt::widgets::RpToolsWidget::enableBoxAction()
{
  QAction* extent = new QAction(this);
  extent->setIcon(QIcon::fromTheme("edit-box"));
  extent->setToolTip(tr("Create by Extent"));
  extent->setCheckable(true);
  extent->setEnabled(true);

  m_ui->m_extentActionToolButtontoolButton->setDefaultAction(extent);

  m_actionGroup->addAction(m_ui->m_extentActionToolButtontoolButton->defaultAction());
}

void te::qt::widgets::RpToolsWidget::enableInfoAction()
{
  QAction* readPixel = new QAction(this);
  readPixel->setIcon(QIcon::fromTheme("color-picker"));
  readPixel->setToolTip(tr("Read Pixel"));
  readPixel->setCheckable(true);
  readPixel->setEnabled(true);

  m_ui->m_readPixelActionToolButton->setDefaultAction(readPixel);

  m_actionGroup->addAction(m_ui->m_readPixelActionToolButton->defaultAction());
}

void te::qt::widgets::RpToolsWidget::setSelectionMode(bool mode)
{
  if(mode)
  {
    m_ui->m_pointActionToolButtontoolButton->setIcon(QIcon::fromTheme("pointer-selection"));
    m_pointCursor = Qt::ArrowCursor;
  }
  else
  {
    m_ui->m_pointActionToolButtontoolButton->setIcon(QIcon::fromTheme("placemark"));
    m_pointCursor = Qt::CrossCursor;
  }
}

void te::qt::widgets::RpToolsWidget::onEnvelopeAcquired(te::gm::Envelope env)
{
  //emit signal
  emit envelopeAcquired(env);
}

void te::qt::widgets::RpToolsWidget::onGeomAquired(te::gm::Polygon* poly)
{
  //emit signal
  emit geomAquired(poly);
}

void te::qt::widgets::RpToolsWidget::onPointPicked(QPointF& point)
{
  //emit signal
  emit pointPicked(point.x(), point.y());
}

void te::qt::widgets::RpToolsWidget::onMapDisplayExtentChanged()
{
  te::gm::Envelope e = m_mapDisplay->getExtent();

  //emit signal
  if(e.isValid())
    emit mapDisplayExtentChanged();

  delete m_draftOriginal;
  m_draftOriginal = 0;
}

void te::qt::widgets::RpToolsWidget::onUndoToggled(bool checked)
{

}

void te::qt::widgets::RpToolsWidget::onRedoToggled(bool checked)
{

}

void te::qt::widgets::RpToolsWidget::onPointPickerToggled(bool checked)
{
  m_checkedTool = checked;

  if (!checked)
    return;

  te::qt::widgets::PointPicker* pp = new te::qt::widgets::PointPicker(m_mapDisplay, m_pointCursor);
  m_mapDisplay->setCurrentTool(pp);
  
  connect(pp, SIGNAL(pointPicked(QPointF&)), this, SLOT(onPointPicked(QPointF&)));
}

void te::qt::widgets::RpToolsWidget::onGeomToggled(bool checked)
{
  m_checkedTool = checked;
 
  if (!checked)
    return;

  te::qt::widgets::PolygonAcquire* pa = new te::qt::widgets::PolygonAcquire(m_mapDisplay);
  m_mapDisplay->setCurrentTool(pa);

  connect(pa, SIGNAL(polygonAquired(te::gm::Polygon*)), this, SLOT(onGeomAquired(te::gm::Polygon*)));
}

void te::qt::widgets::RpToolsWidget::onBoxToggled(bool checked)
{
  m_checkedTool = checked;

  if (!checked)
    return;

  te::qt::widgets::ExtentAcquire* ea = new te::qt::widgets::ExtentAcquire(m_mapDisplay, Qt::ArrowCursor);
  m_mapDisplay->setCurrentTool(ea);

  connect(ea, SIGNAL(extentAcquired(te::gm::Envelope)), this, SLOT(onEnvelopeAcquired(te::gm::Envelope)));
}

void te::qt::widgets::RpToolsWidget::onReadPixelToggled(bool checked)
{
  m_checkedTool = checked;

  if (!checked)
    return;

  te::qt::widgets::ReadPixelTool* pa = new te::qt::widgets::ReadPixelTool(m_mapDisplay, m_layer);
  m_mapDisplay->setCurrentTool(pa);
}

void te::qt::widgets::RpToolsWidget::onRecomposeClicked()
{
  te::gm::Envelope env = m_layer->getExtent();

  m_mapDisplay->setExtent(env);

  //m_eyeBirdMapDisplay->recompose(env);
}

void te::qt::widgets::RpToolsWidget::onPreviewClicked()
{
  emit previewClicked();
}

void te::qt::widgets::RpToolsWidget::onVSliderChanged(int value)
{
  drawOverlay();
}

void te::qt::widgets::RpToolsWidget::onHSliderChanged(int value)
{
  drawOverlay();
}

void te::qt::widgets::RpToolsWidget::setComboBoxText(QComboBox* cb, std::string value)
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

void te::qt::widgets::RpToolsWidget::drawOverlay()
{
  if(!m_draftOriginal)
    return;

  QImage img = m_draftOriginal->toImage();

  QPixmap* draft = m_mapDisplay->getDraftPixmap();

  draft->fill(Qt::transparent);

  //QPainter p(draft);
  //p.drawImage(0, 0, img, 0, 0, m_ui->m_horizontalSlider->value(), m_ui->m_verticalSlider->value());
  //p.end();

  m_mapDisplay->repaint();
}
